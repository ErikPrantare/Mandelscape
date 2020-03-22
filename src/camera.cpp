#include <cmath>
#include <iostream>
#include <tuple>

#include "camera.h"
#include "math3d.h"

Camera::Camera() : Camera(1366, 768, 0.1f, 10'000'000.0f, pi) {
}

Camera::Camera(
    double const& Xdimension,
    double const& Ydimension,
    float const& clippingPlaneNear,
    float const& clippingPlaneFar,
    float const& FOV,
    Vector3f const& startPosition,
    Vector3f const& worldUp) :
    m_width(Xdimension),
    m_height(Ydimension),
    m_zNear(clippingPlaneNear),
    m_zFar(clippingPlaneFar),
    m_FOV(FOV),
    m_pos(startPosition),
    m_up(worldUp),
    m_lookAt({0.0f, 0.0f, 1.0f}),
    m_worldScale(1.0f) {
}

void
Camera::move(const Vector3f& velocity, const float& deltaSeconds) {
    Vector3f const adjustedVelocity = m_worldScale * deltaSeconds * velocity;

    m_pos += adjustedVelocity.y * m_up;
    m_pos +=
        adjustedVelocity.z * normalize(Vector3f(m_lookAt.x, 0.0f, m_lookAt.z));
    Vector3f right = cross(m_up, m_lookAt);
    m_pos += adjustedVelocity.x * normalize(Vector3f(right.x, 0.0f, right.z));
}

Matrix4f
Camera::uvn() const {
    const Vector3f N = normalize(m_lookAt);
    const Vector3f U = normalize(cross(m_up, N));
    const Vector3f V = normalize(cross(N, U));

    float m[4][4] = {{U.x, U.y, U.z, 0.0f},
                     {V.x, V.y, V.z, 0.0f},
                     {N.x, N.y, N.z, 0.0f},
                     {0.0f, 0.0f, 0.0f, 1.0f}};
    return Matrix4f(m);
}

Matrix4f
Camera::projectionTransformation() const {
    const float ar         = m_width / m_height;
    const float zRange     = m_zNear - m_zFar;
    const float tanHalfFOV = std::tan(m_FOV / 2.0);

    float m[4][4] = {{1.0f / (tanHalfFOV * ar), 0.0, 0.0, 0.0},
                     {0.0, 1.0f / tanHalfFOV, 0.0, 0.0},
                     {0.0,
                      0.0,
                      (-m_zNear - m_zFar) / zRange,
                      2.0f * m_zNear * m_zFar / zRange},
                     {0.0, 0.0, 1.0, 0.0}};

    Matrix4f translation = translationMatrix({-m_pos.x, -m_pos.y, -m_pos.z});

    return Matrix4f(m)
           * scaleMatrix({1 / m_worldScale, 1 / m_worldScale, 1 / m_worldScale})
           * uvn() * translation;
}

void
Camera::lookAt(Vector3f direction) {
    m_lookAt = normalize(direction);
}

void
Camera::setScale(float scale) {
    m_worldScale = scale;
}

void
Camera::setCameraHeight(float meshHeight) {
    m_pos.y = m_worldScale + meshHeight;
}

const Vector3f&
Camera::position() const {
    return m_pos;
}
