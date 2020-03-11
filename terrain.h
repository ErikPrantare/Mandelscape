#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

#include <vector>
#include <complex>

#include "math3d.h"

class
Terrain
{
public:
    std::vector<Vector3f>
    getMeshPoints();

    std::vector<int>
    getMeshIndices();

    static constexpr int granularity = 1000;
    static constexpr int iterations = 100;

    float
    iters(const std::complex<float>& c);
    
    GLuint terrainVBO;
    GLuint terrainIBO;

    void
    createVertexBuffer();

    void
    createIndexBuffer();
};

#endif
