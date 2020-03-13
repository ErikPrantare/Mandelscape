#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

#include <vector>
#include <complex>

#include "math3d.h"
#include "terrainMeshLoader.h"

class
Terrain final
{
public:
    static constexpr int granularity = 100;
    static constexpr int iterations = 100;

    Terrain();
    ~Terrain();

    static double
    heightAt(const std::complex<double>& c);

    void 
    populateBuffers();

    void
    updateBuffers(float x, float y, float scale);

    void
    render();

    GLuint VBO;
    GLuint IBO;
private:
    float m_x = 0.0f, 
          m_z = 0.0f, 
          m_scale = 1.0f;

    TerrainMeshLoader m_meshLoader;

    void
    createVertexBuffer();

    void
    createIndexBuffer();
};

#endif
