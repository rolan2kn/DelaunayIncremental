//
// Created by rolo on 30-08-18.
//

#ifndef PMTOOL_MATRIX_H
#define PMTOOL_MATRIX_H


#include "Vector3D.h"

class Matrix {
private:
    float m[4][4];
public:
    Matrix();
    Matrix(const int i);
    Matrix(const float m[16]);
    Matrix(const float m[4][4]);
    Matrix(float m11,float m12,float m13,
           float m21,float m22,float m23,
           float m31,float m32,float m33);
    Matrix(float m11,float m12,float m13, float m14,
           float m21,float m22,float m23, float m24,
           float m31,float m32,float m33, float m34,
           float m41,float m42,float m43, float m44);
    Matrix(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3);
    float det(const Matrix &M);
    static float det(float m11);
    static float det(float m11, float m12,
                     float m21, float m22);
    static float det(float m11, float m12, float m13,
                     float m21, float m22, float m23,
                     float m31, float m32, float m33);
    static float det(float m11, float m12, float m13, float m14,
                     float m21, float m22, float m23, float m24,
                     float m31, float m32, float m33, float m34,
                     float m41, float m42, float m43, float m44);
    static float det(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3);

    inline float& operator()(int i, int j);
    inline const float& operator()(int i, int j) const;

    inline float * operator[](int i);

    inline const float* operator[](int i) const;

    friend Matrix& operator*(float value, Matrix &m);   // MAtrix multiplication by float

};


#endif //PMTOOL_MATRIX_H
