#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

#include <vector>
#include <complex>

#include "math3d.h"

class
Terrain final
{
public:
    static constexpr int granularity = 1000;
    static constexpr int iterations = 100;

    Terrain();
    ~Terrain();

    std::vector<Vector3f>
    getMeshPoints();

    std::vector<int>
    getMeshIndices();

    float
    iters(const std::complex<float>& c);

    void 
    populateBuffers();

    void
    updateBuffers(float x, float y, float scale);

    GLuint terrainVBO;
    GLuint terrainIBO;
private:
    float m_x = 0.0f, 
          m_z = 0.0f, 
          m_scale = 1.0f;


    void
    createVertexBuffer();

    void
    createIndexBuffer();
};

#endif
