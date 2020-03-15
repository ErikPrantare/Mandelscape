#include <chrono>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "terrain.h"
#include "math3d.h"


double
Terrain::heightAt(const std::complex<double>& c)
{
    return TerrainMeshLoader::heightAt(c);
}


void
Terrain::updateBuffers(float x, float z, float scale)
{
    m_meshLoader.updateMeshPoints(x, z, scale);
}


void
Terrain::render()
{
    m_meshLoader.render();
}
