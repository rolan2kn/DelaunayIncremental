//
// Created by rolo on 30-08-18.
//

#include "Matrix.h"


Matrix::Matrix()
{
}

Matrix::Matrix(const int i)
{
    const float s = (float)i;

    Matrix &M = *this;

    M(1, 1) = s; M(1, 2) = 0; M(1, 3) = 0; M(1, 4) = 0;
    M(2, 1) = 0; M(2, 2) = s; M(2, 3) = 0; M(2, 4) = 0;
    M(3, 1) = 0; M(3, 2) = 0; M(3, 3) = s; M(3, 4) = 0;
    M(4, 1) = 0; M(4, 2) = 0; M(4, 3) = 0; M(4, 4) = s;
}

Matrix::Matrix(const float m[16])
{
    Matrix &M = *this;

    M(1, 1) = m[0];  M(1, 2) = m[1];  M(1, 3) = m[2];  M(1, 4) = m[3];
    M(2, 1) = m[4];  M(2, 2) = m[5];  M(2, 3) = m[6];  M(2, 4) = m[7];
    M(3, 1) = m[8];  M(3, 2) = m[8];  M(3, 3) = m[10]; M(3, 4) = m[11];
    M(4, 1) = m[12]; M(4, 2) = m[13]; M(4, 3) = m[14]; M(4, 4) = m[15];
}

Matrix::Matrix(const float m[4][4])
{
    Matrix &M = *this;

    M[0][0] = m[0][0];  M[0][1] = m[0][1];  M[0][2] = m[0][2];  M[0][3] = m[0][3];
    M[1][0] = m[1][0];  M[1][1] = m[1][1];  M[1][2] = m[1][2];  M[1][3] = m[1][3];
    M[2][0] = m[2][0];  M[2][1] = m[2][1];  M[2][2] = m[2][2];  M[2][3] = m[2][3];
    M[3][0] = m[3][0];  M[3][1] = m[3][1];  M[3][2] = m[3][2];  M[3][3] = m[3][3];
}

Matrix::Matrix(float m11, float m12, float m13,
                      float m21, float m22, float m23,
                      float m31, float m32, float m33)
{
    Matrix &M = *this;

    M(1, 1) = m11; M(1, 2) = m12; M(1, 3) = m13; M(1, 4) = 0;
    M(2, 1) = m21; M(2, 2) = m22; M(2, 3) = m23; M(2, 4) = 0;
    M(3, 1) = m31; M(3, 2) = m32; M(3, 3) = m33; M(3, 4) = 0;
    M(4, 1) = 0;   M(4, 2) = 0;   M(4, 3) = 0;   M(4, 4) = 1;
}

Matrix::Matrix(float m11, float m12, float m13, float m14,
                      float m21, float m22, float m23, float m24,
                      float m31, float m32, float m33, float m34,
                      float m41, float m42, float m43, float m44)
{
    Matrix &M = *this;

    M(1, 1) = m11; M(1, 2) = m12; M(1, 3) = m13; M(1, 4) = m14;
    M(2, 1) = m21; M(2, 2) = m22; M(2, 3) = m23; M(2, 4) = m24;
    M(3, 1) = m31; M(3, 2) = m32; M(3, 3) = m33; M(3, 4) = m34;
    M(4, 1) = m41; M(4, 2) = m42; M(4, 3) = m43; M(4, 4) = m44;
}

Matrix::Matrix(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3)
{
    Matrix &M = *this;

    M(1, 1) = v1.x; M(1, 2) = v2.x; M(1, 3) = v3.x; M(1, 4) = 0;
    M(2, 1) = v1.y; M(2, 2) = v2.y; M(2, 3) = v3.y; M(2, 4) = 0;
    M(3, 1) = v1.z; M(3, 2) = v2.z; M(3, 3) = v3.z; M(3, 4) = 0;
    M(4, 1) = 0;    M(4, 2) = 0;    M(4, 3) = 0;    M(4, 4) = 1;
}

inline float *Matrix::operator[](int i)
{
    return m[i];
}

inline const float *Matrix::operator[](int i) const
{
    return m[i];
}

inline float &Matrix::operator()(int i, int j)
{
    return m[i - 1][j - 1];
}

inline const float &Matrix::operator()(int i, int j) const
{
    return m[i - 1][j - 1];
}

float Matrix::det(const  Matrix &M)
{
    float M3344 = M(3, 3) * M(4, 4) - M(4, 3) * M(3, 4);
    float M2344 = M(2, 3) * M(4, 4) - M(4, 3) * M(2, 4);
    float M2334 = M(2, 3) * M(3, 4) - M(3, 3) * M(2, 4);
    float M1344 = M(1, 3) * M(4, 4) - M(4, 3) * M(1, 4);
    float M1334 = M(1, 3) * M(3, 4) - M(3, 3) * M(1, 4);
    float M1324 = M(1, 3) * M(2, 4) - M(2, 3) * M(1, 4);

    return M(1, 1) * (M(2, 2) * M3344 - M(3, 2) * M2344 + M(4, 2) * M2334) -
           M(2, 1) * (M(1, 2) * M3344 - M(3, 2) * M1344 + M(4, 2) * M1334) +
           M(3, 1) * (M(1, 2) * M2344 - M(2, 2) * M1344 + M(4, 2) * M1324) -
           M(4, 1) * (M(1, 2) * M2334 - M(2, 2) * M1334 + M(3, 2) * M1324);
}

float Matrix::det(float m11)
{
    return m11;
}

float Matrix::det(float m11, float m12,
                  float m21, float m22)
{
    return m11 * m22 - m12 * m21;
}

float Matrix::det(float m11, float m12, float m13,
                  float m21, float m22, float m23,
                  float m31, float m32, float m33)
{
    return m11 * (m22 * m33 - m32 * m23) -
           m21 * (m12 * m33 - m32 * m13) +
           m31 * (m12 * m23 - m22 * m13);
}

float Matrix::det(float m11, float m12, float m13, float m14,
                  float m21, float m22, float m23, float m24,
                  float m31, float m32, float m33, float m34,
                  float m41, float m42, float m43, float m44)
{
    float M3344 = m33 * m44 - m43 * m34;
    float M2344 = m23 * m44 - m43 * m24;
    float M2334 = m23 * m34 - m33 * m24;
    float M1344 = m13 * m44 - m43 * m14;
    float M1334 = m13 * m34 - m33 * m14;
    float M1324 = m13 * m24 - m23 * m14;

    return m11 * (m22 * M3344 - m32 * M2344 + m42 * M2334) -
           m21 * (m12 * M3344 - m32 * M1344 + m42 * M1334) +
           m31 * (m12 * M2344 - m22 * M1344 + m42 * M1324) -
           m41 * (m12 * M2334 - m22 * M1334 + m32 * M1324);
}

float Matrix::det(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3)
{
    return v1.dot(v2.cross(v3));
}

Matrix& operator*(float s, Matrix &M)
{

    M(1, 1) *= s; M(1, 2) *= s; M(1, 3) *= s; M(1, 4) *= s;
    M(2, 1) *= s; M(2, 2) *= s; M(2, 3) *= s; M(2, 4) *= s;
    M(3, 1) *= s; M(3, 2) *= s; M(3, 3) *= s; M(3, 4) *= s;
    M(4, 1) *= s; M(4, 2) *= s; M(4, 3) *= s; M(4, 4) *= s;

    return M;

}

