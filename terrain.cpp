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
Terrain::updateBuffers(float x, float y, float scale)
{
    m_x = x;
    m_z = y;
    m_scale = scale;

    createVertexBuffer();
}


void
Terrain::populateBuffers()
{
    createVertexBuffer();
    createIndexBuffer();
}


void
Terrain::createIndexBuffer()
{
    std::vector<GLuint> terrainIndices = m_meshLoader.getMeshIndices();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshLoader.m_IBO);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            terrainIndices.size()*sizeof(GLuint),
            terrainIndices.data(),
            GL_STATIC_DRAW);
}


void
Terrain::createVertexBuffer()
{
    m_meshLoader.updateMeshPoints(m_x, m_z, m_scale);
}


void
Terrain::render()
{
    m_meshLoader.render();
}
