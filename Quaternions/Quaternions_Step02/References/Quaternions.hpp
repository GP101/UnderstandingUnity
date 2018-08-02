
// Copyright (c) 2014, Michael Boyle
// See LICENSE file for details

#ifndef QUATERNIONS_HPP
#define QUATERNIONS_HPP

#include <vector>
#include <cmath>
#include <complex>
#include <iostream>

#define Quaternion_Epsilon 1.0e-14

namespace Quaternions {

  /// Object representing an individual quaternion
  class Quaternion {
  private:
    double w, x, y, z;
  public: // Constructors
    Quaternion();
    Quaternion(const Quaternion& Q);
    explicit Quaternion(const double vartheta, const double varphi); // explicit for python safety
    explicit Quaternion(const double alpha, const double beta, const double gamma); // explicit for python safety
    Quaternion(const double w0, const double x0, const double y0, const double z0);
    Quaternion(const std::vector<double>& q);
    explicit Quaternion(const double angle, const std::vector<double>& axis); // explicit for python safety
  public: // Access and manipulation
    Quaternion& operator=(const Quaternion& Q) { w=Q.w; x=Q.x; y=Q.y; z=Q.z; return *this; }
    Quaternion& operator+=(const Quaternion& Q) { w+=Q.w; x+=Q.x; y+=Q.y; z+=Q.z; return *this; }
    double operator[](const unsigned int i) const;
    double& operator[](const unsigned int i);
    inline bool operator!=(const Quaternion& Q) const { if(w!=Q.w) return false; if(x!=Q.x) return false; if(y!=Q.y) return false; if(z!=Q.z) return false; return true; }
    inline bool operator==(const Quaternion& Q) const { return !((*this) != Q); }
    inline Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }
    inline Quaternion operator+(const double t) const { return Quaternion(w+t, x, y, z); }
    inline Quaternion operator-(const double t) const { return Quaternion(w-t, x, y, z); }
    inline Quaternion operator*(const double t) const { return Quaternion(w*t, x*t, y*t, z*t); }
    inline Quaternion operator/(const double t) const { return Quaternion(w/t, x/t, y/t, z/t); }
    inline Quaternion operator+(const Quaternion& Q) const { return Quaternion(w+Q.w,x+Q.x,y+Q.y,z+Q.z); }
    inline Quaternion operator-(const Quaternion& Q) const { return Quaternion(w-Q.w,x-Q.x,y-Q.y,z-Q.z); }
    Quaternion operator*(const Quaternion& Q) const;
    inline Quaternion operator/(const Quaternion& Q) const { return (*this)*(Q.inverse()); }
    inline Quaternion pow(const double t) const { return (this->log() * t).exp(); }
    inline Quaternion pow(const Quaternion& Q) const { return (this->log() * Q).exp(); }
    inline Quaternion cross(const Quaternion& Q) const { return Quaternion(0, -z*Q.y+y*Q.z, z*Q.x-x*Q.z, -y*Q.x+x*Q.y); }
    inline Quaternion commutator(const Quaternion& Q) const { return Quaternion(0, -2*z*Q.y+2*y*Q.z, 2*z*Q.x-2*x*Q.z, -2*y*Q.x+2*x*Q.y); }
    inline double     dot(const Quaternion& Q) const { return x*Q.x+y*Q.y+z*Q.z; }
    inline double     abs() const { return std::sqrt(normsquared()); }
    Quaternion log() const;
    Quaternion logRotor() const;
    Quaternion exp() const;
    inline Quaternion sqrt() const { const double Abs = abs(); return (*this/Abs+1)*std::sqrt(Abs/(2+2*w/Abs)); }
    inline double     angle() const { return 2*logRotor().abs(); }
    inline Quaternion inverse() const { return conjugate()/normsquared(); }
    inline Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }
    inline Quaternion normalized() const { return (*this)/abs(); }
    inline double     normsquared() const { return (w*w+x*x+y*y+z*z); }
    inline Quaternion sqrtOfRotor() const { if(std::abs(1.0+w)<Quaternion_Epsilon) return Quaternion(0.0, 1.0, 0.0, 0.0); return (*this+1)/std::sqrt(2+2*w); }
    inline std::vector<double> vec() const { std::vector<double> v(3); v[0]=x; v[1]=y; v[2]=z; return v; }
    inline Quaternion XParityConjugateSpinor() const { return Quaternion(w, x, -y, -z); }
    inline Quaternion YParityConjugateSpinor() const { return Quaternion(w, -x, y, -z); }
    inline Quaternion ZParityConjugateSpinor() const { return Quaternion(w, -x, -y, z); }
    inline Quaternion  ParityConjugateSpinor() const { return Quaternion(w, x, y, z); }
    inline double IntrinsicDistance(const Quaternion& that) const { return 2*((*this)*that.inverse()).logRotor().abs(); }
    inline double ChordalDistance(const Quaternion& that) const { return ((*this) - that).abs(); }
    std::string str() const;
  };
  // Helper (non-member) functions for single Quaternions follow
  inline Quaternion operator+(const double a, const Quaternion& Q) { return Q+a; }
  inline Quaternion operator-(const double a, const Quaternion& Q) { return (-Q)+a; }
  inline Quaternion operator*(const double a, const Quaternion& Q) { return Q*a; }
  inline Quaternion operator/(const double a, const Quaternion& Q) { return Q.inverse()*a; }
  inline Quaternion pow(const Quaternion& Q, const double x) { return Q.pow(x); }
  inline Quaternion pow(const Quaternion& Q, const Quaternion& P) { return Q.pow(P); }
  inline Quaternion cross(const Quaternion& Q, const Quaternion& P) { return Q.cross(P); }
  inline Quaternion commutator(const Quaternion& Q, const Quaternion& P) { return Q.commutator(P); }
  inline double     dot(const Quaternion& Q, const Quaternion& P) { return Q.dot(P); }
  inline double     abs(const Quaternion& Q) { return Q.abs(); }
  inline Quaternion log(const Quaternion& Q) { return Q.log(); }
  inline Quaternion logRotor(const Quaternion& Q) { return Q.logRotor(); }
  inline Quaternion exp(const Quaternion& Q) { return Q.exp(); }
  inline Quaternion sqrt(const Quaternion& Q) { return Q.sqrt(); }
  inline double     angle(const Quaternion& Q) { return Q.angle(); }
  inline Quaternion inverse(const Quaternion& Q) { return Q.inverse(); }
  inline Quaternion conjugate(const Quaternion& Q) { return Q.conjugate(); }
  inline Quaternion normalized(const Quaternion& Q) { return Q.normalized(); }
  inline double     normsquared(const Quaternion& Q) { return Q.normsquared(); }
  inline Quaternion sqrtOfRotor(const Quaternion& Q) { return Q.sqrtOfRotor(); }
  inline std::vector<double> vec(const Quaternion& Q) { return Q.vec(); }
  inline Quaternion XParityConjugateSpinor(const Quaternion& Q) { return Q.XParityConjugateSpinor(); }
  inline Quaternion YParityConjugateSpinor(const Quaternion& Q) { return Q.YParityConjugateSpinor(); }
  inline Quaternion ZParityConjugateSpinor(const Quaternion& Q) { return Q.ZParityConjugateSpinor(); }
  inline Quaternion  ParityConjugateSpinor(const Quaternion& Q) { return Q.ParityConjugateSpinor(); }
  inline double IntrinsicDistance(const Quaternion& R_A, const Quaternion& R_B) { return 2*(R_A*R_B.inverse()).logRotor().abs(); }
  inline double ChordalDistance(const Quaternion& R_A, const Quaternion& R_B) { return (R_A - R_B).abs(); }
  inline Quaternion Slerp(const double tau, const Quaternion& Qa, const Quaternion& Qb) { return ( pow((Qb/Qa), tau) * Qa ); }
  std::ostream& operator<<(std::ostream& out, const Quaternions::Quaternion& q);

  // Standard constants we might want (with ugly syntax for SWIG)
  const Quaternion Zero = Quaternion(0.0, 0.0, 0.0, 0.0);
  const Quaternion  One = Quaternion(1.0, 0.0, 0.0, 0.0);
  const Quaternion xHat = Quaternion(0.0, 1.0, 0.0, 0.0);
  const Quaternion yHat = Quaternion(0.0, 0.0, 1.0, 0.0);
  const Quaternion zHat = Quaternion(0.0, 0.0, 0.0, 1.0);

  // Functions for arrays of Quaternion objects
  std::vector<Quaternion> IndefiniteIntegral(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn);
  Quaternion DefiniteIntegral(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn);
  std::vector<Quaternion> DifferentiateRotorByLogarithm(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn);
  std::vector<Quaternion> MinimalRotation(const std::vector<Quaternion>& R, const std::vector<double>& T, const unsigned int NIterations=5);
  std::vector<Quaternion> PrescribedRotation(const std::vector<double>& RotationRateAboutZ,
                                             const std::vector<Quaternion>& R, const std::vector<double>& T, const unsigned int NIterations=5);
  std::vector<Quaternion> FrameFromXY(const std::vector<Quaternion>& X, const std::vector<Quaternion>& Y);
  std::vector<Quaternion> FrameFromZ(const std::vector<Quaternion>& Z, const std::vector<double>& T, const unsigned int NIterations=5);
  std::vector<Quaternion> FrameFromPrescribedRotation(const std::vector<Quaternion>& omega, const std::vector<double>& T, const unsigned int NIterations=5);
  std::vector<double> FrameFromAngularVelocity_Integrand(const std::vector<double>& r, const std::vector<double>& Omega);
  void FrameFromAngularVelocity_2D_Integrand(const double r_x, const double r_y, std::vector<double> Omega, double& rdot_x, double& rdot_y);
  std::vector<Quaternion> UnflipRotors(const std::vector<Quaternion>& R, const double discont=1.4142135623730951);
  std::vector<Quaternion> RDelta(const std::vector<Quaternion>& R1, const std::vector<Quaternion>& R2, const int IndexOfFiducialTime=-1);
  std::vector<Quaternion> Squad(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn, const std::vector<double>& tOut);
  std::vector<Quaternion> FrameAngularVelocity(const std::vector<Quaternion>& f, const std::vector<double>& t);

  // Mean rotors
  Quaternion ApproximateMeanRotor(const std::vector<Quaternion>& R);
  Quaternion ApproximateMeanRotor(const std::vector<Quaternion>& R, const std::vector<double>& t,
                                  const double t1=-1e300, const double t2=1e300);
  Quaternion ApproximateOptimalAlignmentRotor(const std::vector<Quaternion>& Ra, const std::vector<Quaternion>& Rb,
                                              const std::vector<double>& t, const double t1=-1e300, const double t2=1e300);
  #ifdef USE_GSL
  void ApproximateOptimalAlignment(const double t1, const double t2,
                                   const std::vector<Quaternion>& Ra, const std::vector<double>& ta,
                                   const std::vector<Quaternion>& Rb, const std::vector<double>& tb,
                                   double& deltat, Quaternions::Quaternion& R_delta);
  void OptimalAlignment(const double t1, const double t2,
                        const std::vector<Quaternion>& Ra, const std::vector<double>& ta,
                        const std::vector<Quaternion>& Rb, const std::vector<double>& tb,
                        double& deltat, Quaternions::Quaternion& R_delta);
  Quaternion MeanRotor(const std::vector<Quaternion>& R);
  Quaternion MeanRotor(const std::vector<Quaternion>& R, const std::vector<double>& t,
                       double t1=-1e300, double t2=1e300);
  #endif // USE_GSL

  std::vector<Quaternion> operator+(const double a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator-(const double a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator*(const double a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator/(const double a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator+(const std::vector<double>& a, const Quaternion& Q);
  std::vector<Quaternion> operator-(const std::vector<double>& a, const Quaternion& Q);
  std::vector<Quaternion> operator*(const std::vector<double>& a, const Quaternion& Q);
  std::vector<Quaternion> operator/(const std::vector<double>& a, const Quaternion& Q);
  std::vector<Quaternion> operator+(const std::vector<double>& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator-(const std::vector<double>& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator*(const std::vector<double>& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator/(const std::vector<double>& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator+(const Quaternion& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator-(const Quaternion& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator*(const Quaternion& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator/(const Quaternion& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator+(const std::vector<Quaternion>& Q, const Quaternion& a);
  std::vector<Quaternion> operator-(const std::vector<Quaternion>& Q, const Quaternion& a);
  std::vector<Quaternion> operator*(const std::vector<Quaternion>& Q, const Quaternion& a);
  std::vector<Quaternion> operator/(const std::vector<Quaternion>& Q, const Quaternion& a);
  std::vector<Quaternion> operator+(const std::vector<Quaternion>& Q, const double a);
  std::vector<Quaternion> operator-(const std::vector<Quaternion>& Q, const double a);
  std::vector<Quaternion> operator*(const std::vector<Quaternion>& Q, const double a);
  std::vector<Quaternion> operator/(const std::vector<Quaternion>& Q, const double a);
  std::vector<Quaternion> operator+(const Quaternion& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator-(const Quaternion& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator*(const Quaternion& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator/(const Quaternion& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator+(const std::vector<Quaternion>& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator-(const std::vector<Quaternion>& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator*(const std::vector<Quaternion>& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator/(const std::vector<Quaternion>& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator+(const std::vector<Quaternion>& P, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator-(const std::vector<Quaternion>& P, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator*(const std::vector<Quaternion>& P, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator/(const std::vector<Quaternion>& P, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> pow(const std::vector<Quaternion>& Q, const double x);
  std::vector<Quaternion> pow(const std::vector<Quaternion>& Q, const Quaternion& P);
  std::vector<Quaternion> pow(const Quaternion& Q, const std::vector<double>& x);
  std::vector<Quaternion> pow(const Quaternion& Q, const std::vector<Quaternion>& P);
  std::vector<Quaternion> pow(const std::vector<Quaternion>& Q, const std::vector<double>& x);
  std::vector<Quaternion> pow(const std::vector<Quaternion>& Q, const std::vector<Quaternion>& P);
  std::vector<double>     abs(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> log(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> logRotor(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> exp(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> sqrt(const std::vector<Quaternion>& Q);
  std::vector<double>     angle(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> inverse(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> conjugate(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> normalized(const std::vector<Quaternion>& Q);
  std::vector<double>     normsquared(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> sqrtOfRotor(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> QuaternionArray(const std::vector<double>& vartheta, const std::vector<double>& varphi);
  std::vector<Quaternion> QuaternionArray(const std::vector<double>& alpha, const std::vector<double>& beta, const std::vector<double>& gamma);
  std::vector<Quaternion> QuaternionArray(const std::vector<double>& w0, const std::vector<double>& x0, const std::vector<double>& y0, const std::vector<double>& z0);
  std::vector<Quaternion> QuaternionArray(const std::vector<std::vector<double> >& q);
  std::vector<Quaternion> QuaternionArray(const std::vector<double>& angle, const std::vector<std::vector<double> >& axis);
  std::vector<double> Component(const std::vector<Quaternions::Quaternion>& Q, const unsigned int i);
  std::vector<double> Component0(const std::vector<Quaternions::Quaternion>& Q);
  std::vector<double> Component1(const std::vector<Quaternions::Quaternion>& Q);
  std::vector<double> Component2(const std::vector<Quaternions::Quaternion>& Q);
  std::vector<double> Component3(const std::vector<Quaternions::Quaternion>& Q);

  std::vector<std::vector<double> > vec(const std::vector<Quaternions::Quaternion>& Q);

  std::vector<Quaternion> QuaternionDerivative(const std::vector<Quaternion>& f, const std::vector<double>& t);

  Quaternion BoostRotor(std::vector<double> ThreeVelocity,
            std::vector<double> ThreeVectorToBeBoosted);

} // namespace Quaternions

#endif // QUATERNIONS_HPP
