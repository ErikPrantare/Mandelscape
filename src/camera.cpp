#include <cmath>
#include <iostream>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "math3d.h"

Camera::Camera() : Camera(1366, 768, 0.1f, 10'000'000.0f, pi)
{}

Camera::Camera(
        double const& Xdimension,
        double const& Ydimension,
        float const& clippingPlaneNear,
        float const& clippingPlaneFar,
        float const& FOV,
        glm::vec3 const& startPosition,
        glm::vec3 const& worldUp) :
            m_width(Xdimension),
            m_height(Ydimension),
            m_zNear(clippingPlaneNear),
            m_zFar(clippingPlaneFar),
            m_FOV(FOV),
            m_pos(startPosition),
            m_up(worldUp),
            m_lookAt({0.0f, 0.0f, 1.0f}),
            m_worldScale(1.0f)
{}

glm::mat4
Camera::uvn() const
{
    glm::vec3 const N = normalize(m_lookAt);
    glm::vec3 const U = normalize(cross(m_up, N));
    glm::vec3 const V = normalize(cross(N, U));

    return glm::transpose(glm::mat4(
            {U.x, U.y, U.z, 0.0f},
            {V.x, V.y, V.z, 0.0f},
            {N.x, N.y, N.z, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}));
}

void
Camera::move(glm::vec3 const& movement)
{
    glm::vec3 const adjustedMovement = m_worldScale * movement;

    m_pos += adjustedMovement.y * m_up;
    m_pos += adjustedMovement.z
             * normalize(glm::vec3(m_lookAt.x, 0.0f, m_lookAt.z));
    glm::vec3 right = cross(m_up, m_lookAt);
    m_pos += adjustedMovement.x * normalize(glm::vec3(right.x, 0.0f, right.z));
}

glm::mat4
Camera::projection() const
{
    float const aspectRatio = m_width / m_height;
    float const zRange      = m_zNear - m_zFar;
    float const tanHalfFOV  = std::tan(m_FOV / 2.0);

    return glm::transpose(glm::mat4(
            {1.0f / (tanHalfFOV * aspectRatio), 0.0, 0.0, 0.0},
            {0.0, 1.0f / tanHalfFOV, 0.0, 0.0},
            {0.0,
             0.0,
             (-m_zNear - m_zFar) / zRange,
             2.0f * m_zNear * m_zFar / zRange},
            {0.0, 0.0, 1.0, 0.0}));
}

glm::mat4
Camera::cameraSpace() const
{
    glm::mat4 translation = translationMatrix(-m_pos);

    return scaleMatrix({1 / m_worldScale, 1 / m_worldScale, 1 / m_worldScale})
           * uvn() * translation;
}

glm::vec3 const&
Camera::position() const
{
    return m_pos;
}

void
Camera::lookAt(glm::vec3 const& direction)
{
    m_lookAt = normalize(direction);
}

void
Camera::setPosition(glm::vec3 const& pos)
{
    m_pos = pos;
}

void
Camera::setScale(float scale)
{
    m_worldScale = scale;
}

void
Camera::setCameraHeight(float meshHeight)
{
    m_pos.y = m_worldScale + meshHeight;
}

