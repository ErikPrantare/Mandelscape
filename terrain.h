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
    static double
    heightAt(const std::complex<double>& c);

    void
    updateBuffers(float x, float y, float scale);

    void
    render();

private:
    TerrainMeshLoader m_meshLoader;
};

#endif
