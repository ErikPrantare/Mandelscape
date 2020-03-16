#include <cmath>
#include <iostream>

#include "camera.h"
#include "math3d.h"

void
Camera::move(const Vector3f& movement)
{
    m_pos += movement.y * m_up;
    m_pos += movement.z * normalize(Vector3f(m_lookAt.x, 0.0f, m_lookAt.z));
    Vector3f right = cross(m_up, m_lookAt);
    m_pos += movement.x * normalize(Vector3f(right.x, 0.0f, right.z));
}

Matrix4f
Camera::uvn() const
{
    const Vector3f N = normalize(m_lookAt);
    const Vector3f U = normalize(cross(m_up, N));
    const Vector3f V = normalize(cross(N, U));

    float m[4][4] = {
        {U.x, U.y, U.z, 0.0f},
        {V.x, V.y, V.z, 0.0f},
        {N.x, N.y, N.z, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}};
    return Matrix4f(m);
}

Matrix4f 
Camera::projectionTransformation() const
{
    const float ar = m_width/m_height;
    const float zRange = m_zNear - m_zFar;
    const float tanHalfFOV = std::tan(m_FOV/2.0);


    float m[4][4] = {
        {1.0f/(tanHalfFOV*ar), 0.0, 0.0, 0.0},
        {0.0, 1.0f/tanHalfFOV, 0.0, 0.0},
        {0.0, 0.0, (-m_zNear - m_zFar) / zRange, 2.0f*m_zNear*m_zFar/zRange},
        {0.0, 0.0, 1.0, 0.0}};

    Matrix4f translation = translationMatrix({-m_pos.x, -m_pos.y, -m_pos.z});

    return Matrix4f(m) 
        * scaleMatrix({1/m_size, 1/m_size, 1/m_size}) 
        * uvn() 
        * translation;
}
