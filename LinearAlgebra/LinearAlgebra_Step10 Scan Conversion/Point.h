/**
*\file		Point.h
*
*\brief	Point is a float triple, also type defined as Vertex and Vector.
*
*\author	Xianming Chen
*
*\date		11 Jul 2004\n
*\site      http://www.cs.utah.edu/~xchen/columbia/session2/
*\date      modified by _20180703_jintaeks
*/


#ifndef _POINT_H
#define _POINT_H

#include <iostream>

struct Point
{
    float x, y, z;

    explicit Point(float X = .0, float Y = .0, float Z = .0) : x(X), y(Y), z(Z)  { }
    explicit Point(float* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2])           { }

    Point GetUnit() const                                         { return Point(x / GetNorm(), y / GetNorm(), z / GetNorm()); }
    float GetNorm() const                                         { return sqrt(x*x + y*y + z*z); }

    float operator*(Point const& rhs) const                       { return x * rhs.x + y * rhs.y + z * rhs.z; }

    Point operator-()                                             { return Point(-x, -y, -z); }
    Point& operator+=(Point const& rhs)                           { x += rhs.x, y += rhs.y, z += rhs.z; return *this; }
    Point& operator-=(Point const& rhs)                           { x -= rhs.x, y -= rhs.y, z -= rhs.z; return *this; }
    Point operator+(Point const& rhs) const                       { Point tmp(*this); tmp += rhs; return tmp; }
    Point operator-(Point const& rhs) const                       { Point tmp(*this); tmp -= rhs; return tmp; }
    Point& operator*=(float scale)                                { x *= scale, y *= scale, z *= scale; return *this; }
    Point operator*(float scale) const                            { Point tmp(*this); tmp *= scale; return tmp; }
    Point& operator/=(float scale)                                { x /= scale, y /= scale, z /= scale; return *this; }
    Point operator/(float scale) const                            { Point tmp(*this); tmp /= scale; return tmp; }

    bool operator==(Point const& rhs) const                       { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(Point const& rhs) const                       { return !((*this) == rhs); }

    friend std::ostream& operator<<(std::ostream& os, Point const& P)       { return os << "(" << P.x << ", " << P.y << ", " << P.z << ")"; }
};

typedef Point Vertex;
typedef Point Vector;

#endif