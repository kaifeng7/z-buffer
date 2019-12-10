#ifndef __VECTOR_H
#define __VECTOR_H
#include <omp.h>
#include <cmath>
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAX_POLYGON_Y 800
#define MAX_POLYGON_X 1000
#define EPS (1e-6)

struct Color
{
    double r;
    double g;
    double b;
    Color() {}
    Color(double x_, double y_, double z_) : r(x_), g(y_), b(z_) {}

    Color &operator=(const Color &a)
    {
        r = a.r;
        g = a.g;
        b = a.b;
        return *this;
    }
};

class Vector3D
{
  public:
    double x;
    double y;
    double z;
    Vector3D() {}
    Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    Vector3D(const Vector3D &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    inline Vector3D &operator=(const Vector3D &a)
    {
        x = a.x;
        y = a.y;
        z = a.z;
        return *this;
    }
    inline Vector3D &operator+=(const Vector3D &a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }
    friend inline Vector3D operator+(const Vector3D &a, const Vector3D &b)
    {
        Vector3D temp;
        temp.x = a.x + b.x;
        temp.y = a.y + b.y;
        temp.z = a.z + b.z;
        return temp;
    }
    friend inline Vector3D operator-(const Vector3D &a, const Vector3D &b)
    {
        Vector3D temp;
        temp.x = a.x - b.x;
        temp.y = a.y - b.y;
        temp.z = a.z - b.z;
        return temp;
    }

        friend inline Vector3D operator/(const Vector3D &a, const double &b)
    {
        Vector3D temp;
        temp.x = a.x / b;
        temp.y = a.y / b;
        temp.z = a.z / b;
        return temp;
    }
};

static bool isEquald(double a, double b)
{
    if (fabs(a - b) < EPS)
    {
        return true;
    }
    return false;
};

#endif