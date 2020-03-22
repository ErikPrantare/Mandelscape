#ifndef MANDELLANDSCAPE_CAMERA_H
#define MANDELLANDSCAPE_CAMERA_H
#include <tuple>

#include "math3d.h"

class Camera {
public:
    Camera();

    void
    move(const Vector3f& movement);

    Matrix4f
    projectionTransformation() const;

    void
    setClip(float zNear, float zFar)
    {
        m_zNear = zNear;
        m_zFar  = zFar;
    }

    Camera(double const& Xdimension,
           double const& Ydimension,
           float const& clippingPlaneNear,
           float const& clippingPlaneFar,
           float const& FOV,
           Vector3f const& startPosition = {1.0f, 0.0f, 1.0f},
           Vector3f const& worldUp       = {0.0f, 1.0f, 0.0f});

    void
    setDimensions(float width, float height)
    {
        m_width  = width;
        m_height = height;
    }

    void
    setFOV(float FOV)
    {
        m_FOV = FOV;
    }

    const Vector3f&
    position() const;

    void
    lookAt(Vector3f z)
    {
        m_lookAt = normalize(z);
    }

    void
    setUp(Vector3f up)
    {
        m_up = normalize(up);
    }

    void
    setScale(float scale);

    void
    setPos(Vector3f pos)
    {
        m_pos = pos;
    }

    void
    setY(float y)
    {
        m_pos.y = y;
    }

    void
    setCameraHeight(float meshHeight);

    const Vector3f&
    getPos() const
    {
        return m_pos;
    }

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
