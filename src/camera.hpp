#ifndef MANDELLANDSCAPE_CAMERA_HPP
#define MANDELLANDSCAPE_CAMERA_HPP
#include <tuple>

#include <glm/glm.hpp>

#include "util.hpp"

class Camera {
public:
    Camera() = delete;

    Camera(glm::dvec3 position,
           glm::dvec3 lookAt,
           glm::ivec2 viewSize,
           double scale);

    [[nodiscard]] auto
    projection() const -> glm::dmat4;

    [[nodiscard]] auto
    cameraSpace() const -> glm::dmat4;

private:
    [[nodiscard]] auto
    lookAtMatrix() const -> glm::dmat4;

    static double constexpr zNear  = 0.01;
    static double constexpr zFar   = 150.0;
    static double constexpr fov    = util::pi / 2;
    static glm::dvec3 constexpr up = {0.0, 1.0, 0.0};

    glm::ivec2 m_viewSize;
    glm::dvec3 m_pos;
    glm::dvec3 m_lookAt;
    double m_worldScale;
};

#endif
