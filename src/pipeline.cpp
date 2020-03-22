#include "pipeline.h"
#include "math3d.h"

Pipeline::Pipeline() :
        m_position(Vector3f(0.0, 0.0, 0.0)),
        m_rotation(Vector3f(0.0, 0.0, 0.0)),
        m_scale(Vector3f(1.0, 1.0, 1.0))
{
}

void
Pipeline::setPos(float x, float y, float z)
{
    m_position = Vector3f(x, y, z);
}

void
Pipeline::scale(float scaleX, float scaleY, float scaleZ)
{
    m_scale = Vector3f(scaleX, scaleY, scaleZ);
}

void
Pipeline::rotate(float rotationX, float rotationY, float rotationZ)
{
    m_rotation = Vector3f(rotationX, rotationY, rotationZ);
}

Matrix4f
Pipeline::getTrans() const
{
    return m_camera.projectionTransformation() * translationMatrix(m_position)
           * rotationMatrix(m_rotation) * scaleMatrix(m_scale);
}
