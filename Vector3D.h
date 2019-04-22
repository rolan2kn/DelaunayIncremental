//
// Created by rkindela on 12-04-2018.
//

#ifndef PMTOOL_VECTOR3D_H
#define PMTOOL_VECTOR3D_H

#include "Point.h"
#include <iostream>


class Vector3D {
public:
    int x, y, z;
public:
    Vector3D(int _x = 0, int _y = 0, int _z = 0): x(_x), y(_y), z(_z){};
    Vector3D(const POINT& p0);
    Vector3D(const POINT& p0, const POINT& p1);
    Vector3D(const Vector3D& otherV): x(otherV.x), y(otherV.y), z(otherV.z){};
    Vector3D& sum(const Vector3D& otherV);
    Vector3D& sum(const Vector3D& otherV) const ;
    Vector3D& multi(float value);
    Vector3D& multi(float value) const;
    Vector3D& minus(const Vector3D& otherV);
    Vector3D& cross(const Vector3D& otherV);
    const Vector3D& cross(const Vector3D& otherV) const;
    int detFromCross(const Vector3D& otherV);

    float dot(const Vector3D& otherV);
    float dot(const Vector3D& otherV) const;
    float norm() const;
    float squareNorm() const;

    void normalize();

    float angle(const Vector3D& otherV);
    float angleCos(const Vector3D& otherV);
    POINT& asPoint();

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& p);
    friend std::ostream& operator<<(std::ostream& os, const Vector3D* p);
    friend float operator^(const Vector3D &v, const int n);   // Vector power
    friend Vector3D& operator*(float value, Vector3D &v);   // Vector multiplication
    friend Vector3D& operator/(Vector3D &v, float value);   // Vector division
    friend float operator^(const Vector3D &u, const Vector3D &v);   // Angle between vectors
    friend Vector3D& operator+(const Vector3D &u, const Vector3D &v);   // sum between vectors
};


#endif //PMTOOL_VECTOR3D_H
