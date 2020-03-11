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

    GLuint terrainVBO;
    GLuint terrainIBO;

    Terrain();
    ~Terrain();

    std::vector<Vector3f>
    getMeshPoints();

    std::vector<int>
    getMeshIndices();

    float
    iters(const std::complex<float>& c);

    void
    createVertexBuffer();

    void
    createIndexBuffer();
};

#endif
