//
// Created by rkindela on 12-04-2018.
//

#include "Vector3D.h"
#include <cmath>


Vector3D::Vector3D(const POINT& p0, const POINT& p1)
{
    this->x = p1.x - p0.x;
    this->y = p1.y - p0.y;
    this->z = p1.z - p0.z;
}

Vector3D::Vector3D(const POINT& p0)
{
    this->x = p0.x;
    this->y = p0.y;
    this->z = p0.z;
}

Vector3D& Vector3D::sum(const Vector3D& otherV)
{
    return *(new Vector3D(x + otherV.x, y + otherV.y, z + otherV.z));
}

Vector3D& Vector3D::sum(const Vector3D& otherV) const
{
    return *(new Vector3D(x + otherV.x, y + otherV.y, z + otherV.z));
}

Vector3D& Vector3D::minus(const Vector3D& otherV)
{
    return *(new Vector3D(x - otherV.x, y - otherV.y, z - otherV.z));
}

float Vector3D::dot(const Vector3D& otherV)
{
    return x*otherV.x + y*otherV.y + z*otherV.z;
}

float Vector3D::dot(const Vector3D& otherV) const
{
    return x*otherV.x + y*otherV.y + z*otherV.z;
}

Vector3D& Vector3D::multi(float value)
{
    return *(new Vector3D((int)x*value, (int)y*value, (int)z*value));
}

Vector3D& Vector3D::multi(float value) const
{
    return *(new Vector3D((int)x*value, (int)y*value, (int)z*value));
}

Vector3D& Vector3D::cross(const Vector3D& otherV)
{
    /*
     * i  j  k
     * ax ay az
     * bx by bz
     *

     *
     * */

    return *(new Vector3D(y*otherV.z - z*otherV.y, z*otherV.x - x*otherV.z, x*otherV.y - y*otherV.x));
}

const Vector3D& Vector3D::cross(const Vector3D& otherV) const
{
    /*
     * i  j  k
     * ax ay az
     * bx by bz
     *

     *
     * */

    return *(new Vector3D(y*otherV.z - z*otherV.y, z*otherV.x - x*otherV.z, x*otherV.y - y*otherV.x));
}

int Vector3D::detFromCross(const Vector3D& otherV)
{
    /*
     * 1  1  1
     * ax ay az
     * bx by bz
     *

     *
     * */

    return (y*otherV.z - z*otherV.y) - (z*otherV.x - x*otherV.z) + (x*otherV.y - y*otherV.x);
}

float Vector3D::norm() const
{
    return sqrt(x*x + y*y + z*z);
}

float Vector3D::squareNorm() const
{
    return x*x + y*y + z*z;
}

void Vector3D::normalize()
{
    float _norm = this->norm();
    _norm = (_norm > 0) ? _norm : 1;

    x /= _norm; y /= _norm; z /= _norm;
}

float Vector3D::angle(const Vector3D& otherV)
{
    float product = this->dot(otherV);
    float norms = this->norm() * otherV.norm();
    if (!norms)
        norms = 1;

    return acos(product / norms);
}

float Vector3D::angleCos(const Vector3D& otherV)
{
    float product = this->dot(otherV);
    float norms = this->norm() * otherV.norm();
    if (!norms)
        norms = 1;

    return product / norms;
}

POINT& Vector3D::asPoint()
{
    return *(new POINT(x, y, z));
}

std::ostream& operator<<(std::ostream& os, const Vector3D& p)
{
    if (p.z == 0)
        os<<"("<<p.x<<", "<<p.y<<")";
    else
        os<<"("<<p.x<<", "<<p.y<<", "<<p.z<<")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector3D* p)
{
    if (p->z == 0)
        os<<"("<<p->x<<", "<<p->y<<")";
    else
        os<<"("<<p->x<<", "<<p->y<<", "<<p->z<<")";
    return os;
}

float operator^(const Vector3D &v, const int n)  // Vector power
{
    switch(n)
    {
        case 2:
            return v.squareNorm();
        default:
            return 1;
    }
}

float operator^(const Vector3D &u, const Vector3D &v)   // Angle between vectors
{
    return u.dot(v);
}

Vector3D& operator*(float value, Vector3D &v)   // Vector multiplication
{
    return v.multi(value);
}

Vector3D& operator/(Vector3D &v, float value)   // Vector multiplication
{
    value = (value != 0) ? value : 1;

    float division = 10000 / value; // to reduce floating division errors
    division /= 10000;

    return v.multi(division);
}

Vector3D& operator+(const Vector3D &u, const Vector3D &v)
{
    return u.sum(v);
}


