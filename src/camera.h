#ifndef MANDELLANDSCAPE_CAMERA_H
#define MANDELLANDSCAPE_CAMERA_H
#include <tuple>

#include <glm/glm.hpp>

#include "config.h"

class Camera {
public:
    Camera();

    Camera(Config const& config);

    Camera(double const& Xdimension,
           double const& Ydimension,
           double const& clippingPlaneNear,
           double const& clippingPlaneFar,
           double const& FOV,
           glm::dvec3 const& startPosition = {1.0, 0.0, 1.0},
           glm::dvec3 const& worldUp       = {0.0, 1.0, 0.0});

    void
    move(glm::dvec3 const& movement);

    glm::dmat4
    projection() const;

    glm::dmat4
    cameraSpace() const;

    glm::dvec3
    position() const;

    void
    lookAt(glm::dvec3 const&);

    void
    setPosition(glm::dvec3 const&);

    void
    setScale(double scale);

private:
    glm::dmat4
    lookAtMatrix() const;

    double m_width, m_height;
    double m_zNear, m_zFar;
    double m_FOV;
    glm::dvec3 m_pos;
    glm::dvec3 m_up;
    glm::dvec3 m_lookAt;
    double m_worldScale;
};

#endif
