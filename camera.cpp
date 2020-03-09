#include <cmath>

#include "camera.h"
#include "math3d.h"

void
Camera::move(const Vector3f& movement)
{
    m_pos += uvn()*movement;
}

Matrix4f
Camera::uvn() const
{
    const Vector3f N = normalize(m_lookAt);
    const Vector3f U = normalize(cross(m_up, N));
    const Vector3f V = normalize(cross(N, U));

    float m[4][4] = {
        U.x, U.y, U.z, 0,
        V.x, V.y, V.z, 0,
        N.x, N.y, N.z, 0,
        0.0f, 0.0f, 0.0f, 1.0f};
    return Matrix4f(m);
}

Matrix4f 
Camera::projectionTransformation() const
{
    const float ar = m_width/m_height;
    const float zRange = m_zNear - m_zFar;
    const float tanHalfFOV = std::tan(m_FOV/2.0);


    float m[4][4] = {
        1.0f/(tanHalfFOV*ar), 0.0, 0.0, 0.0,
        0.0, 1.0f/tanHalfFOV, 0.0, 0.0,
        0.0, 0.0, (-m_zNear - m_zFar) / zRange, 2.0f*m_zNear*m_zFar/zRange,
        0.0, 0.0, 1.0, 0.0};

    float translation[4][4] = {
        1.0f, 0.0f, 0.0f, -m_pos.x,
        0.0f, 1.0f, 0.0f, -m_pos.y,
        0.0f, 0.0f, 1.0f, -m_pos.z,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return Matrix4f(m) * Matrix4f(translation) * uvn();
}
