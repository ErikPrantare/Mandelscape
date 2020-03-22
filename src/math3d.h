#ifndef MANDELLANDSCAPE_MATH3D_H
#define MANDELLANDSCAPE_MATH3D_H

constexpr long double pi = 3.141592653589793238462643383279502884;

struct Vector3f {
    float x, y, z;
    Vector3f() = default;
    Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
    {}
};

struct Matrix4f {
    Matrix4f();
    Matrix4f(float _m[4][4]);
    float m[4][4];
};

Vector3f operator*(float, const Vector3f&);

Matrix4f operator*(const Matrix4f&, const Matrix4f&);

Vector3f operator*(const Matrix4f&, const Vector3f&);

Vector3f
operator+(const Vector3f&, const Vector3f&);

Vector3f
operator-(const Vector3f&);

Vector3f
operator-(const Vector3f&, const Vector3f&);

Vector3f&
operator+=(Vector3f&, const Vector3f&);

float
dot(const Vector3f&, const Vector3f&);

Vector3f
cross(const Vector3f& a, const Vector3f& b);

Vector3f
normalize(const Vector3f&);

Matrix4f
translationMatrix(const Vector3f& translation);

Matrix4f
scaleMatrix(const Vector3f& scale);

Matrix4f
rotationMatrix(const Vector3f& rotation);

float
toRadians(float);

float
toDegrees(float);

#endif
