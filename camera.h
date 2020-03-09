#ifndef MANDELLANDSCAPE_CAMERA_H
#define MANDELLANDSCAPE_CAMERA_H

#include "math3d.h"

class
Camera
{
public:
    Matrix4f uvn() const;
    Matrix4f projectionTransformation() const;

    void move(const Vector3f& movement);

    void setClip(float zNear, float zFar) {
        m_zNear = zNear;
        m_zFar = zFar;
    }

    void setSize(float width, float height) {
        m_width = width;
        m_height = height;
    }

    void setFOV(float FOV) {
        m_FOV = FOV;
    }

    void lookAt(Vector3f z) {
        m_lookAt = z;
    }

    void setUp(Vector3f up) {
        m_up = up;
    }

    void setPos(Vector3f pos) {
        m_pos = pos;
    }
private:
    float m_zNear, m_zFar;
    float m_width, m_height;
    float m_FOV;
    Vector3f m_up, m_lookAt, m_pos;
};

#endif
