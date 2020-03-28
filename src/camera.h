#ifndef MANDELLANDSCAPE_CAMERA_H
#define MANDELLANDSCAPE_CAMERA_H
#include <tuple>

#include "math3d.h"

class Camera {
public:
    Camera();

    Camera(double const& Xdimension,
           double const& Ydimension,
           float const& clippingPlaneNear,
           float const& clippingPlaneFar,
           float const& FOV,
           Vector3f const& startPosition = {1.0f, 0.0f, 1.0f},
           Vector3f const& worldUp       = {0.0f, 1.0f, 0.0f});

    Matrix4f
    projection() const;

    Matrix4f
    cameraSpace() const;

    const Vector3f&
    position() const;

    void position(Vector3f);

    void
    lookAt(Vector3f direction);

    void
    setScale(float scale);

    void
    move(const Vector3f& movement);

    void
    setCameraHeight(float meshHeight);

private:
    Matrix4f
    uvn() const;

    float m_width, m_height;
    float m_zNear, m_zFar;
    float m_FOV;
    Vector3f m_pos;
    Vector3f m_up;
    Vector3f m_lookAt;
    float m_worldScale;
};

#endif
