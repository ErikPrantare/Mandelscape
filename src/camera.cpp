#include "camera.hpp"

#include <cmath>
#include <iostream>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
        glm::dvec3 const position,
        glm::dvec3 const lookAt,
        glm::ivec2 const viewSize,
        double scale) :
            m_viewSize(viewSize),
            m_pos(position),
            m_lookAt(normalize(lookAt)),
            m_worldScale(scale)
{}

auto
Camera::lookAtMatrix() const -> glm::dmat4
{
    glm::dvec3 const forward = normalize(m_lookAt);

    return glm::lookAt(m_pos, m_pos + forward, up);
}

auto
Camera::projection() const -> glm::dmat4
{
    double const aspectRatio = (double)m_viewSize.x / m_viewSize.y;

    return glm::perspective(fov, aspectRatio, zNear, zFar);
}

auto
Camera::cameraSpace() const -> glm::dmat4
{
    return glm::scale(glm::dmat4(1.0), 1.0 / glm::dvec3(m_worldScale))
           * lookAtMatrix();
}
