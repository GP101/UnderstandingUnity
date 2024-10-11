#include "stdafx.h"
#include "KVectorUtil.h"
#include "KMatrix2.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>
#include "KMath.h"
#include <windowsx.h>

KBasis2             KVectorUtil::g_basis2;
KScreenCoordinate   KVectorUtil::g_screenCoordinate;
extern int g_idebug;


void KVectorUtil::SetScreenCoordinate(const KScreenCoordinate& screenCoord)
{
    g_screenCoordinate = screenCoord;
}

void KVectorUtil::SetBasis2(const KBasis2& basis2)
{
    g_basis2 = basis2;
}

KVector2 KVectorUtil::ScreenToWorld(const KVector2& v0_)
{
    KMatrix2 m0;
    m0.Set(g_basis2.basis0, g_basis2.basis1);
    KMatrix2 m1;
    m1.Set(g_screenCoordinate.axis0, g_screenCoordinate.axis1);

    // Vscreen = Mscreen * Mworld * Vworld
    // MworldInv * MscreenInv * Vscreen = Vworld
    KVector2 v = v0_ - g_screenCoordinate.origin; // inverse translation
    KMatrix2 m1Inv = m1.GetInverse();
    KMatrix2 m0Inv = m0.GetInverse();
    v = m0Inv * m1Inv * v;
    return v;
}

KVector2 KVectorUtil::WorldToScreen(const KVector2& v0)
{
    return g_screenCoordinate.Transform(g_basis2.Transform(v0));
}

void KVectorUtil::DrawLine(HDC hdc, const KVector2& v0_, const KVector2& v1_, int lineWidth, int penStyle, COLORREF color)
{
    KVector2 v0 = g_basis2.Transform(v0_);
    KVector2 v1 = g_basis2.Transform(v1_);
    v0 = g_screenCoordinate.Transform(v0);
    v1 = g_screenCoordinate.Transform(v1);

    HPEN hpen = CreatePen(penStyle, lineWidth, color);
    HGDIOBJ original = SelectObject(hdc, hpen);
    {
        MoveToEx(hdc, (int)v0.x, (int)v0.y, nullptr);
        LineTo(hdc, (int)v1.x, (int)v1.y);
    }
    SelectObject(hdc, original);
    DeleteObject(hpen);
}

void KVectorUtil::DrawLine(HDC hdc, double x0, double y0, double x1, double y1
    , int lineWidth, int penStyle, COLORREF color)
{
    DrawLine(hdc, KVector2(x0, y0), KVector2(x1, y1), lineWidth, penStyle, color);
}

void KVectorUtil::DrawAxis(HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color)
{
    {
        KVector2 v0 = KVector2(0, -numVerticalGrid / 2);
        KVector2 v1 = KVector2(0, numVerticalGrid / 2);
        KVectorUtil::DrawLine(hdc, v0, v1, 2, PS_SOLID, color );
    }

    {
        KVector2 v0 = KVector2(-numHorizontalGrid / 2, 0);
        KVector2 v1 = KVector2(numHorizontalGrid / 2, 0);
        KVectorUtil::DrawLine(hdc, v0, v1, 2, PS_SOLID, color );
    }
}

void KVectorUtil::DrawGrid( HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color)
{
    int hbegin = int(-numHorizontalGrid / 2.0 - 0.5);
    for (int count = 0; count <= numHorizontalGrid; ++count) {
        KVector2 v0 = KVector2(hbegin, 0) + KVector2(0, -numVerticalGrid / 2);
        KVector2 v1 = KVector2(hbegin, 0) + KVector2(0, numVerticalGrid / 2);
        KVectorUtil::DrawLine(hdc, v0, v1, 1, PS_DOT, color );
        hbegin += 1;
    }

    int vbegin = int(-numVerticalGrid / 2.0 - 0.5);
    for (int count = 0; count <= numVerticalGrid; ++count) {
        KVector2 v0 = KVector2(0,vbegin) + KVector2( -numHorizontalGrid / 2, 0 );
        KVector2 v1 = KVector2(0,vbegin)+ KVector2( numHorizontalGrid / 2, 0);
        KVectorUtil::DrawLine(hdc, v0, v1, 1, PS_DOT, color);
        vbegin += 1;
    }
}

void KVectorUtil::DrawCircle(HDC hdc, const KVector2& center, double radius, int numSegment
    , int lineWidth, int penStyle, COLORREF color)
{
    const double dt = (2.0 * PI) / numSegment;
    double theta = 0;
    const KVector2 p0 = KVector2(radius, 0.0f);
    KVector2 p1 = p0;
    KVector2 p2;
    for (int i = 0; i <= numSegment; ++i)
    {
        KMatrix2 m;
        theta += dt;
        m.SetRotation((double)theta);
        p2 = m * p0;
        DrawLine(hdc, p1 + center, p2 + center, lineWidth, penStyle, color);
        p1 = p2;
    }
}

void KVectorUtil::PutPixel(HDC hdc, int x_, int y_, Gdiplus::Color color)
{
    Gdiplus::Graphics    graphics(hdc);
    const double dotWidth = g_screenCoordinate.axis0.Length();
    const double dotHeight = g_screenCoordinate.axis1.Length();

    const double x = (double)x_;
    const double y = (double)y_;
    Gdiplus::Point  point[4];
    KVector2 v[4] = { KVector2(x, y)
        , KVector2(x, y + 1)
        , KVector2(x + 1, y + 1)
        , KVector2(x + 1, y) };
    for (int i = 0; i < 4; ++i)
    {
        KVector2 v0 = g_basis2.Transform(v[i]);
        v0 = g_screenCoordinate.Transform(v0);
        point[i].X = (int)v0.x;
        point[i].Y = (int)v0.y;
    }

    Gdiplus::SolidBrush  brush(color);
    graphics.FillPolygon(&brush, point, 4);
    //graphics.FillRectangle(&brush, v0.x, v0.y, dotWidth, dotHeight);
}

void KVectorUtil::_ScanLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    auto sign = []( double delta){ return delta > 0.f ? 1.0 : -1.0; };

    double deltax = x1 - x0;
    double deltay = y1 - y0;
    double deltaerr = abs(deltay / deltax); // Assume deltax != 0 (line is not vertical),
    // note that this division needs to be done in a way that preserves the fractional part
    double error = 0.0; // No error at start
    int y = y0;
    for (int x = x0; x <= x1; ++x)
    {
        PutPixel(hdc, x, y, color);
        error = error + deltaerr;
        if (error >= 0.5)
        {
            y = y + int(sign(deltay) * 1.0);
            error = error - 1.0;
        }
    }
}

void KVectorUtil::_ScanLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    auto sign = []( double delta){ return delta > 0.0 ? 1.0 : -1.0; };

    double deltax = x1 - x0;
    double deltay = y1 - y0;
    double deltaerr = abs(deltax / deltay); // Assume deltax != 0 (line is not vertical),
    // note that this division needs to be done in a way that preserves the fractional part
    double error = 0.0; // No error at start
    int x = x0;
    for (int y = y0; y <= y1; ++y)
    {
        PutPixel(hdc, x, y, color);
        error = error + deltaerr;
        if (error >= 0.5)
        {
            x = x + int(sign(deltax) * 1.0);
            error = error - 1.0;
        }
    }
}

void KVectorUtil::ScanLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            _ScanLineLow(hdc, x1, y1, x0, y0, color);
        }
        else {
            _ScanLineLow(hdc, x0, y0, x1, y1, color);
        }

    }
    else {
        if (y0 > y1) {
            _ScanLineHigh(hdc, x1, y1, x0, y0, color);
        }
        else {
            _ScanLineHigh(hdc, x0, y0, x1, y1, color);
        }
    }
}

void KVectorUtil::ScanPlotLineOctant0(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int D = 2 * dy - dx;
    int y = y0;

    for( int x = x0; x <= x1; ++x )
    {
        PutPixel(hdc, x, y, color);
        if (D > 0) {
            y = y + 1;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }
}

void KVectorUtil::_ScanPlotLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;
    int y = y0;

    for (int x = x0; x <= x1; ++x)
    {
        PutPixel(hdc, x, y, color);
        if (D > 0) {
            y = y + yi;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }
}

void KVectorUtil::_ScanPlotLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int D = 2 * dx - dy;
    int x = x0;
    for (int y = y0; y <= y1; ++y) {
        PutPixel(hdc, x, y, color);
        if (D > 0) {
            x = x + xi;
            D = D - 2 * dy;
        }
        D = D + 2 * dx;
    }
}

void KVectorUtil::ScanPlotLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            _ScanPlotLineLow(hdc, x1, y1, x0, y0, color);
        }
        else {
            _ScanPlotLineLow(hdc, x0, y0, x1, y1, color);
        }

    } 
    else {
        if (y0 > y1) {
            _ScanPlotLineHigh(hdc, x1, y1, x0, y0, color);
        }
        else {
            _ScanPlotLineHigh(hdc, x0, y0, x1, y1, color);
        }
    }
}

void KVectorUtil::ScanMidpointCircle(HDC hdc, int x_centre, int y_centre, int r, Gdiplus::Color color)
{
    int x = r;
    int y = 0;

    Gdiplus::Color color2 = color;
#ifdef _DEBUG
    //color2 = Gdiplus::Color::Red; // for debug
#endif

    // Printing the initial point on the axes
    // after translation
    PutPixel(hdc, x + x_centre, y + y_centre, color);

    // When radius is zero only a single
    // point will be printed
    if (r > 0)
    {
        PutPixel(hdc, -x + x_centre, y + y_centre, color2);
        PutPixel(hdc, y + x_centre, x + y_centre, color);
        PutPixel(hdc, -y + x_centre, -x + y_centre, color2);
    }

    // Initialising the value of P
    int P = 1 - r;
    int dbgCnt = 0;
    while (x > y)
    {
        y++;

        // Mid-point is inside or on the perimeter
        if (P <= 0)
            P = P + 2 * y + 1;
        // Mid-point is outside the perimeter
        else
        {
            x--;
            P = P + 2 * y - 2 * x + 1;
        }

        // All the perimeter points have already been printed
        if (x < y)
            break;

#ifdef _DEBUG
        //if (dbgCnt == g_idebug)
        //    break;
        //dbgCnt += 1;
#endif

        // Printing the generated point and its reflection
        // in the other octants after translation
        PutPixel(hdc, x + x_centre, y + y_centre, color);
        PutPixel(hdc, -x + x_centre, y + y_centre, color2);
        PutPixel(hdc, x + x_centre, -y + y_centre, color);
        PutPixel(hdc, -x + x_centre, -y + y_centre, color2);

        // If the generated point is on the line x = y then
        // the perimeter points have already been printed
        if (x != y)
        {
            PutPixel(hdc, y + x_centre, x + y_centre, color);
            PutPixel(hdc, -y + x_centre, x + y_centre, color2);
            PutPixel(hdc, y + x_centre, -x + y_centre, color);
            PutPixel(hdc, -y + x_centre, -x + y_centre, color2);
        }
    }
}

/*
Scan Conversion and Filling Triangle Algorithms
*/
void KVectorUtil::ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
    , std::set<ScannedResult>* output)
{
    // if swap_xy, flip the line around y=x line(i.e. the diagonal line).
    // if flip_y,  flip the line around y=0 line(i.e, x axis)
    bool swap_xy = false;
    bool flip_y = false;

    auto _scanLineSegment = [&hdc, &swap_xy, &flip_y](int x1, int y1, KRgb const col1
        , int x2, int y2, KRgb const col2
        , std::set<ScannedResult>* output)
    {
        assert(x2 - x1 >= 0 && x2 - x1 >= y2 - y1 && y2 - y1 >= 0);

        bool horizontal = y1 == y2;
        bool diagonal = y2 - y1 == x2 - x1;
        double scale = 1.0 / (x2 - x1);

        //KRgb col_step2 = (col2 - col1) * scale;
        KRgb col = col1, col_step = (col2 - col1) * scale * .99999f;

        int y = y1;
        double yy = (double)y1, y_step = (y2 - y1) * scale;

        for (int x = x1; x <= x2; ++x, col += col_step)
        {
            int X(x), Y(y);
            if (swap_xy) std::swap(X, Y);
            if (flip_y)  Y = -Y;

            //col[0] = __min(__max(col[0], 0), 255);
            //col[1] = __min(__max(col[1], 0), 255);
            //col[2] = __min(__max(col[2], 0), 255);
            if (output)
                output->insert(ScannedResult(X, Y, col));
            else {
                //Pixel(Y + vReso / 2, X + hReso / 2) = col;
                PutPixel(hdc, X, Y, col.GetGdiColor());
            }

            if (!horizontal)
            {
                if (diagonal) y++;
                else y = int(round(yy += y_step));
            }
        }
    };

    if (x1 > x2) // always scan convert from left to right.
    {
        std::swap(x1, x2); std::swap(y1, y2); std::swap(c1, c2);
    }
    flip_y = (y1 > y2);
    if (flip_y) // always scan convert from down to up.
    {
        y1 = -y1, y2 = -y2;
    }
    swap_xy = (y2 - y1 > x2 - x1);
    if (swap_xy) // and always scan convert a line with <= 45 deg to x-axis.
    {
        std::swap(x1, y1), std::swap(x2, y2);
    }
    _scanLineSegment(x1, y1, c1, x2, y2, c2, output);
}

void KVectorUtil::ScanLineSegment( HDC hdc, double x1, double y1, KRgb c1, double x2, double y2, KRgb c2
    , std::set<ScannedResult>* output )
{
    ScanLineSegment( hdc, (int)x1, (int)y1, c1, (int)x2, (int)y2, c2, output );
}

void KVectorUtil::FillTriangle(HDC hdc, int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , int x3, int y3, KRgb const col3)

{
    std::set<ScannedResult> scanned_pnts; // sorted in y val, and in x val if y val the same.
    ScanLineSegment(hdc, x1, y1, col1, x2, y2, col2, &scanned_pnts);
    ScanLineSegment(hdc, x2, y2, col2, x3, y3, col3, &scanned_pnts);
    ScanLineSegment(hdc, x3, y3, col3, x1, y1, col1, &scanned_pnts);

    int cur_yval = INT_MIN;// vReso / 2; // initialize to an invalid value.
    std::set<ScannedResult> same_yval; // of the scanned result.

    for (std::set<ScannedResult>::iterator it = scanned_pnts.begin(); it != scanned_pnts.end(); ++it)
    {
        int y = it->y;
        if (y != cur_yval)
        {
            if (same_yval.size())
            {
                std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
                ScanLineSegment(hdc, it1->x, cur_yval, it1->col,
                    it2->x, cur_yval, it2->col, nullptr);

                same_yval.clear();
            }
            cur_yval = y;
        }
        same_yval.insert(*it);
    }
    if (same_yval.size())
    {
        std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
        ScanLineSegment(hdc, it1->x, cur_yval, it1->col
            , it2->x, cur_yval, it2->col, nullptr);
    }
}

void KVectorUtil::FillTriangle( HDC hdc, double x1, double y1, KRgb const col1
    , double x2, double y2, KRgb const col2
    , double x3, double y3, KRgb const col3 )
{
    FillTriangle( hdc, (int)x1, (int)y1, col1, (int)x2, (int)y2, col2, (int)x3, (int)y3, col3 );
}

double KVectorUtil::LengthSquared(const KVector2& a, const KVector2& b)
{
    KVector2 t = b - a;
    return t.x * t.x + t.y * t.y;
}

double KVectorUtil::Length(const KVector2& a, const KVector2& b)
{
    return sqrt(LengthSquared(a, b));
}

double KVectorUtil::PointLinesegmentDistance(KVector2 p, KVector2 v, KVector2 w)
{
    // Return minimum distance between line segment vw and point p
    const double l2 = LengthSquared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 <= 0.0001) return Length(p, v);   // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    const double t = __max(0, __min(1, KVector2::Dot(p - v, w - v) / l2));
    const KVector2 projection = v + t * (w - v);  // Projection falls on the segment
    return Length(p, projection);
}

double KVectorUtil::PointLineDistance(KVector2 p, KVector2 v0, KVector2 v1)
{
    KVector2 ab = v1 - v0;
    KVector2 ac = p - v0;
    double area = KVector2::Cross(ab, ac);
    double CD = area / ab.Length();
    return CD;
}

bool KVectorUtil::IsPointInPolygon(const KVector2& p, const std::vector<KVector2>& points)
{
    const int numPoints = (int)points.size();
    if (numPoints <= 2)
        return false;
    return IsPointInPolygon(p, &points[0], numPoints);
}

bool KVectorUtil::IsPointInPolygon(const KVector2& p, const KVector2* points, int numPoints)
{
    if (numPoints <= 2)
        return false;
    for (int i = 0; i < numPoints; ++i)
    {
        const KVector2& p1 = points[i];
        const KVector2& p2 = points[(i + 1) % numPoints];
        if (!KVector2::IsCCW(p1 - p, p2 - p))
            return false;
    }
    return true;
}

bool KVectorUtil::LineSegementsIntersect(KVector2 p, KVector2 p2, KVector2 q, KVector2 q2
    , KVector2& intersection, bool considerCollinearOverlapAsIntersect)
{
    KVector2 r = p2 - p;
    KVector2 s = q2 - q;
    double rxs = KVector2::Cross(r, s);
    double qpxr = KVector2::Cross(q - p, r);

    // If r x s = 0 and (q - p) x r = 0, then the two lines are collinear.
    if (IsZero(rxs) && IsZero(qpxr))
    {
        // 1. If either  0 <= (q - p) * r <= r * r or 0 <= (p - q) * s <= * s
        // then the two lines are overlapping,
        if (considerCollinearOverlapAsIntersect)
            if ((0 <= (q - p) * r && (q - p) * r <= r * r) || (0 <= (p - q) * s && (p - q) * s <= s * s))
                return true;

        // 2. If neither 0 <= (q - p) * r = r * r nor 0 <= (p - q) * s <= s * s
        // then the two lines are collinear but disjoint.
        // No need to implement this expression, as it follows from the expression above.
        return false;
    }

    // 3. If r x s = 0 and (q - p) x r != 0, then the two lines are parallel and non-intersecting.
    if (IsZero(rxs) && !IsZero(qpxr))
        return false;

    // t = (q - p) x s / (r x s)
    double t = KVector2::Cross((q - p), s) / rxs;

    // u = (q - p) x r / (r x s)

    double u = KVector2::Cross((q - p), r) / rxs;

    // 4. If r x s != 0 and 0 <= t <= 1 and 0 <= u <= 1
    // the two line segments meet at the point p + t r = q + u s.
    if (!IsZero(rxs) && (0 <= t && t <= 1) && (0 <= u && u <= 1))
    {
        // We can calculate the intersection point using either t or u.
        intersection = p + t * r;

        // An intersection was found.
        return true;
    }

    // 5. Otherwise, the two line segments are not parallel but do not intersect.
    return false;
}

int KVectorUtil::LineSegmentPolygonIntersection(const KVector2& p0, const KVector2& p1, const std::vector<KVector2>& points)
{
    KVector2 collPoint;
    int numIntersection = 0;
    for (uint32 i = 0; i < points.size(); ++i)
    {
        const int i0 = i;
        const int i1 = (i + 1) % points.size();
        const bool isIntersect = KVectorUtil::LineSegementsIntersect(p0, p1, points[i0], points[i1], collPoint);
        if (isIntersect)
            numIntersection += 1;
    }
    return numIntersection;
}

KVector2 KVectorUtil::GetGeoCenter(const KVector2* vertices, int vertexCount)
{
    KVector2 geoCenter = KVector2::zero;

    for (int i1 = 0; i1 < vertexCount; ++i1)
    {
        geoCenter += vertices[i1];
    }
    geoCenter.x /= (double)vertexCount;
    geoCenter.y /= (double)vertexCount;
    return geoCenter;
}

KVector2 KVectorUtil::GetGeoCenter(const std::vector<KVector2>& points)
{
    return GetGeoCenter(&points[0], (int)points.size());
}

// This functions clips all the edges w.r.t one clip
// edge of clipping area
void KVectorUtil::Clip(const std::vector<KVector2>& points, const KVector2 p0, const KVector2 p1
    , std::vector<KVector2>& new_points)
{
    const int poly_size = (int)points.size();

    // vi,vk are the co-ordinate values of
    // the points
    for (int i = 0; i < poly_size; i++)
    {
        // i and k form a line in polygon
        int k = (i + 1) % poly_size;
        KVector2 vi = points[i];
        KVector2 vk = points[k];

        // Calculating position of first point
        // w.r.t. clipper line
        double i_pos = (p1.x - p0.x) * (vi.y - p0.y) - (p1.y - p0.y) * (vi.x - p0.x);

        // Calculating position of second point
        // w.r.t. clipper line
        double k_pos = (p1.x - p0.x) * (vk.y - p0.y) - (p1.y - p0.y) * (vk.x - p0.x);

        // Case 1 : When both points are inside
        if (i_pos < 0 && k_pos < 0)
        {
            //Only second point is added
            new_points.push_back(vk);
        }
        // Case 2: When only first point is outside
        else if (i_pos >= 0 && k_pos < 0)
        {
            // Point of intersection with edge
            // and the second point is added
            KVector2 intersect;
            KVectorUtil::LineSegementsIntersect(p0, p1, vi, vk, intersect);
            new_points.push_back(intersect);
            new_points.push_back(vk);
        }
        // Case 3: When only second point is outside
        else if (i_pos < 0 && k_pos >= 0)
        {
            //Only point of intersection with edge is added
            KVector2 intersect;
            KVectorUtil::LineSegementsIntersect(p0, p1, vi, vk, intersect);
            new_points.push_back(intersect);
        }
        // Case 4: When both points are outside
        else
        {
            //No points are added
        }
    }
}

void KVectorUtil::ScanDrawPolygon(HDC hdc, std::vector<KVector2>& vertices, COLORREF color_)
{
    KRgb rgbColor(color_);
    Gdiplus::Color color = rgbColor.GetGdiColor();
    const int numVertices = (int)vertices.size();
    for (int i = 0; i < numVertices-1; ++i)
    {
        ScanLine(hdc, (int)vertices[i].x, (int)vertices[i].y
            , (int)vertices[i + 1].x, (int)vertices[i + 1].y, color );
    }
    ScanLine(hdc, (int)vertices[numVertices-1].x, (int)vertices[numVertices - 1].y
        , (int)vertices[0].x, (int)vertices[0].y, color );
}

void KVectorUtil::ScanDrawFilledPolygon(HDC hdc, std::vector<KVector2>& vertices, KRgb color)
{
    const int numVertices = (int)vertices.size();
    for (int i = 1; i < numVertices - 1; ++i)
    {
        KVector2 p0 = vertices[i];
        KVector2 p1 = vertices[i+1];
        // triangle center, p0, p1
        FillTriangle(hdc, vertices[0].x, vertices[0].y, color, p0.x, p0.y, color, p1.x, p1.y, color);
    }
    //KVector2 p0 = vertices[numVertices - 1];
    //KVector2 p1 = vertices[0];
    //FillTriangle(hdc, center.x, center.y, color, p0.x, p0.y, color, p1.x, p1.y, color);
}

int KVectorUtil::GetDirection(const KVector2& a, const KVector2& b, const KVector2& c)
{
    double val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (KVector2::IsZero(val))
        return 0;    //colinear
    else if (val > 0)
        return 1;    //clockwise direction

    return 2; //counter-clockwise direction
}
