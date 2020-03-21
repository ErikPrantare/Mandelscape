#include "math3d.h"

#include <algorithm>
#include <cmath>

Matrix4f::Matrix4f() : m{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}
{
}

Matrix4f::Matrix4f(float _m[4][4])
{
    std::copy(&_m[0][0], &_m[0][0] + 4 * 4, &m[0][0]);
}

Matrix4f operator*(const Matrix4f& a, const Matrix4f& b)
{
    float res[4][4];

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j) {
            res[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j]
                        + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }

    return Matrix4f(res);
}

Vector3f operator*(float a, const Vector3f& v)
{
    return Vector3f(a * v.x, a * v.y, a * v.z);
}

Vector3f operator*(const Matrix4f& m, const Vector3f& v)
{
    return Vector3f(
        m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3],
        m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[0][3],
        m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[0][3]);
}

Vector3f
operator+(const Vector3f& a, const Vector3f& b)
{
    return Vector3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3f
operator-(const Vector3f& a)
{
    return Vector3f(-a.x, -a.y, -a.z);
}

Vector3f
operator-(const Vector3f& a, const Vector3f& b)
{
    return a + (-b);
}

Vector3f&
operator+=(Vector3f& a, const Vector3f& b)
{
    a = a + b;
    return a;
}

float
dot(const Vector3f& a, const Vector3f& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3f
cross(const Vector3f& a, const Vector3f& b)
{
    return Vector3f(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

Vector3f
normalize(const Vector3f& x)
{
    return (1 / std::sqrt(dot(x, x))) * x;
}

Matrix4f
translationMatrix(const Vector3f& translation)
{
    float m[4][4] = {1.0f,
                     0.0f,
                     0.0f,
                     translation.x,
                     0.0f,
                     1.0f,
                     0.0f,
                     translation.y,
                     0.0f,
                     0.0f,
                     1.0f,
                     translation.z,
                     0.0f,
                     0.0f,
                     0.0f,
                     1.0f};

    return Matrix4f(m);
}

Matrix4f
scaleMatrix(const Vector3f& scale)
{
    float m[4][4] = {scale.x,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     scale.y,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     scale.z,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     1.0f};

    return Matrix4f(m);
}

Matrix4f
rotationMatrix(const Vector3f& rotation)
{
    float mx[4][4] = {1.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      std::cos(rotation.x),
                      -std::sin(rotation.x),
                      0.0f,
                      0.0f,
                      std::sin(rotation.x),
                      std::cos(rotation.x),
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f};

    float my[4][4] = {std::cos(rotation.y),
                      0.0f,
                      -std::sin(rotation.y),
                      0.0f,
                      0.0f,
                      1.0f,
                      0.0f,
                      0.0f,
                      std::sin(rotation.y),
                      0.0f,
                      std::cos(rotation.y),
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f};

    float mz[4][4] = {std::cos(rotation.z),
                      -std::sin(rotation.z),
                      0.0f,
                      0.0f,
                      std::sin(rotation.z),
                      std::cos(rotation.z),
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f};

    return Matrix4f(mz) * Matrix4f(my) * Matrix4f(mx);
}

float
toRadians(float x)
{
    return x * pi / 180;
}

float
toDegrees(float x)
{
    return x / pi * 180;
}
