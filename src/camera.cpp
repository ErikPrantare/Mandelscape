#include <cmath>
#include <iostream>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

Camera::Camera() : Camera(1366, 768, 0.1f, 10'000'000.0f, glm::pi<double>())
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
            m_forward({0.0f, -1.0f, 1.0f}),
            m_worldScale(1.0f)
{}

glm::mat4
Camera::lookAtMatrix() const
{
    glm::vec3 const forward = normalize(m_forward);
    glm::vec3 const right   = normalize(cross(m_up, forward));
    glm::vec3 const up      = normalize(cross(forward, right));

    return glm::lookAt(m_pos, m_pos + forward, up);
}

void
Camera::move(glm::vec3 const& movement)
{
    glm::vec3 const adjustedMovement = m_worldScale * movement;

    m_pos += adjustedMovement.y * m_up;
    m_pos += adjustedMovement.z
             * normalize(glm::vec3(m_forward.x, 0.0f, m_forward.z));

    glm::vec3 const right = cross(m_forward, m_up);
    m_pos += adjustedMovement.x * normalize(glm::vec3(right.x, 0.0f, right.z));
}

glm::mat4
Camera::projection() const
{
    float const aspectRatio = m_width / m_height;

    return glm::perspective(m_FOV, aspectRatio, 0.1f, 100.f);
}

glm::mat4
Camera::cameraSpace() const
{
    return glm::scale(glm::mat4(1.0f), 1.0f / glm::vec3(m_worldScale))
           * lookAtMatrix();
}

glm::vec3 const&
Camera::position() const
{
    return m_pos;
}

void
Camera::lookAt(glm::vec3 const& direction)
{
    m_forward = normalize(direction);
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
