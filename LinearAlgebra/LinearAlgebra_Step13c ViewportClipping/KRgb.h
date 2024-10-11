#pragma once

#include <math.h>
#include <iostream>
#include <objidl.h>
#include <gdiplus.h>

inline bool approx_eq(double a, double b)
{
    return fabs(a - b) < 1.0E-8;
}

class KRgb
{
    double rgb[3];

public:

    KRgb() { rgb[0] = 0.0; rgb[1] = 0.0; rgb[2] = 0.0; }
    KRgb(double r, double g, double b) { rgb[0] = r; rgb[1] = g; rgb[2] = b; }
    KRgb(const KRgb& rhs_) { *this = rhs_; }
    KRgb( double* pvalue_) { rgb[0] = pvalue_[0]; rgb[1] = pvalue_[1]; rgb[2] = pvalue_[2]; }
    KRgb( COLORREF color )
    {
        rgb[0] = GetRValue( color ) / 255.0;
        rgb[1] = GetGValue( color ) / 255.0;
        rgb[2] = GetBValue( color ) / 255.0;
    }

    Gdiplus::Color GetGdiColor()
    {
        Gdiplus::Color temp((BYTE)(rgb[0]*255.0), (BYTE)(rgb[1]*255.0), (BYTE)(rgb[2]*255.0));
        return temp;
    }

    KRgb& operator=(const KRgb& rhs_) { rgb[0] = rhs_.rgb[0]; rgb[1] = rhs_.rgb[1]; rgb[2] = rhs_.rgb[2]; return *this; }

    bool operator==(const KRgb& rhs_) const { return approx_eq(rgb[0], rhs_.rgb[0]) && approx_eq(rgb[1], rhs_.rgb[1]) && approx_eq(rgb[2], rhs_.rgb[2]); }
    bool operator!=(const KRgb& rhs_) const { return !this->operator==(rhs_); }
    KRgb operator+(const KRgb& rhs_) const
    {
        KRgb temp;
        temp.rgb[0] = rgb[0] + rhs_.rgb[0];
        temp.rgb[1] = rgb[1] + rhs_.rgb[1];
        temp.rgb[2] = rgb[2] + rhs_.rgb[2];
        return temp;
    }
    KRgb operator-(const KRgb& rhs_) const
    {
        KRgb temp;
        temp.rgb[0] = rgb[0] - rhs_.rgb[0];
        temp.rgb[1] = rgb[1] - rhs_.rgb[1];
        temp.rgb[2] = rgb[2] - rhs_.rgb[2];
        return temp;
    }
    KRgb operator*( double scale) const
    {
        KRgb temp;
        temp.rgb[0] = rgb[0] * scale;
        temp.rgb[1] = rgb[1] * scale;
        temp.rgb[2] = rgb[2] * scale;
        return temp;
    }
    KRgb operator*(const KRgb& rhs) const
    {
        KRgb temp;
        temp.rgb[0] = rgb[0] * rhs.rgb[0];
        temp.rgb[1] = rgb[1] * rhs.rgb[1];
        temp.rgb[2] = rgb[2] * rhs.rgb[2];
        return temp;
    }
    KRgb operator/( double scale) const
    {
        KRgb temp;
        temp.rgb[0] = rgb[0] / scale;
        temp.rgb[1] = rgb[1] / scale;
        temp.rgb[2] = rgb[2] / scale;
        return temp;
    }
    KRgb operator+=(const KRgb& rhs_)
    {
        rgb[0] += rhs_.rgb[0];
        rgb[1] += rhs_.rgb[1];
        rgb[2] += rhs_.rgb[2];
        return *this;
    }
    KRgb operator-=(const KRgb& rhs_)
    {
        rgb[0] -= rhs_.rgb[0];
        rgb[1] -= rhs_.rgb[1];
        rgb[2] -= rhs_.rgb[2];
        return *this;
    }
    KRgb operator/=( double scale)
    {
        rgb[0] /= scale;
        rgb[1] /= scale;
        rgb[2] /= scale;
        return *this;
    }
    KRgb operator*=( double scale)
    {
        rgb[0] *= scale;
        rgb[1] *= scale;
        rgb[2] *= scale;
        return *this;
    }
    double& operator[](int index)
    {
        return rgb[index];
    }
    const double& operator[](int index) const
    {
        return rgb[index];
    }

    friend std::ostream& operator<<(std::ostream& os, KRgb const& P)       { return os << "rgb=(" << P.rgb[0] << ", " << P.rgb[1] << ", " << P.rgb[2] << ")"; }
};

struct ScannedResult
{
    ScannedResult(int X, int Y, KRgb const& Col) : x(X), y(Y), col(Col) {}

    bool operator<(ScannedResult const& rhs) const { return (y < rhs.y) || ((y == rhs.y) && (x < rhs.x)); }

    int x, y;
    KRgb col;
};

/*

{
    KRgb col1;
    KRgb col2(1.0);
    double ptr[3] = { 1, 1, 1 };
    KRgb col3(ptr);
    KRgb col4(col3);

    assert(col1 == Black);
    assert(col2 == Red);
    assert(col3 == White);
    assert(col4 == White);

    try
    {
        std::cout << "col1 is " << col1 << std::endl;
        std::cout << "col2 is " << col2 << std::endl;
        std::cout << "col3 is " << col3 << std::endl;
        std::cout << "col4 is " << col4 << std::endl;


        KRgb col5 = col2 * 1.0;// + 1.0 * col3;
        col5 /= 2.0;
        std::cout << "col 5 is " << col5 << std::endl;

        col5 = col2 * col3;
        assert(col5 == Red);
        std::cout << "col 5 is " << col5 << std::endl;

        assert(col5 != col1);

        assert(White - Blue == Yellow);

        for (int i = 0; i<3; i++)
        col1[i] = col4[i];
    }
    catch (const char* msg)
    {
        std::cerr << msg << std::endl;
        exit(0);
    }
}

*/