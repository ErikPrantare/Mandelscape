#ifndef MANDELLANDSCAPE_PIPELINE_H
#define MANDELLANDSCAPE_PIPELINE_H

#include "math3d.h"
#include "camera.h"

class
Pipeline
{
public:
    Pipeline();
    void scale(float scaleX, float scaleY, float scaleZ);
    void scale(const Vector3f& scale);

    void setPos(float x, float y, float z);
    void setPos(const Vector3f& pos);

    void rotate(float x, float y, float z);
    void rotate(const Vector3f& rotation);

    void setCamera(Camera camera) {
        m_camera = camera;
    };

    Matrix4f getTrans() const;
private:
    Vector3f m_position, m_rotation, m_scale;
    Camera m_camera;
    Matrix4f m_mat;
};

#endif
