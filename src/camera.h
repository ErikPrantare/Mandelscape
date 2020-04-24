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
           float const& clippingPlaneNear,
           float const& clippingPlaneFar,
           float const& FOV,
           glm::vec3 const& startPosition = {1.0f, 0.0f, 1.0f},
           glm::vec3 const& worldUp       = {0.0f, 1.0f, 0.0f});

    void
    move(glm::vec3 const& movement);

    glm::mat4
    projection() const;

    glm::mat4
    cameraSpace() const;

    glm::vec3 const&
    position() const;

    void
    lookAt(glm::vec3 const&);

    void
    setPosition(glm::vec3 const&);

    void
    setScale(float const scale);

    void
    setCameraHeight(float const meshHeight);

private:
    glm::mat4
    lookAtMatrix() const;

    float m_width, m_height;
    float m_zNear, m_zFar;
    float m_FOV;
    glm::vec3 m_pos;
    glm::vec3 m_up;
    glm::vec3 m_lookAt;
    float m_worldScale;
};

#endif
