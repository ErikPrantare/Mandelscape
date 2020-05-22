#include "camera.h"

#include <cmath>
#include <iostream>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>

#include "config.h"

Camera::Camera() : Camera(1366, 768, 0.1, 10'000'000.0, glm::pi<double>())
{}

Camera::Camera(Config const& config) :
            Camera(config.get<Settings::WindowWidth>(),
                   config.get<Settings::WindowHeight>(),
                   config.get<Settings::ClippingPlaneNear>(),
                   config.get<Settings::ClippingPlaneFar>(),
                   config.get<Settings::FOV>())
{}

Camera::Camera(
        double const Xdimension,
        double const Ydimension,
        double const clippingPlaneNear,
        double const clippingPlaneFar,
        double const FOV,
        glm::dvec3 const& startPosition,
        glm::dvec3 const& worldUp) :
            m_width(Xdimension),
            m_height(Ydimension),
            m_zNear(clippingPlaneNear),
            m_zFar(clippingPlaneFar),
            m_FOV(FOV),
            m_pos(startPosition),
            m_up(worldUp),
            m_lookAt({0.0, -1.0, 1.0}),
            m_worldScale(1.0)
{}

glm::dmat4
Camera::lookAtMatrix() const
{
    glm::dvec3 const forward = normalize(m_lookAt);
    glm::dvec3 const right   = normalize(cross(m_up, forward));
    glm::dvec3 const up      = normalize(cross(forward, right));

    return glm::lookAt(m_pos, m_pos + forward, up);
}

void
Camera::move(glm::dvec3 const& movement)
{
    glm::dvec3 const adjustedMovement = m_worldScale * movement;

    m_pos += adjustedMovement.y * m_up;

    glm::dvec3 const front =
            glm::normalize(m_lookAt - glm::proj(m_lookAt, m_up));
    m_pos += adjustedMovement.z * front;

    glm::dvec3 const right = glm::normalize(cross(front, m_up));
    m_pos += adjustedMovement.x * right;
}

glm::dmat4
Camera::projection() const
{
    double const aspectRatio = m_width / m_height;

    return glm::perspective(m_FOV, aspectRatio, m_zNear, m_zFar);
}

glm::dmat4
Camera::cameraSpace() const
{
    return glm::scale(glm::dmat4(1.0), 1.0 / glm::dvec3(m_worldScale))
           * lookAtMatrix();
}

glm::dvec3
Camera::position() const
{
    return m_pos;
}

void
Camera::lookAt(glm::dvec3 const& direction)
{
    m_lookAt = normalize(direction);
}

void
Camera::setPosition(glm::dvec3 const& pos)
{
    m_pos = pos;
}

void
Camera::setScale(double const scale)
{
    m_worldScale = scale;
}
