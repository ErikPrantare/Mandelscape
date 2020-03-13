#ifndef MANDELLANDSCAPE_TERRAINMESHLOADER_H
#define MANDELLANDSCAPE_TERRAINMESHLOADER_H

#include <GL/glew.h>

#include "math3d.h"

class
TerrainMeshLoader
{
public:
    TerrainMeshLoader() = default;

    std::vector<Vector3f>
    getMeshPoints(double, double, double);

    std::vector<GLuint>
    getMeshIndices();

    static double
    heightAt(const std::complex<double>&);
private:
    static constexpr int granularity = 100;
    static constexpr int iterations = 100;
};


#endif
