// Copyright (c) 2014, Michael Boyle
// See LICENSE file for details

#include "Quaternions.hpp"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <math.h> // acosh is in math.h, but not cmath
#include <algorithm>
#include "QuaternionUtilities.hpp"
#include "Errors.hpp"

// These macros are useful for debugging
#define INFOTOCERR std::cerr << __FILE__ << ":" << __LINE__ << ":" << __func__ << ": "
#define INFOTOCOUT std::cout << __FILE__ << ":" << __LINE__ << ":" << __func__ << ": "

// Note: Don't do 'using namespace Quaternions' because we don't want
// to confuse which log, exp, etc., is being used in any instance.
using Quaternions::Quaternion;
using Quaternions::QuaternionArray;
using Quaternions::yHat;
using Quaternions::zHat;


// Note: Don't do 'using namespace std' because we don't want
// to confuse which log, exp, etc., is being used in any instance.
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::vector;




/////////////////////
// Local functions //
/////////////////////

/// Integrate scalar function by simple trapezoidal rule.
std::vector<double> ScalarIntegral(const std::vector<double>& fdot, const std::vector<double>& t) {
  ///
  /// \param fdot Vector of scalars.
  /// \param t Vector of corresponding time steps.
  if(fdot.size() != t.size()) {
    INFOTOCERR << " fdot.size()=" << fdot.size() << " != t.size()=" << t.size() << endl;
    throw(VectorSizeMismatch);
  }
  const unsigned int Size=fdot.size();
  vector<double> f(Size);
  f[0] = 0.0;
  for(unsigned int i=1; i<Size; ++i) {
    f[i] = f[i-1] + (t[i]-t[i-1])*(fdot[i]+fdot[i-1])/2.0;
  }
  return f;
}

#ifndef DOXYGEN
// Some utility operators for std::vector, etc.
inline double SQR(const double& x) { return x*x; }
namespace Quaternions {
  std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2) {
    const unsigned int size = v1.size();
    vector<double> v3(v1);
    for(unsigned int i=0; i<size; ++i) {
      v3[i] += v2[i];
    }
    return v3;
  }
  std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2) {
    const unsigned int size = v1.size();
    vector<double> v3(v1);
    for(unsigned int i=0; i<size; ++i) {
      v3[i] -= v2[i];
    }
    return v3;
  }
  std::vector<double> operator*(const std::vector<double>& v, const double a) {
    const unsigned int size = v.size();
    vector<double> v2(v);
    for(unsigned int i=0; i<size; ++i) {
      v2[i] *= a;
    }
    return v2;
  }
  vector<double> operator/(const vector<double>& v, const double a) {
    const unsigned int size = v.size();
    vector<double> v2(v);
    for(unsigned int i=0; i<size; ++i) {
      v2[i] /= a;
    }
    return v2;
  }
  std::vector<double> cross(const std::vector<double>& v1, const std::vector<double>& v2) {
    vector<double> v3(3);
    v3[0] = -v1[2]*v2[1]+v1[1]*v2[2];
    v3[1] = v1[2]*v2[0]-v1[0]*v2[2];
    v3[2] = -v1[1]*v2[0]+v1[0]*v2[1];
    return v3;
  }
  double dot(const std::vector<double>& v1, const std::vector<double>& v2) {
    const unsigned int size = v1.size();
    double d=0.0;
    for(unsigned int i=0; i<size; ++i) {
      d += v1[i]*v2[i];
    }
    return d;
  }
};
#endif // DOXYGEN




////////////////////////////////////
// Functions for the class itself //
////////////////////////////////////

/// Empty constructor -- initialized to 0s.
Quaternions::Quaternion::Quaternion()
  : w(0.0), x(0.0), y(0.0), z(0.0) { }

/// Copy constructor.
Quaternions::Quaternion::Quaternion(const Quaternion& Q)
  : w(Q.w), x(Q.x), y(Q.y), z(Q.z) { }

/// Constructor from spherical coordinates.
Quaternions::Quaternion::Quaternion(const double vartheta, const double varphi) {
  ///
  /// \param vartheta Float representing the polar angle
  /// \param varphi Float representing the azimuthal angle
  ///
  /// The unit Quaternion constructed in this way rotates the z axis
  /// onto the point given by the coordinates (vartheta, varphi).
  *this = Quaternions::exp((varphi/2.)*zHat) * Quaternions::exp((vartheta/2.)*yHat);
}

/// Constructor from Euler angles.
Quaternions::Quaternion::Quaternion(const double alpha, const double beta, const double gamma) {
  ///
  /// \param alpha First Euler angle
  /// \param beta Second Euler angle
  /// \param gamma Third Euler angle
  ///
  /// The unit Quaternion constructed in this way corresponds to a
  /// rotation by the given Euler angles.  The convention used here is
  /// the z-y-z convention.  That is, the rotations occur about the
  /// fixed axes: first a rotation by gamma about the z axis, then a
  /// rotation by beta about the y axis, and finally a rotation by
  /// alpha about the z axis.
  *this = Quaternions::exp((alpha/2.)*zHat) * Quaternions::exp((beta/2.)*yHat) * Quaternions::exp((gamma/2.)*zHat);
}

/// Constructor by components.
Quaternions::Quaternion::Quaternion(const double w0, const double x0, const double y0, const double z0)
  : w(w0), x(x0), y(y0), z(z0)
{
  ///
  /// \param w0 Scalar component of Quaternion
  /// \param x0 First vector component of Quaternion
  /// \param y0 Second vector component of Quaternion
  /// \param z0 Third vector component of Quaternion
}

/// Constructor from vector.
Quaternions::Quaternion::Quaternion(const std::vector<double>& q) {
  ///
  /// \param q Vector containing three or four components
  ///
  /// If the input vector has three components, they are assumed to
  /// represent the vector components of the Quaternion, and the
  /// scalar component is set to zero.  If the input vector has four
  /// components, they are assumed to represent the four components of
  /// the Quaternion, with the 0 component being the scalar part.
  if(q.size()==3) {
    w = 0.0;
    x = q[0];
    y = q[1];
    z = q[2];
  } else if(q.size()==4) {
    w = q[0];
    x = q[1];
    y = q[2];
    z = q[3];
  } else {
    INFOTOCERR << " q.size()=" << q.size() << endl;
    throw(VectorSizeNotUnderstood);
  }
}

/// Constructor from axis-angle.
Quaternions::Quaternion::Quaternion(const double angle, const std::vector<double>& axis)
  : w(std::cos(angle/2.)), x(std::sin(angle/2.)*axis[0]), y(std::sin(angle/2.)*axis[1]), z(std::sin(angle/2.)*axis[2])
{
  ///
  /// \param angle Single number giving the rotation angle
  /// \param axis Three-component vector (assumed to be normalized) giving the axis
  ///
  /// This constructs a rotor (assuming 'axis' is normalized)
  /// corresponding to rotation about the given axis through the given
  /// angle.
}

/// Get component of Quaternion.
double Quaternions::Quaternion::operator[](const unsigned int i) const {
  /// The 0 component is the scalar part, and the 1--3 components are
  /// the vector components.
  switch(i) {
  case 0:
    return w;
  case 1:
    return x;
  case 2:
    return y;
  case 3:
    return z;
  default:
    INFOTOCERR << " i=" << i << " is not a possible quaternion index" << endl;
    throw(IndexOutOfBounds);
  }
}

/// Get reference to component of Quaternion.
double& Quaternions::Quaternion::operator[](const unsigned int i) {
  /// Note: This is unavailable from python.
  switch(i) {
  case 0:
    return w;
  case 1:
    return x;
  case 2:
    return y;
  case 3:
    return z;
  default:
    INFOTOCERR << " i=" << i << " is not a possible quaternion index" << endl;
    throw(IndexOutOfBounds);
  }
}

/// Quaternion multiplication.
Quaternion Quaternions::Quaternion::operator*(const Quaternion& Q) const {
  return Quaternion(w*Q.w - x*Q.x - y*Q.y - z*Q.z,
            w*Q.x + x*Q.w + y*Q.z - z*Q.y,
            w*Q.y - x*Q.z + y*Q.w + z*Q.x,
            w*Q.z + x*Q.y - y*Q.x + z*Q.w);
}

/// Return logarithm of Quaternion.
Quaternion Quaternions::Quaternion::log() const {
  Quaternion Result;
  const double b = std::sqrt(x*x + y*y + z*z);
  if(std::abs(b) <= Quaternion_Epsilon*std::abs(w)) {
    if(w<0.0) {
      INFOTOCERR << " Error: Infinitely many solutions for log of a negative scalar (w=" << w << ")." << endl;
      throw(InfinitelyManySolutions);
    }
    Result.w = std::log(w);
  } else {
    const double v = std::atan2(b, w);
    const double f = v/b;
    Result.w = std::log(w*w+b*b)/2.0;
    // Result.w = std::log(w/std::cos(v)); // Not nice for unit vectors [w=cos(v)=0]
    Result.x = f*x;
    Result.y = f*y;
    Result.z = f*z;
  }
  return Result;
}

/// Return logarithm of a rotor.
Quaternion Quaternions::Quaternion::logRotor() const {
  /// This function is just like the standard log function, except
  /// that a negative scalar does not raise an exception; instead, the
  /// scalar pi is returned.
  Quaternion Result;
  const double b = std::sqrt(x*x + y*y + z*z);
  if(std::abs(b) <= Quaternion_Epsilon*std::abs(w)) {
    if(w<0.0) {
      INFOTOCERR
           << "\nWarning: Infinitely many solutions for log of a negative scalar (w=" << w << "); "
           << "arbitrarily returning the one in the x direction." << endl;
      Result.x = M_PI;
      if(std::abs(w+1)>Quaternion_Epsilon) {
        Result.w = std::log(-w);
      }
    } else {
      Result.w = std::log(w);
    }
  } else {
    const double v = std::atan2(b, w);
    const double f = v/b;
    Result.w = std::log(w*w+b*b)/2.0;
    // Result.w = std::log(w/std::cos(v)); // Not nice for unit vectors [w=cos(v)=0]
    Result.x = f*x;
    Result.y = f*y;
    Result.z = f*z;
  }
  return Result;
}

/// Return exponent of Quaternion.
Quaternion Quaternions::Quaternion::exp() const {
  Quaternion Result;
  const double b = std::sqrt(x*x + y*y + z*z);
  if(std::abs(b)<=Quaternion_Epsilon*std::abs(w)) {
    Result.w = std::exp(w);
  } else {
    const double e = std::exp(w);
    const double f = std::sin(b)/b; // Note: b is never 0.0 at this point
    Result.w = e*std::cos(b);
    Result.x = e*f*x;
    Result.y = e*f*y;
    Result.z = e*f*z;
  }
  return Result;
}

/// Return a space-separated copy of the values
std::string Quaternions::Quaternion::str() const {
  std::stringstream ss;
  ss << w << " " << x << " " << y << " " << z;
  return ss.str();
}

/// Print the quaternion nicely to stream
std::ostream& Quaternions::operator<<(std::ostream& out, const Quaternions::Quaternion& q) {
  out << "[" << q[0] << ", " << q[1] << ", " << q[2] << ", " << q[3] << "]";
  return out;
}


/////////////////////
// Array operators //
/////////////////////

/// Simple component-wise integration, returning integral at each input time
std::vector<Quaternion> Quaternions::IndefiniteIntegral(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn) {
  ///
  /// \param RIn Vector of Quaternions
  /// \param tIn Vector of corresponding time steps
  if(RIn.size() != tIn.size()) {
    INFOTOCERR << " RIn.size()=" << RIn.size() << " != tIn.size()=" << tIn.size() << endl;
    throw(VectorSizeMismatch);
  }
  if(RIn.size()==0) {
    return RIn;
  }
  const unsigned int Size1=RIn.size();
  vector<Quaternion> ROut(Size1, Quaternion());
  // ROut[0] is already zero
  for(unsigned int i=1; i<Size1; ++i) {
    for(unsigned int j=0; j<4; ++j) {
      ROut[i][j] = ROut[i-1][j] + (tIn[i]-tIn[i-1])*(RIn[i][j]+RIn[i-1][j])/2.0;
    }
  }
  return ROut;
}

/// Simple component-wise integration, returning total
Quaternion Quaternions::DefiniteIntegral(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn) {
  ///
  /// \param RIn Vector of Quaternions
  /// \param tIn Vector of corresponding time steps
  if(RIn.size() != tIn.size()) {
    INFOTOCERR << " RIn.size()=" << RIn.size() << " != tIn.size()=" << tIn.size() << endl;
    throw(VectorSizeMismatch);
  }
  if(RIn.size()==0) {
    return Quaternion();
  }
  const unsigned int Size1=RIn.size();
  Quaternion ROut;
  for(unsigned int i=1; i<Size1; ++i) {
    for(unsigned int j=0; j<4; ++j) {
      ROut[j] += (tIn[i]-tIn[i-1])*(RIn[i][j]+RIn[i-1][j])/2.0;
    }
  }
  return ROut;
}


/// Calculate the derivative of a rotor by the logarithm formula.
std::vector<Quaternion> Quaternions::DifferentiateRotorByLogarithm(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn) {
  /// This is a much more complicated way of evaluating the derivative
  /// of a quaternion function of time, as compared to finite
  /// differencing by 'QuaternionDerivative'.  However, there may be
  /// accuracy advantages when the logarithm is smooth, and -- at the
  /// least -- this can serve as a good test of the correctness of the
  /// logarithm formula.
  const vector<Quaternion> logR = Quaternions::logRotor(RIn);
  const vector<Quaternion> rdot = Quaternions::QuaternionDerivative(logR, tIn);
  vector<Quaternion> ROut(RIn.size());
  for(unsigned int i=0; i<logR.size(); ++i) {
    const double absquatlogR = abs(logR[i]);
    if(absquatlogR==0.0) { ROut[i] = rdot[i]; continue; }
    const double absquatlogRsquared = SQR(absquatlogR);
    const double a = SQR(std::sin(absquatlogR)/absquatlogR)/2.0;
    const double b = (absquatlogR<0.001
              ? 0.6666666666666666 + absquatlogRsquared*(-0.13333333333333333 + absquatlogRsquared*(0.012698412698412698 + (-0.0007054673721340388 + (4*absquatlogRsquared)/155925.)*absquatlogRsquared))
              : (absquatlogR-std::sin(absquatlogR)*std::cos(absquatlogR))/(absquatlogRsquared*absquatlogR) ) / 4.0;
    const Quaternion comm = Quaternions::commutator(logR[i],rdot[i]);
    ROut[i] = (rdot[i] + a*comm + b*Quaternions::commutator(logR[i],comm)) * RIn[i];
  }
  return ROut;
}


/// Minimal-rotation version of the input frame.
std::vector<Quaternion> Quaternions::MinimalRotation(const std::vector<Quaternion>& R, const std::vector<double>& T, const unsigned int NIterations) {
  ///
  /// \param R Vector of rotors.
  /// \param T Vector of corresponding time steps.
  /// \param NIterations Number of refinements [default: 5]
  ///
  /// This function returns a copy of the input R, which takes the z
  /// axis to the same point as R, but adjusts the rotation about that
  /// new point by imposing the minimal-rotation condition.
  if(T.size() != R.size()) {
    INFOTOCERR << " T.size()=" << T.size() << " != R.size()=" << R.size() << endl;
    throw(VectorSizeMismatch);
  }
  const unsigned int Size=T.size();
  const Quaternion z(0,0,0,1);
  vector<double> gammaover2dot(Size);
  vector<Quaternion> Rreturn(R);
  for(unsigned int iteration=0; iteration<NIterations; ++iteration) {
    // cout << "\t\tIteration " << iteration << endl;
    const vector<Quaternion> Rdot = Quaternions::QuaternionDerivative(Rreturn, T);
    for(unsigned int i=0; i<Size; ++i) {
      gammaover2dot[i] = ( Rreturn[i].inverse() * Rdot[i] * z )[0];
    }
    const vector<double> gammaover2 = ScalarIntegral(gammaover2dot, T);
    for(unsigned int i=0; i<Size; ++i) {
      Rreturn[i] = Rreturn[i] * (gammaover2[i]*z).exp();
    }
  }
  // cout << "\tFinished" << endl;
  return Rreturn;
}

/// Input frame with prescribed rate of rotation about Z axis.
std::vector<Quaternion> Quaternions::PrescribedRotation(const std::vector<double>& RotationRateAboutZ,
                             const std::vector<Quaternion>& R, const std::vector<double>& T, const unsigned int NIterations) {
  ///
  /// \param RotationRateAboutZ Vector of rotation rates about the new frame's Z axis.
  /// \param R Vector of rotors.
  /// \param T Vector of corresponding time steps.
  /// \param NIterations Number of refinements [default: 5]
  ///
  /// This function returns a copy of the input R, which takes the z
  /// axis to the same point as R, but adjusts the rotation about that
  /// new point by imposing the minimal-rotation condition, and then
  /// including an additional rotation about the new Z axis to agree
  /// with the given rotation rate.

  if(T.size() != R.size() || T.size() != RotationRateAboutZ.size()) {
    INFOTOCERR
      << " T.size()=" << T.size() << " != R.size()=" << R.size()
      << " != RotationRateAboutZ.size()=" << RotationRateAboutZ.size() << endl;
    throw(VectorSizeMismatch);
  }
  const unsigned int Size=T.size();
  const Quaternion z(0,0,0,1);
  vector<double> gammaover2dot(Size);
  vector<Quaternion> Rreturn(R);

  // Repeat the process a few times to refine the result
  for(unsigned int iteration=0; iteration<NIterations; ++iteration) {
    cout << "\t\tIteration " << iteration << endl;
    const vector<Quaternion> Rdot = Quaternions::QuaternionDerivative(Rreturn, T);

    // Calculate \dot{\gamma}/2 at each instant in time
    for(unsigned int i=0; i<Size; ++i) {
      gammaover2dot[i] = 2*RotationRateAboutZ[i] + ( Rreturn[i].inverse() * Rdot[i] * z )[0];
    }

    // Integrate to find \gamma/2 as a function of time
    const vector<double> gammaover2 = ScalarIntegral(gammaover2dot, T);

    // Now pre-multiply the input rotor by exp(\gamma * z / 2) at each instant
    for(unsigned int i=0; i<Size; ++i) {
      Rreturn[i] = Rreturn[i] * (gammaover2[i]*z).exp();
    }
  }

  cout << "\tFinished" << endl;
  return Rreturn;
}

/// Construct frame given the X and Y basis vectors of that frame.
std::vector<Quaternion> Quaternions::FrameFromXY(const std::vector<Quaternion>& X, const std::vector<Quaternion>& Y) {
  ///
  /// \param X Vector of Quaternions
  /// \param Y Vector of Quaternions
  ///
  /// The input parameters are Quaternions, assumed to be pure unit
  /// vectors, representing the X and Y basis vectors of the frame at
  /// each instant of time.  The returned vector of rotors will rotate
  /// the stationary frame's (x,y,z) vectors into the new frame's
  /// (X,Y,Z) vectors.
  if(X.size() != Y.size()) {
    INFOTOCERR << " X.size()=" << X.size() << " != Y.size()=" << Y.size() << endl;
    throw(VectorSizeMismatch);
  }
  const unsigned int Size=X.size();
  const Quaternion x(0,1,0,0);
  const Quaternion y(0,0,1,0);
  const Quaternion z(0,0,0,1);
  vector<Quaternion> R(Size);
  for(unsigned int k=0; k<Size; ++k) {
    const Quaternion Ra = sqrtOfRotor(-X[k]*x);
    const double beta = std::atan2(Quaternions::dot(Ra*z*Ra.inverse(), Y[k]),
                   Quaternions::dot(Ra*y*Ra.inverse(), Y[k]));
    R[k] = Ra * Quaternions::exp((beta/2.0)*x);
  }
  return R;
}

/// Construct minimal-rotation frame from Z basis vector of that frame.
std::vector<Quaternion> Quaternions::FrameFromZ(const std::vector<Quaternion>& Z, const std::vector<double>& T, const unsigned int NIterations) {
  ///
  /// \param Z Vector of Quaternions
  /// \param T Vector of corresponding times
  /// \param NIterations Number of refinements [default: 5]
  ///
  /// The input vector of Quaternions, assumed to be pure unit
  /// vectors, represent the Z basis vectors of the frame at each
  /// instant of time.  The returned vector of rotors will rotate the
  /// stationary frame's (x,y,z) vectors into the new frame's (X,Y,Z)
  /// vectors.  The X and Y vectors are deduced by imposing the
  /// minimal-rotation condition.  Note that this leaves an unfixed
  /// initial rotation about z.
  if(Z.size() != T.size()) {
    INFOTOCERR << " Z.size()=" << Z.size() << " != T.size()=" << T.size() << endl;
    throw(VectorSizeMismatch);
  }
  const unsigned int Size=Z.size();
  const Quaternion z(0,0,0,1);
  vector<Quaternion> R(Size);
  // cout << "\tComputing basic frame" << endl;
  for(unsigned int k=0; k<Size; ++k) {
    R[k] = Quaternions::sqrtOfRotor(-Z[k]*z);
  }
  // cout << "\tRemoving rotation:" << endl;
  return Quaternions::MinimalRotation(Quaternions::UnflipRotors(R), T, NIterations);
}

/// Construct minimal-rotation frame from Z basis vector of that frame.
std::vector<Quaternion> Quaternions::FrameFromPrescribedRotation(const std::vector<Quaternion>& omega, const std::vector<double>& T, const unsigned int NIterations) {
  ///
  /// \param omega Vector of Quaternions
  /// \param T Vector of corresponding times
  /// \param NIterations Number of refinements [default: 5]
  ///
  /// The input vector of Quaternions represent the angular-velocity
  /// vector (omega) of the frame at each instant of time.  The
  /// returned vector of rotors will rotate the stationary frame's
  /// (x,y,z) vectors into the new frame's (X,Y,Z) vectors, where Z is
  /// parallel to omega, and the X and Y vectors are deduced by
  /// enforcing the condition that the instantaneous rotation of the
  /// frame about Z is |omega|.  Note that this leaves an unfixed
  /// initial rotation in the X--Y plane.
  if(omega.size() != T.size()) {
    INFOTOCERR << " omega.size()=" << omega.size() << " != T.size()=" << T.size() << endl;
    throw(VectorSizeMismatch);
  }
  const unsigned int Size=omega.size();
  const Quaternion z(0,0,0,1);
  vector<Quaternion> R(Size);
  cout << "\tComputing basic frame" << endl;
  for(unsigned int k=0; k<Size; ++k) {
    R[k] = Quaternions::sqrt(-omega[k].normalized()*z);
  }
  cout << "\tRemoving rotation:" << endl;
  return Quaternions::PrescribedRotation(abs(omega), Quaternions::UnflipRotors(R), T, NIterations);
}


/// Time-derivative of the quaternion logarithm for frame with given angular velocity
std::vector<double> Quaternions::FrameFromAngularVelocity_Integrand(const std::vector<double>& rfrak, const std::vector<double>& Omega) {
  ///
  /// \param r Quaternion logarithm corresponding to the current frame
  /// \param Omega Quaternion vector giving instantaneous orbital velocity of frame
  ///
  /// This function returns the time-derivative of the quaternion
  /// logarithm of the frame with angular velocity `Omega`.  This can
  /// be integrated to give the quaternion logarithm of the frame,
  /// which can then be exponentiated to give the frame rotor.
  const double rfrakMag = std::sqrt(rfrak[0]*rfrak[0]+rfrak[1]*rfrak[1]+rfrak[2]*rfrak[2]);
  const double OmegaMag = std::sqrt(Omega[0]*Omega[0]+Omega[1]*Omega[1]+Omega[2]*Omega[2]);
  if(rfrakMag < Quaternion_Epsilon * OmegaMag) { // If the matrix is really close to the identity, return
    return Omega/2.0;
  }
  if(std::abs(std::sin(rfrakMag)) < Quaternion_Epsilon) { // If the matrix is really close to singular, it's equivalent to the identity, so return
    return Omega/2.0;
  }
  const vector<double> OmegaOver2 = Omega/2.0;
  const vector<double> rfrakHat = rfrak/rfrakMag;
  return (OmegaOver2 - rfrakHat*dot(rfrakHat,OmegaOver2))*(rfrakMag/std::tan(rfrakMag)) + rfrakHat*dot(rfrakHat,OmegaOver2) + cross(OmegaOver2,rfrak);
}

// /// Time-derivative of quaternion logarithm for vector with given angular velocity
// std::vector<double> Quaternions::FrameFromAngularVelocity_Integrand(const std::vector<double>& rfrak,
//                     const std::vector<double>& Omega) {
//   std::vector<double> rfrakDot(3);
//   const double& rfrak_x = rfrak[0];
//   const double& rfrak_y = rfrak[1];
//   const double& rfrak_z = rfrak[2];
//   double& rfrakDot_x = rfrakDot[0];
//   double& rfrakDot_y = rfrakDot[1];
//   double& rfrakDot_z = rfrakDot[2];
//   const double rfrakMag = std::sqrt(rfrak_x*rfrak_x+rfrak_y*rfrak_y+rfrak_z*rfrak_z);
//   const double OmegaMag = std::sqrt(Omega[0]*Omega[0]+Omega[1]*Omega[1]+Omega[2]*Omega[2]);
//   if(rfrakMag < Quaternion_Epsilon * OmegaMag) { // If the matrix is really close to the identity, return
//     rfrakDot_x = Omega[0]/2.0;
//     rfrakDot_y = Omega[1]/2.0;
//     rfrakDot_z = Omega[2]/2.0;
//     return rfrakDot;
//   }
//   if(std::abs(std::sin(rfrakMag)) < Quaternion_Epsilon) { // If the matrix is really close to singular, it's equivalent to the identity, so return
//     rfrakDot_x = Omega[0]/2.0;
//     rfrakDot_y = Omega[1]/2.0;
//     rfrakDot_z = Omega[2]/2.0;
//     return rfrakDot;
//   }
//   const double dotTerm = (rfrak_x*Omega[0]+rfrak_y*Omega[1]+rfrak_z*Omega[2])/(rfrakMag*rfrakMag);
//   const double cotTerm = rfrakMag/(2*tan(rfrakMag));
//   rfrakDot_x = (Omega[0] - rfrak_x*dotTerm)*cotTerm + rfrak_x*dotTerm/2. - 0.5*Omega[2]*rfrak_y + 0.5*Omega[1]*rfrak_z;
//   rfrakDot_y = (Omega[1] - rfrak_y*dotTerm)*cotTerm + rfrak_y*dotTerm/2. + 0.5*Omega[2]*rfrak_x - 0.5*Omega[0]*rfrak_z;
//   rfrakDot_z = (Omega[2] - rfrak_z*dotTerm)*cotTerm + rfrak_z*dotTerm/2. - 0.5*Omega[1]*rfrak_x + 0.5*Omega[0]*rfrak_y;
//   return rfrakDot;
// }

/// Time-derivative of 2-D quaternion logarithm for vector with given angular velocity
void Quaternions::FrameFromAngularVelocity_2D_Integrand(const double rfrak_x, const double rfrak_y,
                                                        std::vector<double> Omega,
                                                        double& rfrakDot_x, double& rfrakDot_y) {
  const double rfrakMag = std::sqrt(rfrak_x*rfrak_x+rfrak_y*rfrak_y);
  if(std::abs(std::sin(rfrakMag)) < Quaternion_Epsilon) { // If the transformation is really close to the identity, we can just remove the z component
    rfrakDot_x = Omega[0]/2.0;
    rfrakDot_y = Omega[1]/2.0;
    return;
  }
  if(std::abs(std::cos(rfrakMag)) < Quaternion_Epsilon) { // If the transformation is really close to -1, we can just remove the z component
    rfrakDot_x = Omega[0]/2.0;
    rfrakDot_y = Omega[1]/2.0;
    return;
  }
  // sfrakHat = {-rfrak_y/rfrakMag, rfrak_x/rfrakMag, 0.0};
  const double omega_v = (Omega[0]*(-rfrak_y/rfrakMag)+Omega[1]*(rfrak_x/rfrakMag))*std::tan(rfrakMag)-Omega[2];
  Omega[0] += -omega_v*std::sin(2*rfrakMag)*(-rfrak_y/rfrakMag);
  Omega[1] += -omega_v*std::sin(2*rfrakMag)*(rfrak_x/rfrakMag);
  Omega[2] +=  omega_v*std::cos(2*rfrakMag);
  const double dotTerm = (rfrak_x*Omega[0]+rfrak_y*Omega[1])/(rfrakMag*rfrakMag);
  const double cotTerm = rfrakMag/(2*tan(rfrakMag));
  rfrakDot_x = (Omega[0] - rfrak_x*dotTerm)*cotTerm + rfrak_x*dotTerm/2. - 0.5*Omega[2]*rfrak_y;
  rfrakDot_y = (Omega[1] - rfrak_y*dotTerm)*cotTerm + rfrak_y*dotTerm/2. + 0.5*Omega[2]*rfrak_x;
  return;
}


/// Remove sign-ambiguity of rotors.
std::vector<Quaternion> Quaternions::UnflipRotors(const std::vector<Quaternion>& R, const double discont) {
  ///
  /// \param R Vector of rotors
  /// \param discont Acceptable discontinuity [default: sqrt(2)]
  ///
  /// Because of the two-sided nature of quaternion rotations, the
  /// sign of a rotor may be undetermined in many cases.
  /// Discontinuous flips in that sign for rotor-valued functions of
  /// time can cause significant problems.  This function removes
  /// those flips by ensuring that the output rotors at successive
  /// instants are within 'discont' of each other.
  vector<Quaternion> Q(R.size());
  Q[0] = R[0];
  for(unsigned int i=1; i<R.size(); ++i) {
    if((R[i]-Q[i-1]).abs() > discont) {
      Q[i] = -R[i];
    } else {
      Q[i] = R[i];
    }
  }
  return Q;
}

/// Difference between frame rotors R1*R2.inverse()
std::vector<Quaternion> Quaternions::RDelta(const std::vector<Quaternion>& R1, const std::vector<Quaternion>& R2, const int IndexOfFiducialTime) {
  ///
  /// \param R1 Vector of rotors
  /// \param R2 Vector of rotors
  /// \param IndexOfFiducialTime Integer index of time at which
  ///        difference is set to zero [default: -1]
  ///
  /// If the optional `IndexOfFiducialTime` is given, the returned
  /// quantity is `R1*Offset*R2.inverse()`, where `Offset` is
  /// `R1[I].inverse()*R2[I]`, thus ensuring that `RDelta[I]`=1.
  if(R1.size() != R2.size()) {
    INFOTOCERR << " R1.size()=" << R1.size() << " != R2.size()=" << R2.size() << endl;
    throw(VectorSizeMismatch);
  }
  if(R1.size()==0) {
    return vector<Quaternion>(0);
  }
  if(int(R1.size())<=IndexOfFiducialTime) {
    INFOTOCERR << " R1.size()=" << R1.size() << " <= IndexOfFiducialTime=" << IndexOfFiducialTime << endl;
    throw(IndexOutOfBounds);
  }
  const unsigned int Size=R1.size();
  vector<Quaternion> Rd(Size);
  if(IndexOfFiducialTime<0) {
    for(unsigned int i=0; i<Size; ++i) {
      Rd[i] = R1[i] * R2[i].inverse();
    }
    return Rd;
  }
  const Quaternion Offset = R1[IndexOfFiducialTime].inverse() * R2[IndexOfFiducialTime];
  for(unsigned int i=0; i<Size; ++i) {
    Rd[i] = R1[i] * Offset * R2[i].inverse();
  }
  return Rd;
}

/// Squad interpolation of Quaternion time series.
std::vector<Quaternion> Quaternions::Squad(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn, const std::vector<double>& tOut) {
  ///
  /// \param RIn Vector of rotors
  /// \param tIn Vector of corresponding times
  /// \param tOut Vector of times to which RIn will be interpolated
  ///
  /// This function implements a version of cubic-spline interpolation
  /// designed for unit quaternions, which delivers more accurate,
  /// smooth, and physical rotations than other forms of
  /// interpolation.
  ///
  /// Note that input rotors are assumed to have magnitude 1.  It is
  /// possible that things will go very badly if this is not true.
  ///
  /// The output rotors are "unflipped", meaning that they will be
  /// roughly continuous.
  if(RIn.size() != tIn.size()) {
    INFOTOCERR << " RIn.size()=" << RIn.size() << " != tIn.size()=" << tIn.size() << endl;
    throw(VectorSizeMismatch);
  }
  if(tOut[0]<tIn[0] || tOut.back()>tIn.back()) {
    INFOTOCERR << " (tOut[0]=" << tOut[0] << ") < (tIn[0]=" << tIn[0]
               << ") || (tOut.back()=" << tOut.back() << ") > (tIn.back()=" << tIn.back() << ")" << endl;
    throw(CannotExtrapolateQuaternions);
  }
  vector<Quaternion> ROut(tOut.size());
  unsigned int iIn = 0;
  unsigned int iOut = 0;
  while(iOut<tOut.size() && iIn<tIn.size() && tIn[tIn.size()-1]>=tOut[iOut]) {
    double Dtim1, Dti, Dtip1;
    Quaternion Qim1, Qi, Qip1, Qip2, Ai, Bip1;
    while(iIn+1<tIn.size() && tIn[iIn+1]<tOut[iOut]) {
      iIn += 1;
    }
    if(iIn+1==tIn.size()) {
      INFOTOCERR << " Time " << tOut[iOut] << " is beyond the end of the input data (time " << tIn.back() << ")." << endl;
      throw(CannotExtrapolateQuaternions);
    }
    if(iIn==0) {
      Dti = tIn[iIn+1]-tIn[iIn];
      Dtim1 = Dti;
      Dtip1 = tIn[iIn+2]-tIn[iIn+1];
      Qim1 = RIn[iIn]*RIn[iIn+1].inverse()*RIn[iIn];
      Qi = RIn[iIn];
      Qip1 = RIn[iIn+1];
      Qip2 = RIn[iIn+2];
    } else if(iIn+2>=tIn.size()) {
      const unsigned int i=tIn.size()-2;
      Dtim1 = tIn[i]-tIn[i-1];
      Dti = tIn[i+1]-tIn[i];
      Dtip1 = Dti;
      Qim1 = RIn[i-1];
      Qi = RIn[i];
      Qip1 = RIn[i+1];
      Qip2 = RIn[i+1]*RIn[i].inverse()*RIn[i+1];
    } else {
      Dtim1 = tIn[iIn]-tIn[iIn-1];
      Dti = tIn[iIn+1]-tIn[iIn];
      Dtip1 = tIn[iIn+2]-tIn[iIn+1];
      Qim1 = RIn[iIn-1];
      Qi = RIn[iIn];
      Qip1 = RIn[iIn+1];
      Qip2 = RIn[iIn+2];
    }
    Ai = Qi * Quaternions::exp((
                                (Qi.inverse()*Qip1).logRotor()
                                +(Dti/Dtim1)*(Qim1.inverse()*Qi).logRotor()
                                -2*(Qi.inverse()*Qip1).logRotor()
                                )*0.25);
    Bip1 = Qip1 * Quaternions::exp((
                                    (Dti/Dtip1)*(Qip1.inverse()*Qip2).logRotor()
                                    +(Qi.inverse()*Qip1).logRotor()
                                    -2*(Qi.inverse()*Qip1).logRotor()
                                    )*-0.25);
    while(iOut<tOut.size() && tOut[iOut]<=tIn[iIn+1]) {
      const double taui = (tOut[iOut]-tIn[iIn]) / Dti;
      ROut[iOut] = Quaternions::Slerp(2*taui*(1-taui),
                                      Quaternions::Slerp(taui, Qi, Qip1),
                                      Quaternions::Slerp(taui, Ai, Bip1));
      iOut += 1;
    }
    iIn += 1;
  }
  return Quaternions::UnflipRotors(ROut);
}

std::vector<Quaternion> Quaternions::FrameAngularVelocity(const std::vector<Quaternion>& f, const std::vector<double>& t) {
  return 2.0*Quaternions::QuaternionDerivative(f, t)*Quaternions::conjugate(f);
}

/// "Mean" rotor, minimizing "distance" between R(t) and R_mean
Quaternion Quaternions::ApproximateMeanRotor(const std::vector<Quaternion>& R) {
  /// \param R Vector of rotors to be averaged.
  ///
  /// The true mean of a rotor function of time can only be calculated
  /// by an optimization algorithm because the metric is too
  /// complicated to solve analytically.  Instead, this algorithm uses
  /// a simple approximation that is true in the limit of `R(t)` being
  /// constant.  In particular, the ideal mean would be calculated
  /// using the `angle` between `R(t)` and `R_mean`; this function
  /// minimizes the squared `sin` of half that angle.  Another way of
  /// thinking of this is that this function isn't approximate; it
  /// just uses a different metric, which is approximately equal to
  /// what we would like to use.
  ///
  /// This algorithm calculates the averaging formula found in "Means
  /// and averaging in the group of rotations"
  /// <http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.16.5040>.
  Quaternion Mean(0.,0.,0.,0.);
  const unsigned int j=R.size();
  for(unsigned int i=0; i<j; ++i) {
    Mean += R[i];
  }
  return Mean.normalized();
}

/// "Mean" rotor, approximately minimizing integrated "distance" between R(t) and R_mean
Quaternion Quaternions::ApproximateMeanRotor(const std::vector<Quaternion>& R, const std::vector<double>& t,
                                             const double t1, const double t2) {
  /// \param R Vector of rotors to be averaged.
  /// \param t Vector of corresponding time steps.
  /// \param t1 Optional early bound of time to average over
  /// \param t2 Optional late bound of time to average over
  ///
  /// The true mean of a rotor function of time can only be calculated
  /// by an optimization algorithm because the metric is too
  /// complicated to solve analytically.  Instead, this algorithm uses
  /// a simple approximation that is true in the limit of `R(t)` being
  /// constant.  In particular, the ideal mean would be calculated
  /// using the `angle` between `R(t)` and `R_mean`; this function
  /// minimizes the squared `sin` of half that angle.  Another way of
  /// thinking of this is that this function isn't approximate; it
  /// just uses a different metric, which is approximately equal to
  /// what we would like to use.
  ///
  /// This algorithm calculates the continuous limit of the averaging
  /// formula found in "Means and averaging in the group of rotations"
  /// <http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.16.5040>.
  if(R.size() != t.size()) {
    INFOTOCERR << " R.size()=" << R.size() << " != t.size()=" << t.size() << std::endl;
    throw(VectorSizeMismatch);
  }
  Quaternion Mean(0.,0.,0.,0.);
  const unsigned int i1 = hunt(t, t1, 0);
  const unsigned int i2 = hunt(t, t2, t.size()-1);
  for(unsigned int i=i1+1; i<=i2; ++i) {
    Mean += (t[i]-t[i-1])*(R[i]+R[i-1])/2.0;
  }
  return Mean.normalized();
}

/// Approximate Rdelta such that Rdelta*Rb is as nearly equal to Ra as possible
Quaternion Quaternions::ApproximateOptimalAlignmentRotor(const std::vector<Quaternion>& Ra, const std::vector<Quaternion>& Rb,
                                                         const std::vector<double>& t, const double t1, const double t2) {
  /// \param Ra Vector of fixed rotors.
  /// \param Rb Vector of rotors to be aligned to Ra.
  /// \param t Vector of corresponding time steps.
  /// \param t1 Optional early bound of time to average over
  /// \param t2 Optional late bound of time to average over
  ///
  /// This routine essentially just returns the `MeanRotor` of the
  /// difference between `Ra` and `Rb`.  The result is an `Rdelta`
  /// which can be applied to `Rb`, by multiplication on the left, to
  /// give a new time-series for `Rb` that is approximately as close
  /// to `Ra` as possible.
  if(Ra.size() != t.size() || Rb.size() != t.size()) {
    INFOTOCERR
      << "\nRa.size()=" << Ra.size() << " != t.size()=" << t.size() << " or "
      <<   "Rb.size()=" << Rb.size() << " != t.size()=" << t.size() << std::endl;
    throw(VectorSizeMismatch);
  }
  return ApproximateMeanRotor(RDelta(Ra, Rb), t, t1, t2);
}

#ifdef USE_GSL
#include <gsl/gsl_errno.h>
#include <gsl/gsl_min.h>
#include <gsl/gsl_multimin.h>

void myGSLErrorHandler (const char * reason, const char * file, int line, int gsl_errno) {
  std::cerr << "\n\n" << file << ":" << line << ": " << reason << std::endl;
  throw(gsl_errno);
}
gsl_error_handler_t* defaultGSLErrorHandler = gsl_set_error_handler((gsl_error_handler_t*) &myGSLErrorHandler);

#ifndef DOXYGEN
// This is a local object used by various alignment routines below
class RotorAligner {
private:
  std::vector<Quaternions::Quaternion> Ra;
  std::vector<double> ta;
  const std::vector<Quaternions::Quaternion>& Rb;
  const std::vector<double>& tb;
public:
  RotorAligner(const std::vector<Quaternions::Quaternion>& RA)
    : Ra(RA), ta(0), Rb(Ra), tb(ta) { } // In this case, Rb and tb
                                        // should never be used, so
                                        // just fill them with things
                                        // acceptable to the compiler
  RotorAligner(double t1, double t2,
               const std::vector<Quaternions::Quaternion>& RA, const std::vector<double>& tA,
               const std::vector<Quaternions::Quaternion>& RB, const std::vector<double>& tB)
    : Ra(RA), ta(tA), Rb(RB), tb(tB)
  {
    // Check to make sure we have sufficient times before any offset.
    // (This is necessary but not sufficient for the method to work.)
    if(t1<ta[0]) {
      INFOTOCERR << " Alignment time t1=" << t1
                 << " does not occur in ta (which has ta[0]=" << ta[0] << ")." << std::endl;
      throw(IndexOutOfBounds);
    }
    if(t2>ta.back()) {
      INFOTOCERR << " Alignment time t2=" << t2
                 << " does not occur in ta (which has ta.back()=" << ta.back() << ")." << std::endl;
      throw(IndexOutOfBounds);
    }
    if(tb.size()>0) {
      if(t1<tb[0]) {
        INFOTOCERR << " Alignment time t1=" << t1
                   << " does not occur in tb (which has tb[0]=" << tb[0] << ")." << std::endl;
        throw(IndexOutOfBounds);
      }
      if(t2>tb.back()) {
        INFOTOCERR << " Alignment time t1=" << t1
                  << " does not occur in tb (which has tb.back()=" << tb.back() << ")." << std::endl;
        throw(IndexOutOfBounds);
      }
    }
    // Trim the fixed frame (Ra) and its set of times, to which we
    // will interpolate.
    unsigned int i=ta.size()-1;
    while(ta[i]>t2 && i>0) { --i; }
    ta.erase(ta.begin()+i, ta.end());
    Ra.erase(Ra.begin()+i, Ra.end());
    i=0;
    while(i<ta.size() && ta[i]<t1) { ++i; }
    ta.erase(ta.begin(), ta.begin()+i);
    Ra.erase(Ra.begin(), Ra.begin()+i);
  }

  Quaternions::Quaternion ApproximateOptimalR_delta(const double deltat) const {
    using namespace Quaternions; // Allow me to subtract a double from a vector<double> below
    const std::vector<Quaternions::Quaternion> Rb1 = Quaternions::Squad(Rb, tb+deltat, ta);
    return ApproximateOptimalAlignmentRotor(Ra, Rb1, ta);
  }

  double EvaluateMinimizationQuantity(const double deltat) const {
    using namespace Quaternions; // Allow me to subtract a double from a vector<double> below
    const std::vector<Quaternions::Quaternion> Rb1 = Quaternions::Squad(Rb, tb+deltat, ta);
    const Quaternion R_delta = ApproximateOptimalAlignmentRotor(Ra, Rb1, ta);
    const std::vector<Quaternion> Rb2 = R_delta * Rb1;
    const unsigned int Size=Rb2.size();
    double f = 0.0;
    double fdot_last = 4 * Quaternions::normsquared( ( Ra[0] * Quaternions::inverse(Rb2[0]) ).logRotor() );
    for(unsigned int i=1; i<Size; ++i) {
      const double fdot = 4 * Quaternions::normsquared( ( Ra[i] * Quaternions::inverse(Rb2[i]) ).logRotor() );
      f += (ta[i]-ta[i-1])*(fdot+fdot_last)/2.0;
      fdot_last = fdot;
    }
    return f;
  }

  double EvaluateMinimizationQuantity(const double deltax, const double deltay, const double deltaz) const {
    const Quaternions::Quaternion R_mean = Quaternions::exp(Quaternions::Quaternion(0, deltax, deltay, deltaz));
    const std::vector<Quaternions::Quaternion> Raprime = Ra * R_mean.inverse();
    const unsigned int Size=Raprime.size();
    double f = 0.0;
    double fdot_last = 4 * Quaternions::normsquared( Quaternions::logRotor( Raprime[0] ) );
    for(unsigned int i=1; i<Size; ++i) {
      const double fdot = 4 * Quaternions::normsquared( Quaternions::logRotor( Raprime[i] ) );
      f += (ta[i]-ta[i-1])*(fdot+fdot_last)/2.0;
      fdot_last = fdot;
    }
    return f;
  }

  double EvaluateMinimizationQuantityWithoutTime(const double deltax, const double deltay, const double deltaz) const {
    const Quaternions::Quaternion R_mean = Quaternions::exp(Quaternions::Quaternion(0, deltax, deltay, deltaz));
    const std::vector<Quaternions::Quaternion> Raprime = Ra * R_mean.inverse();
    const unsigned int Size=Raprime.size();
    double f = 0.0;
    for(unsigned int i=0; i<Size; ++i) {
      f += 4 * Quaternions::normsquared( Quaternions::logRotor( Raprime[i] ) );
    }
    return f;
  }

  double EvaluateMinimizationQuantity(const double deltat, const double deltax, const double deltay, const double deltaz) const {
    using namespace Quaternions; // Allow me to subtract a double from a vector<double> below
    const Quaternions::Quaternion R_delta = Quaternions::exp(Quaternions::Quaternion(0, deltax, deltay, deltaz));
    const std::vector<Quaternions::Quaternion> Rbprime = Quaternions::Squad(R_delta * Rb, tb+deltat, ta);
    const unsigned int Size=Rbprime.size();
    double f = 0.0;
    double fdot_last = 4 * Quaternions::normsquared( Quaternions::logRotor( Ra[0] * Quaternions::inverse(Rbprime[0]) ) );
    for(unsigned int i=1; i<Size; ++i) {
      const double fdot = 4 * Quaternions::normsquared( Quaternions::logRotor( Ra[i] * Quaternions::inverse(Rbprime[i]) ) );
      f += (ta[i]-ta[i-1])*(fdot+fdot_last)/2.0;
      fdot_last = fdot;
    }
    return f;
  }
};
double minfunc1d (const double deltat, void* params) {
  RotorAligner* Aligner = (RotorAligner*) params;
  return Aligner->EvaluateMinimizationQuantity(deltat);
}
double minfunc3d (const gsl_vector* delta, void* params) {
  RotorAligner* Aligner = (RotorAligner*) params;
  return Aligner->EvaluateMinimizationQuantity(gsl_vector_get(delta,0),
                                               gsl_vector_get(delta,1),
                                               gsl_vector_get(delta,2));
}
double minfunc3d_NoTime (const gsl_vector* delta, void* params) {
  RotorAligner* Aligner = (RotorAligner*) params;
  return Aligner->EvaluateMinimizationQuantityWithoutTime(gsl_vector_get(delta,0),
                                                          gsl_vector_get(delta,1),
                                                          gsl_vector_get(delta,2));
}
double minfunc4d (const gsl_vector* delta, void* params) {
  RotorAligner* Aligner = (RotorAligner*) params;
  return Aligner->EvaluateMinimizationQuantity(gsl_vector_get(delta,0),
                                               gsl_vector_get(delta,1),
                                               gsl_vector_get(delta,2),
                                               gsl_vector_get(delta,3));
}
#endif // DOXYGEN

/// Time and rotor offsets such that Rdelta*Rb(t+deltat) is as close to Ra(t) as possible
void Quaternions::ApproximateOptimalAlignment(const double t1, const double t2,
                                              const std::vector<Quaternion>& Ra, const std::vector<double>& ta,
                                              const std::vector<Quaternion>& Rb, const std::vector<double>& tb,
                                              double& deltat, Quaternions::Quaternion& R_delta) {
  /// \param t1 Early bound of time to average over
  /// \param t2 Late bound of time to average over
  /// \param Ra Vector of fixed rotors.
  /// \param ta Vector of corresponding time steps.
  /// \param Rb Vector of rotors to be aligned to Ra.
  /// \param tb Vector of corresponding time steps.
  /// \param deltat Output optimal time offset
  /// \param R_delta Output optimal rotor
  ///
  /// This routine actively optimizes over the time offset of `Rb` and
  /// approximately optimizes over the rotational degrees of freedom
  /// in `Rb` such that Rdelta*Rb(t+deltat) is as close to Ra(t) as
  /// possible.  The rotational optimization for any given time offset
  /// is performed analytically by the routine
  /// `ApproximateOptimalAlignmentRotor`.
  ///
  /// The times `t1` and `t2` are measured relative to `ta`, and all
  /// are left fixed; `Rb` and `tb` are shifted to achieve alignment.

  int status=GSL_CONTINUE;
  int iter = 0, max_iter = 100;
  const double epsabs=1.e-8, epsrel=1.e-9;
  const gsl_min_fminimizer_type *T;
  gsl_min_fminimizer *s;

  const RotorAligner Aligner(t1, t2, Ra, ta, Rb, tb);

  gsl_function F;
  F.function = &minfunc1d;
  F.params = (void*) &Aligner;

  // Initial guess for the location of the minimum
  double m = 0.0;

  // Set the bounds to ensure that ta[0]+deltat<t1 and
  // ta[-1]+deltat>t2, which mean t2-ta[-1] < deltat < t1-ta[0].
  // Also, don't search more than (t2-t1)/2. to either left or right.
  double a = std::max(t2-ta.back(), (t1-t2)/2.);
  double b = std::min(t1-ta[0], (t2-t1)/2.);
  // std::cerr << __FILE__ << ":" << __LINE__ << ": a=" << a << " b=" << b << " m=" << m << std::endl;
  // std::cerr << __FILE__ << ":" << __LINE__ << ": F(a)=" << minfunc1d(a,F.params) << " F(b)=" << minfunc1d(b,F.params) << " F(m)=" << minfunc1d(m,F.params) << std::endl;

  // To avoid any local minima, let's just start off by minimizing
  // roughly in the dumbest possible way.  First, evaluate at a bunch
  // of points all across the initial range.  Then, reset (a,m,b) to
  // be our best guess, with the best value found on the mesh, and a
  // little range nearby.  Next, narrow that range further by simply
  // repeating the above
  for(unsigned int iterations=0; iterations<2; ++iterations) {
    const unsigned int MeshSize = 1000;
    const double Dt = (b-a)/(MeshSize-1);
    double BestF = minfunc1d(m,F.params);
    for(unsigned int i=0; i<MeshSize; ++i) {
      const double t = a + i*Dt;
      const double ThisF = minfunc1d(t,F.params);
      if(ThisF < BestF) {
        m = t;
        BestF = ThisF;
      }
    }
    a = m-5*Dt;
    b = m+5*Dt;
    // std::cerr << __FILE__ << ":" << __LINE__ << ": a=" << a << " b=" << b << " m=" << m << std::endl;
    // std::cerr << __FILE__ << ":" << __LINE__ << ": F(a)=" << minfunc1d(a,F.params) << " F(b)=" << minfunc1d(b,F.params) << " F(m)=" << minfunc1d(m,F.params) <<std::endl;
  }

  T = gsl_min_fminimizer_brent;
  s = gsl_min_fminimizer_alloc(T);
  try {
    gsl_min_fminimizer_set(s, &F, m, a, b);
  } catch(int i) {
    INFOTOCERR << " Error code=`" << i
              << "`.\n    That's probably `" << GSL_EINVAL << "` because of an invalid initial guess for the minimizer;"
              <<          " F(deltat=" << m << ")=" << minfunc1d(m,F.params) << " is"
              <<   "\n    a worse value than one or both of the endpoints [F(deltat=" << a << ")=" << minfunc1d(a,F.params)
              <<          "; F(deltat=" << b << ")=" << minfunc1d(b,F.params) << "]."
              <<   "\n    Try a better rough initial alignment, or increasing the distance between t1 and t2." << std::endl;
    throw(FailedGSLCall);
  }
  while (status == GSL_CONTINUE && iter < max_iter) {
    iter++;
    status = gsl_min_fminimizer_iterate(s);

    if(status==GSL_EBADFUNC) {
      INFOTOCERR
        << "\nThe iteration encountered a singular point where the function evaluated to Inf or NaN"
        << "\nwhile minimizing with (" << a << "," << m << "," << b << ")." << std::endl;
    }

    if(status==GSL_FAILURE) {
      INFOTOCERR << "\nThe algorithm could not improve the current best approximation or bounding interval." << std::endl;
    }

    m = gsl_min_fminimizer_x_minimum(s);
    a = gsl_min_fminimizer_x_lower(s);
    b = gsl_min_fminimizer_x_upper(s);

    status = gsl_min_test_interval(a, b, epsabs, epsrel);

    // if (status == GSL_SUCCESS) printf ("Converged:\n");
    // printf ("%5d [%.7f, %.7f] %.7f %.7f\n", iter, a, b, m, b - a);
  }

  if(iter==max_iter) {
    INFOTOCERR
      << "\nWarning: Minimization ended because it went through " << max_iter << " iterations."
      << "\n         This may indicate failure.  You may want to try with a better initial guess." << std::endl;
  }

  gsl_min_fminimizer_free(s);

  deltat = m;
  R_delta = Aligner.ApproximateOptimalR_delta(deltat);

  return;
}

/// Time and rotor offsets such that Rdelta*Rb(t+deltat) is as close to Ra(t) as possible
void Quaternions::OptimalAlignment(const double t1, const double t2,
                                   const std::vector<Quaternion>& Ra, const std::vector<double>& ta,
                                   const std::vector<Quaternion>& Rb, const std::vector<double>& tb,
                                   double& deltat, Quaternions::Quaternion& R_delta) {
  /// \param t1 Initial bound of alignment range
  /// \param t2 Final bound of alignment range
  /// \param Ra Vector of fixed rotors.
  /// \param ta Vector of corresponding time steps.
  /// \param Rb Vector of rotors to be aligned to Ra.
  /// \param tb Vector of corresponding time steps.
  /// \param deltat Output optimal time offset
  /// \param R_delta Output optimal rotor
  ///
  /// This routine actively optimizes the time offset and all three
  /// rotational degrees of freedom for `Rb`, so that
  /// `Rdelta*Rb(t+deltat)` is as close to `Ra(t)` as possible over
  /// the range (t1, t2).  The error measure is just the usual
  /// `angle(UnflipRotors(RDelta(Ra,Rb)))` integrated over `(t1,t2)`.
  ///
  /// The times `t1` and `t2` are measured relative to `ta`, and all
  /// are left fixed; `Rb` and `tb` are shifted to achieve alignment.

  // Start with initial guess from ApproximateOptimalAlignment
  ApproximateOptimalAlignment(t1, t2, Ra, ta, Rb, tb, deltat, R_delta);

  const RotorAligner Aligner(t1, t2, Ra, ta, Rb, tb);
  const unsigned int NDimensions = 4;
  const unsigned int MaxIterations = 2000;
  const double MinSimplexSize = 1.0e-10;

  // Don't try to offset by more than (t2-t1), or (t1-ta[0]), or
  // (ta.back()-t2).  The minimization might try to take twice this
  // step, so let's cut that range down further -- by a factor 2.5;
  const double InitialTrialTimeStep = std::min(t2-t1, std::min(t1-ta[0], ta.back()-t2))/2.5;
  const double InitialTrialAngleStep = 1.0;

  // Use Nelder-Mead simplex minimization
  const gsl_multimin_fminimizer_type* T =
    gsl_multimin_fminimizer_nmsimplex2;
  gsl_multimin_fminimizer* s = NULL;
  gsl_vector* ss;
  gsl_vector* x;
  gsl_multimin_function min_func;
  size_t iter = 0;
  int status = GSL_CONTINUE;
  double size = 0.0;

  // Set initial values
  {
    // std::cerr << __FILE__ << ":" << __LINE__ << ": R_delta=" << R_delta << "; R_delta.logRotor()=" << R_delta.logRotor() << std::endl;
    const Quaternions::Quaternion R_delta_log = R_delta.logRotor();
    x = gsl_vector_alloc(NDimensions);
    gsl_vector_set(x, 0, deltat);
    gsl_vector_set(x, 1, R_delta_log[1]);
    gsl_vector_set(x, 2, R_delta_log[2]);
    gsl_vector_set(x, 3, R_delta_log[3]);
  }

  // Set initial step sizes
  ss = gsl_vector_alloc(NDimensions);
  gsl_vector_set(ss, 0, InitialTrialTimeStep);
  gsl_vector_set(ss, 1, InitialTrialAngleStep);
  gsl_vector_set(ss, 2, InitialTrialAngleStep);
  gsl_vector_set(ss, 3, InitialTrialAngleStep);

  min_func.n = NDimensions;
  min_func.f = &minfunc4d;
  min_func.params = (void*) &Aligner;

  s = gsl_multimin_fminimizer_alloc(T, NDimensions);
  gsl_multimin_fminimizer_set(s, &min_func, x, ss);

  // Run the minimization
  while(status == GSL_CONTINUE && iter < MaxIterations) {
    iter++;
    status = gsl_multimin_fminimizer_iterate(s);

    if(status==GSL_EBADFUNC) {
      INFOTOCERR
                << ":\nThe iteration encountered a singular point where the function evaluated to Inf or NaN"
                << "\nwhile minimizing at (" << gsl_vector_get(s->x, 0) << ", " << gsl_vector_get(s->x, 1)
                << ", " << gsl_vector_get(s->x, 2) << ", " << gsl_vector_get(s->x, 3) << ")." << std::endl;
    }

    if(status==GSL_FAILURE) {
      INFOTOCERR
                << ":\nThe algorithm could not improve the current best approximation or bounding interval." << std::endl;
    }

    if(status==GSL_ENOPROG) {
      INFOTOCERR
                << ":\nThe minimizer is unable to improve on its current estimate, either due to"
                << "\nnumerical difficulty or because a genuine local minimum has been reached." << std::endl;
    }

    if(status) break;
    size = gsl_multimin_fminimizer_size(s);
    status = gsl_multimin_test_size(size, MinSimplexSize);

    // std::cerr << iter << ": " << std::setprecision(15);
    // for(unsigned int k=0; k<NDimensions; ++k)
    //   std::cerr << gsl_vector_get(s->x, k) << " ";
    // std::cerr << std::endl;
  }

  if(iter==MaxIterations) {
      INFOTOCERR
                << "\nWarning: Minimization ended because it went through " << MaxIterations << " iterations."
                << "\n         This may indicate failure.  You may want to try with a better initial guess." << std::endl;
  }

  // Get time shift and rotation
  deltat = gsl_vector_get(s->x, 0);
  R_delta = Quaternions::exp(Quaternions::Quaternion(0.0, gsl_vector_get(s->x, 1), gsl_vector_get(s->x, 2), gsl_vector_get(s->x, 3)));

  // Free allocated memory
  gsl_vector_free(x);
  gsl_vector_free(ss);
  gsl_multimin_fminimizer_free(s);

  return;
}

/// Mean rotor, minimizing integrated distance between R(t_i) and R_mean
Quaternion Quaternions::MeanRotor(const std::vector<Quaternion>& R) {
  /// \param R Vector of rotors to be averaged.
  ///
  /// This algorithm minimizes the integrated distance between the
  /// input data's `R(t)` and `R_mean` by varying `R_mean` and finding
  /// the minimum using the Nedler-Mead simplex algorithm(2) of GSL.
  /// This is the correct generalization of the similar
  /// `ApproximateMeanRotor` function, and is essentially described in
  /// my paper <http://arxiv.org/abs/1302.2919/>.

  // Start with initial guess from ApproximateOptimalAlignment
  Quaternions::Quaternion R_mean = ApproximateMeanRotor(R);

  const unsigned int MaxIterations = 2000;
  const double MinSimplexSize = 1.0e-8;
  const double InitialTrialAngleStep = 1.0;

  const RotorAligner Aligner(R);
  const unsigned int NDimensions = 3;

  // Use Nelder-Mead simplex minimization
  const gsl_multimin_fminimizer_type* T =
    gsl_multimin_fminimizer_nmsimplex2;
  gsl_multimin_fminimizer* s = NULL;
  gsl_vector* ss;
  gsl_vector* x;
  gsl_multimin_function min_func;
  size_t iter = 0;
  int status = GSL_CONTINUE;
  double size = 0.0;

  // Set initial values
  {
    const Quaternions::Quaternion R_mean_log = R_mean.logRotor();
    x = gsl_vector_alloc(NDimensions);
    gsl_vector_set(x, 0, R_mean_log[1]);
    gsl_vector_set(x, 1, R_mean_log[2]);
    gsl_vector_set(x, 2, R_mean_log[3]);
  }

  // Set initial step sizes
  ss = gsl_vector_alloc(NDimensions);
  gsl_vector_set(ss, 0, InitialTrialAngleStep);
  gsl_vector_set(ss, 1, InitialTrialAngleStep);
  gsl_vector_set(ss, 2, InitialTrialAngleStep);

  min_func.n = NDimensions;
  min_func.f = &minfunc3d_NoTime;
  min_func.params = (void*) &Aligner;

  s = gsl_multimin_fminimizer_alloc(T, NDimensions);
  gsl_multimin_fminimizer_set(s, &min_func, x, ss);

  // Run the minimization
  while(status == GSL_CONTINUE && iter < MaxIterations) {
    iter++;
    status = gsl_multimin_fminimizer_iterate(s);

    // size = gsl_multimin_fminimizer_size(s);
    // std::cerr << iter << ": " << status << " " << std::setprecision(15) << size << " ";
    // for(unsigned int k=0; k<NDimensions; ++k)
    //   std::cerr << gsl_vector_get(s->x, k) << " ";
    // std::cerr << std::endl;

    if(status==GSL_EBADFUNC) {
      INFOTOCERR
                << ":\nThe iteration encountered a singular point where the function evaluated to Inf or NaN"
                << "\nwhile minimizing at (" << gsl_vector_get(s->x, 0) << ", " << gsl_vector_get(s->x, 1)
                << ", " << gsl_vector_get(s->x, 2) << ")." << std::endl;
    } else if(status==GSL_FAILURE) {
      INFOTOCERR
                << ":\nThe algorithm could not improve the current best approximation or bounding interval." << std::endl;
    } else if(status==GSL_ENOPROG) {
      INFOTOCERR
                << ":\nThe minimizer is unable to improve on its current estimate, either due to"
                << "\nnumerical difficulty or because a genuine local minimum has been reached." << std::endl;
    } else if(status) {
      INFOTOCERR
                << ":\nUnkown minimization status=" << status << std::endl;
      break;
    }

    size = gsl_multimin_fminimizer_size(s);
    status = gsl_multimin_test_size(size, MinSimplexSize);
    // if(status!=GSL_CONTINUE) {
    //   INFOTOCERR
    //             << ":\nsize=" << size << "\tMinSimplexSize=" << MinSimplexSize << "\tstatus=" << status << std::endl;
    // }
  }

  if(iter==MaxIterations) {
      INFOTOCERR
                << "\nWarning: Minimization ended because it went through " << MaxIterations << " iterations."
                << "\n         This may indicate failure.  You may want to try with a better initial guess." << std::endl;
  }

  // Get time shift and rotation
  R_mean = Quaternions::exp(Quaternions::Quaternion(0.0, gsl_vector_get(s->x, 0), gsl_vector_get(s->x, 1), gsl_vector_get(s->x, 2)));

  // Free allocated memory
  gsl_vector_free(x);
  gsl_vector_free(ss);
  gsl_multimin_fminimizer_free(s);

  return R_mean;
}

/// Mean rotor, minimizing integrated distance between R(t) and R_mean
Quaternion Quaternions::MeanRotor(const std::vector<Quaternion>& R, const std::vector<double>& t,
                                  double t1, double t2) {
  /// \param R Vector of rotors to be averaged.
  /// \param t Vector of corresponding time steps.
  /// \param t1 Optional early bound of time to average over
  /// \param t2 Optional late bound of time to average over
  ///
  /// This algorithm minimizes the integrated distance between the
  /// input data's `R(t)` and `R_mean` by varying `R_mean` and finding
  /// the minimum using the Nedler-Mead simplex algorithm(2) of GSL.
  /// This is the correct generalization of the similar
  /// `ApproximateMeanRotor` function, and is essentially described in
  /// my paper <http://arxiv.org/abs/1302.2919/>.

  // Make sure we have actual bounds for the time
  if(t1==-1e300) {
    t1 = t[0];
  }
  if(t2==1e300) {
    t2 = t.back();
  }

  // Start with initial guess from ApproximateOptimalAlignment
  Quaternions::Quaternion R_mean = ApproximateMeanRotor(R, t, t1, t2);

  const unsigned int MaxIterations = 2000;
  const double MinSimplexSize = 1.0e-8;
  const double InitialTrialAngleStep = 1.0;

  const std::vector<Quaternions::Quaternion> Rb;
  const std::vector<double> tb(0);
  const RotorAligner Aligner(t1, t2, R, t, Rb, tb);
  const unsigned int NDimensions = 3;

  // Use Nelder-Mead simplex minimization
  const gsl_multimin_fminimizer_type* T =
    gsl_multimin_fminimizer_nmsimplex2;
  gsl_multimin_fminimizer* s = NULL;
  gsl_vector* ss;
  gsl_vector* x;
  gsl_multimin_function min_func;
  size_t iter = 0;
  int status = GSL_CONTINUE;
  double size = 0.0;

  // Set initial values
  {
    const Quaternions::Quaternion R_mean_log = R_mean.logRotor();
    x = gsl_vector_alloc(NDimensions);
    gsl_vector_set(x, 0, R_mean_log[1]);
    gsl_vector_set(x, 1, R_mean_log[2]);
    gsl_vector_set(x, 2, R_mean_log[3]);
  }

  // Set initial step sizes
  ss = gsl_vector_alloc(NDimensions);
  gsl_vector_set(ss, 0, InitialTrialAngleStep);
  gsl_vector_set(ss, 1, InitialTrialAngleStep);
  gsl_vector_set(ss, 2, InitialTrialAngleStep);

  min_func.n = NDimensions;
  min_func.f = &minfunc3d;
  min_func.params = (void*) &Aligner;

  s = gsl_multimin_fminimizer_alloc(T, NDimensions);
  gsl_multimin_fminimizer_set(s, &min_func, x, ss);

  // Run the minimization
  while(status == GSL_CONTINUE && iter < MaxIterations) {
    iter++;
    status = gsl_multimin_fminimizer_iterate(s);

    // size = gsl_multimin_fminimizer_size(s);
    // std::cerr << iter << ": " << status << " " << std::setprecision(15) << size << " ";
    // for(unsigned int k=0; k<NDimensions; ++k)
    //   std::cerr << gsl_vector_get(s->x, k) << " ";
    // std::cerr << std::endl;

    if(status==GSL_EBADFUNC) {
      INFOTOCERR
                << ":\nThe iteration encountered a singular point where the function evaluated to Inf or NaN"
                << "\nwhile minimizing at (" << gsl_vector_get(s->x, 0) << ", " << gsl_vector_get(s->x, 1)
                << ", " << gsl_vector_get(s->x, 2) << ")." << std::endl;
    } else if(status==GSL_FAILURE) {
      INFOTOCERR
                << ":\nThe algorithm could not improve the current best approximation or bounding interval." << std::endl;
    } else if(status==GSL_ENOPROG) {
      INFOTOCERR
                << ":\nThe minimizer is unable to improve on its current estimate, either due to"
                << "\nnumerical difficulty or because a genuine local minimum has been reached." << std::endl;
    } else if(status) {
      INFOTOCERR
                << ":\nUnkown minimization status=" << status << std::endl;
      break;
    }

    size = gsl_multimin_fminimizer_size(s);
    status = gsl_multimin_test_size(size, MinSimplexSize);
    // if(status!=GSL_CONTINUE) {
    //   INFOTOCERR
    //             << ":\nsize=" << size << "\tMinSimplexSize=" << MinSimplexSize << "\tstatus=" << status << std::endl;
    // }
  }

  if(iter==MaxIterations) {
      INFOTOCERR
                << "\nWarning: Minimization ended because it went through " << MaxIterations << " iterations." << std::endl;
  }

  // Get time shift and rotation
  R_mean = Quaternions::exp(Quaternions::Quaternion(0.0, gsl_vector_get(s->x, 0), gsl_vector_get(s->x, 1), gsl_vector_get(s->x, 2)));

  // Free allocated memory
  gsl_vector_free(x);
  gsl_vector_free(ss);
  gsl_multimin_fminimizer_free(s);

  return R_mean;
}
#endif // USE_GSL


#ifndef DOXYGEN
std::vector<Quaternion> Quaternions::operator+(const double a, const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]+a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const double a, const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a-Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const double a, const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a*Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const double a, const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a/Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator+(const std::vector<double>& a, const Quaternion& Q) {
  vector<Quaternion> R(a.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q+a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const std::vector<double>& a, const Quaternion& Q) {
  vector<Quaternion> R(a.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a[i]-Q;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const std::vector<double>& a, const Quaternion& Q) {
  vector<Quaternion> R(a.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a[i]*Q;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const std::vector<double>& a, const Quaternion& Q) {
  vector<Quaternion> R(a.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a[i]/Q;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator+(const std::vector<double>& a, const std::vector<Quaternion>& Q) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]+a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const std::vector<double>& a, const std::vector<Quaternion>& Q) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a[i]-Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const std::vector<double>& a, const std::vector<Quaternion>& Q) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a[i]*Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const std::vector<double>& a, const std::vector<Quaternion>& Q) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a[i]/Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator+(const Quaternion& a, const std::vector<Quaternion>& Q) {
  std::vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]+a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const Quaternion& a, const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a-Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const Quaternion& a, const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a*Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const Quaternion& a, const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = a/Q[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator+(const std::vector<Quaternion>& Q, const Quaternion& a) {
  std::vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]+a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const std::vector<Quaternion>& Q, const Quaternion& a) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]-a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const std::vector<Quaternion>& Q, const Quaternion& a) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]*a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const std::vector<Quaternion>& Q, const Quaternion& a) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]/a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator+(const std::vector<Quaternion>& Q, const double a) {
  std::vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]+a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const std::vector<Quaternion>& Q, const double a) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]-a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const std::vector<Quaternion>& Q, const double a) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]*a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const std::vector<Quaternion>& Q, const double a) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]/a;
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator+(const Quaternion& Q, const std::vector<double>& a) {
  vector<Quaternion> R(a.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q+a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const Quaternion& Q, const std::vector<double>& a) {
  vector<Quaternion> R(a.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q-a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const Quaternion& Q, const std::vector<double>& a) {
  vector<Quaternion> R(a.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q*a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const Quaternion& Q, const std::vector<double>& a) {
  vector<Quaternion> R(a.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q/a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator+(const std::vector<Quaternion>& Q, const std::vector<double>& a) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]+a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const std::vector<Quaternion>& Q, const std::vector<double>& a) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]-a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const std::vector<Quaternion>& Q, const std::vector<double>& a) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]*a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const std::vector<Quaternion>& Q, const std::vector<double>& a) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]/a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator+(const std::vector<Quaternion>& Q, const std::vector<Quaternion>& a) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]+a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator-(const std::vector<Quaternion>& Q, const std::vector<Quaternion>& a) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]-a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator*(const std::vector<Quaternion>& Q, const std::vector<Quaternion>& a) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]*a[i];
  }
  return R;
}
std::vector<Quaternion> Quaternions::operator/(const std::vector<Quaternion>& Q, const std::vector<Quaternion>& a) {
  if(a.size() != Q.size()) {
    INFOTOCERR << " a.size()=" << a.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i]/a[i];
  }
  return R;
}


/////////////////////
// Array functions //
/////////////////////

std::vector<Quaternion> Quaternions::pow(const std::vector<Quaternion>& Q, const double t) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].pow(t);
  }
  return R;
}
std::vector<Quaternion> Quaternions::pow(const std::vector<Quaternion>& Q, const Quaternion& P) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].pow(P);
  }
  return R;
}
std::vector<Quaternion> Quaternions::pow(const Quaternion& Q, const std::vector<double>& t) {
  vector<Quaternion> R(t.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q.pow(t[i]);
  }
  return R;
}
std::vector<Quaternion> Quaternions::pow(const Quaternion& Q, const std::vector<Quaternion>& P) {
  vector<Quaternion> R(P.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q.pow(P[i]);
  }
  return R;
}
std::vector<Quaternion> Quaternions::pow(const std::vector<Quaternion>& Q, const std::vector<double>& t) {
  if(t.size() != Q.size()) {
    INFOTOCERR << " t.size()=" << t.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].pow(t[i]);
  }
  return R;
}
std::vector<Quaternion> Quaternions::pow(const std::vector<Quaternion>& Q, const std::vector<Quaternion>& P) {
  if(P.size() != Q.size()) {
    INFOTOCERR << " P.size()=" << P.size() << " != Q.size()=" << Q.size() << endl;
    throw(VectorSizeMismatch);
  }
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].pow(P[i]);
  }
  return R;
}
std::vector<double> Quaternions::abs(const std::vector<Quaternion>& Q) {
  vector<double> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].abs();
  }
  return R;
}
std::vector<Quaternion> Quaternions::log(const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].log();
  }
  return R;
}
std::vector<Quaternion> Quaternions::logRotor(const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].logRotor();
  }
  return R;
}
std::vector<Quaternion> Quaternions::exp(const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].exp();
  }
  return R;
}
std::vector<Quaternion> Quaternions::sqrt(const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].sqrt();
  }
  return R;
}
std::vector<Quaternion> Quaternions::sqrtOfRotor(const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].sqrtOfRotor();
  }
  return R;
}
std::vector<double> Quaternions::angle(const std::vector<Quaternion>& Q) {
  vector<double> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].angle();
  }
  return R;
}
std::vector<Quaternion> Quaternions::inverse(const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].inverse();
  }
  return R;
}
std::vector<Quaternion> Quaternions::conjugate(const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].conjugate();
  }
  return R;
}
std::vector<Quaternion> Quaternions::normalized(const std::vector<Quaternion>& Q) {
  vector<Quaternion> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].normalized();
  }
  return R;
}
std::vector<double> Quaternions::normsquared(const std::vector<Quaternion>& Q) {
  vector<double> R(Q.size());
  for(unsigned int i=0; i<R.size(); ++i) {
    R[i] = Q[i].normsquared();
  }
  return R;
}

std::vector<Quaternion> Quaternions::QuaternionArray(const std::vector<double>& vartheta, const std::vector<double>& varphi) {
  vector<Quaternion> Qs(vartheta.size());
  for(unsigned int i=0; i<Qs.size(); ++i) {
    Qs[i] = Quaternion(vartheta[i], varphi[i]);
  }
  return Qs;
}

std::vector<Quaternion> Quaternions::QuaternionArray(const std::vector<double>& alpha, const std::vector<double>& beta, const std::vector<double>& gamma) {
  vector<Quaternion> Qs(alpha.size());
  for(unsigned int i=0; i<Qs.size(); ++i) {
    Qs[i] = Quaternion(alpha[i], beta[i], gamma[i]);
  }
  return Qs;
}

std::vector<Quaternion> Quaternions::QuaternionArray(const std::vector<double>& w0, const std::vector<double>& x0, const std::vector<double>& y0, const std::vector<double>& z0) {
  vector<Quaternion> Qs(w0.size());
  for(unsigned int i=0; i<Qs.size(); ++i) {
    Qs[i] = Quaternion(w0[i], x0[i], y0[i], z0[i]);
  }
  return Qs;
}

std::vector<Quaternion> Quaternions::QuaternionArray(const std::vector<std::vector<double> >& q) {
  vector<Quaternion> Qs(q.size());
  for(unsigned int i=0; i<Qs.size(); ++i) {
    Qs[i] = Quaternion(q[i]);
  }
  return Qs;
}

std::vector<Quaternion> Quaternions::QuaternionArray(const std::vector<double>& angle, const std::vector<std::vector<double> >& axis) {
  vector<Quaternion> Qs(angle.size());
  for(unsigned int i=0; i<Qs.size(); ++i) {
    Qs[i] = Quaternion(angle[i], axis[i]);
  }
  return Qs;
}

std::vector<double> Quaternions::Component(const std::vector<Quaternions::Quaternion>& Q, const unsigned int i) {
  std::vector<double> v(Q.size());
  for(unsigned int j=0; j<Q.size(); ++j) {
    v[j] = Q[j][i];
  }
  return v;
}

std::vector<double> Quaternions::Component0(const std::vector<Quaternions::Quaternion>& Q) {
  return Component(Q, 0);
}

std::vector<double> Quaternions::Component1(const std::vector<Quaternions::Quaternion>& Q) {
  return Component(Q, 1);
}

std::vector<double> Quaternions::Component2(const std::vector<Quaternions::Quaternion>& Q) {
  return Component(Q, 2);
}

std::vector<double> Quaternions::Component3(const std::vector<Quaternions::Quaternion>& Q) {
  return Component(Q, 3);
}

std::vector<std::vector<double> > Quaternions::vec(const std::vector<Quaternions::Quaternion>& Q) {
  const unsigned int size = Q.size();
  std::vector<std::vector<double> > v(size);
  for(unsigned int i=0; i<size; ++i) {
    v[i] = Q[i].vec();
  }
  return v;
}

#endif // DOXYGEN


namespace {
  /// Returns the rapidity of a Lorentz boost with velocity three-vector v
  double Rapidity(const std::vector<double>& v) {
    /// The vector v is expected to be the velocity three-vector of the
    /// new frame relative to the current frame, in units where c=1.
    const double magv = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    return acosh(1.0/std::sqrt(1.0-magv*magv));
  }
}

/// Return a rotor taking the given vector into its boosted version
Quaternions::Quaternion Quaternions::BoostRotor(std::vector<double> ThreeVelocity,
                        std::vector<double> ThreeVectorToBeBoosted) {
  ///
  /// \param ThreeVelocity Three-vector velocity of the new frame WRT this frame
  /// \param ThreeVectorToBeBoosted Three-vector direction to be boosted by the rotor
  ///
  /// This function returns a rotor \f$R_b\f$ that takes the input
  /// vector \f$\hat{w}\f$ (which will be normalized) -- considered as
  /// a direction on the future null sphere -- into its boosted
  /// version.  Note that this rotor is a function of both the vector
  /// being boosted and the boost itself.
  ///

  const double alpha = ::Rapidity(ThreeVelocity);

  // If ThreeVelocity is too small to make much difference, just return the identity
  const double absThreeVelocity = std::sqrt(ThreeVelocity[0]*ThreeVelocity[0]
                        +ThreeVelocity[1]*ThreeVelocity[1]
                        +ThreeVelocity[2]*ThreeVelocity[2]);
  if(absThreeVelocity<1.0e-14 || std::abs(1-std::exp(alpha))<1.0e-14) {
    return Quaternion(1.0, 0.0, 0.0, 0.0);
  }

  // Otherwise, just normalize
  ThreeVelocity[0] = ThreeVelocity[0]/absThreeVelocity;
  ThreeVelocity[1] = ThreeVelocity[1]/absThreeVelocity;
  ThreeVelocity[2] = ThreeVelocity[2]/absThreeVelocity;

  // Normalize ThreeVectorToBeBoosted if possible
  const double absThreeVectorToBeBoosted = std::sqrt(ThreeVectorToBeBoosted[0]*ThreeVectorToBeBoosted[0]
                             +ThreeVectorToBeBoosted[1]*ThreeVectorToBeBoosted[1]
                             +ThreeVectorToBeBoosted[2]*ThreeVectorToBeBoosted[2]);
  if(absThreeVectorToBeBoosted==0.) {
    INFOTOCERR << " |ThreeVectorToBeBoosted|=" << absThreeVectorToBeBoosted << " is too small." << endl;
    throw(ValueError);
  }
  ThreeVectorToBeBoosted[0] = ThreeVectorToBeBoosted[0]/absThreeVectorToBeBoosted;
  ThreeVectorToBeBoosted[1] = ThreeVectorToBeBoosted[1]/absThreeVectorToBeBoosted;
  ThreeVectorToBeBoosted[2] = ThreeVectorToBeBoosted[2]/absThreeVectorToBeBoosted;

  // Evaluate the angle between ThreeVectorToBeBoosted and ThreeVelocity
  const double Theta = std::acos(ThreeVectorToBeBoosted[0]*ThreeVelocity[0]
                 +ThreeVectorToBeBoosted[1]*ThreeVelocity[1]
                 +ThreeVectorToBeBoosted[2]*ThreeVelocity[2]);

  // Calculate the new angle between ThreeVectorToBeBoostedPrime and ThreeVelocity
  const double ThetaPrime = 2 * std::atan( std::exp(alpha) * std::tan(0.5*Theta) );

  // Evaluate the cross product; if it is too small, just return the identity
  Quaternion ThreeVelocityThreeVectorToBeBoosted = Quaternion(ThreeVelocity).cross(Quaternion(ThreeVectorToBeBoosted));
  const double absThreeVelocityThreeVectorToBeBoosted = ThreeVelocityThreeVectorToBeBoosted.abs();
  if(absThreeVelocityThreeVectorToBeBoosted<1.0e-14) {
    return Quaternion(1.0, 0.0, 0.0, 0.0);
  }

  return Quaternions::exp( 0.5*(ThetaPrime-Theta) * ThreeVelocityThreeVectorToBeBoosted.normalized() );
}




///////////////////////////////////////////////
// Derivative and integral utility functions //
///////////////////////////////////////////////

/// Five-point finite-differencing of vector of Quaternions.
std::vector<Quaternion> Quaternions::QuaternionDerivative(const std::vector<Quaternion>& f, const std::vector<double>& t) {
  ///
  /// \param f Vector of Quaternions.
  /// \param t Vector of corresponding time steps.
  ///
  /// The formula for this finite difference comes from Eq. (A 5b) of
  /// "Derivative formulas and errors for non-uniformly spaced points"
  /// by M. K. Bowen and Ronald Smith.  As explained in their Eqs. (B
  /// 9b) and (B 10b), this is a fourth-order formula.
  ///
  /// If there are fewer than five points, the function reverts to
  /// simpler formulas.  If there are fewer than two points, or there
  /// are different numbers of points in the two input vectors, an
  /// exception is thrown.

  if(f.size() != t.size()) {
    INFOTOCERR << " f.size()=" << f.size() << " != t.size()=" << t.size() << endl;
    throw(VectorSizeMismatch);
  }
  if(f.size()<2) { INFOTOCERR << " size=" << f.size() << endl; throw(NotEnoughPointsForDerivative); }

  vector<Quaternion> D(f.size());
  const unsigned int i_f = f.size()-1;

  if(f.size()==2) {
    D[0] = (f[1]-f[0])/(t[1]-t[0]);
    D[1] = D[0];
    return D;
  }

  if(f.size()==3 || f.size()==4) {
    double hprev = t[1]-t[0];
    { // Compute first point
      const double hnext = t[2]-t[1];
      D[0] = -((2*hprev+hnext)/(hprev*(hprev+hnext)))*f[0] + ((hnext+hprev)/(hnext*hprev))*f[1] - (hprev/(hnext*(hnext+hprev)))*f[2];
    }
    for(unsigned int i=1; i<i_f; ++i) { // Compute intermediate points
      const double hnext = t[i+1]-t[i];
      /// Sundqvist and Veronis, Tellus XXII (1970), 1
      D[i] = (f[i+1] - f[i-1]*SQR(hnext/hprev) - f[i]*(1-SQR(hnext/hprev))) / (hnext*(1+hnext/hprev));
      hprev = hnext;
    }
    { // Compute final point
      const double hnext = t[i_f]  -t[i_f-1];
      const double hprev = t[i_f-1]-t[i_f-2];
      D[i_f] = (hnext/(hprev*(hprev+hnext)))*f[i_f-2] - ((hnext+hprev)/(hnext*hprev))*f[i_f-1] + ((hprev+2*hnext)/(hnext*(hnext+hprev)))*f[i_f];
    }
    return D;
  }

  for(unsigned int i=0; i<2; ++i) {
    const double x = t[i];
    const Quaternion& f1 = f[0];
    const Quaternion& f2 = f[1];
    const Quaternion& f3 = f[2];
    const Quaternion& f4 = f[3];
    const Quaternion& f5 = f[4];
    const double& x1 = t[0];
    const double& x2 = t[1];
    const double& x3 = t[2];
    const double& x4 = t[3];
    const double& x5 = t[4];
    const double h1 = x1 - x;
    const double h2 = x2 - x;
    const double h3 = x3 - x;
    const double h4 = x4 - x;
    const double h5 = x5 - x;
    const double h12 = x1 - x2;
    const double h13 = x1 - x3;
    const double h14 = x1 - x4;
    const double h15 = x1 - x5;
    const double h23 = x2 - x3;
    const double h24 = x2 - x4;
    const double h25 = x2 - x5;
    const double h34 = x3 - x4;
    const double h35 = x3 - x5;
    const double h45 = x4 - x5;
    D[i] =
      (-(h2*h3*h4 +h2*h3*h5 +h2*h4*h5 +h3*h4*h5)*f1/((h12)*(h13)*(h14)*(h15))
       +(h1*h3*h4 + h1*h3*h5 + h1*h4*h5 + h3*h4*h5)*f2/((h12)*(h23)*(h24)*(h25))
       -(h1*h2*h4 + h1*h2*h5 + h1*h4*h5 + h2*h4*h5)*f3/((h13)*(h23)*(h34)*(h35))
       +(h1*h2*h3 + h1*h2*h5 + h1*h3*h5 + h2*h3*h5)*f4/((h14)*(h24)*(h34)*(h45))
       -(h1*h2*h3 + h1*h2*h4 + h1*h3*h4 + h2*h3*h4)*f5/((h15)*(h25)*(h35)*(h45)));
  }
  for(unsigned int i=2; i<i_f-1; ++i) {
    const Quaternion& f1 = f[i-2];
    const Quaternion& f2 = f[i-1];
    const Quaternion& f3 = f[i];
    const Quaternion& f4 = f[i+1];
    const Quaternion& f5 = f[i+2];
    const double& x1 = t[i-2];
    const double& x2 = t[i-1];
    const double& x3 = t[i];
    const double& x4 = t[i+1];
    const double& x5 = t[i+2];
    const double h1 = x1 - x3;
    const double h2 = x2 - x3;
    const double h4 = x4 - x3;
    const double h5 = x5 - x3;
    const double h12 = x1 - x2;
    const double h13 = x1 - x3;
    const double h14 = x1 - x4;
    const double h15 = x1 - x5;
    const double h23 = x2 - x3;
    const double h24 = x2 - x4;
    const double h25 = x2 - x5;
    const double h34 = x3 - x4;
    const double h35 = x3 - x5;
    const double h45 = x4 - x5;
    D[i] =
      (-(h2*h4*h5)*f1/(h12*h13*h14*h15)
       +(h1*h4*h5)*f2/(h12*h23*h24*h25)
       -(h1*h2*h4 + h1*h2*h5 + h1*h4*h5 + h2*h4*h5)*f3/((h13)*(h23)*(h34)*(h35))
       +(h1*h2*h5)*f4/(h14*h24*h34*h45)
       -(h1*h2*h4)*f5/(h15*h25*h35*h45));
  }
  for(unsigned int i=i_f-1; i<i_f+1; ++i) {
    const double x = t[i];
    const Quaternion& f1 = f[i_f-4];
    const Quaternion& f2 = f[i_f-3];
    const Quaternion& f3 = f[i_f-2];
    const Quaternion& f4 = f[i_f-1];
    const Quaternion& f5 = f[i_f];
    const double& x1 = t[i_f-4];
    const double& x2 = t[i_f-3];
    const double& x3 = t[i_f-2];
    const double& x4 = t[i_f-1];
    const double& x5 = t[i_f];
    const double h1 = x1 - x;
    const double h2 = x2 - x;
    const double h3 = x3 - x;
    const double h4 = x4 - x;
    const double h5 = x5 - x;
    const double h12 = x1 - x2;
    const double h13 = x1 - x3;
    const double h14 = x1 - x4;
    const double h15 = x1 - x5;
    const double h23 = x2 - x3;
    const double h24 = x2 - x4;
    const double h25 = x2 - x5;
    const double h34 = x3 - x4;
    const double h35 = x3 - x5;
    const double h45 = x4 - x5;
    D[i] =
      (-(h2*h3*h4 +h2*h3*h5 +h2*h4*h5 +h3*h4*h5)*f1/((h12)*(h13)*(h14)*(h15))
       +(h1*h3*h4 + h1*h3*h5 + h1*h4*h5 + h3*h4*h5)*f2/((h12)*(h23)*(h24)*(h25))
       -(h1*h2*h4 + h1*h2*h5 + h1*h4*h5 + h2*h4*h5)*f3/((h13)*(h23)*(h34)*(h35))
       +(h1*h2*h3 + h1*h2*h5 + h1*h3*h5 + h2*h3*h5)*f4/((h14)*(h24)*(h34)*(h45))
       -(h1*h2*h3 + h1*h2*h4 + h1*h3*h4 + h2*h3*h4)*f5/((h15)*(h25)*(h35)*(h45)));
  }
  return D;

}
