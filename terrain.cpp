#include <chrono>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "terrain.h"
#include "math3d.h"


Terrain::Terrain()
{
    GLuint VBO2;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &IBO);

    std::vector<Vector3f> terrainMesh = 
        m_meshLoader.updateMeshPoints(m_x, m_z, m_scale);
    
    m_meshLoader.m_VBO = VBO;
    glBindBuffer(GL_ARRAY_BUFFER, m_meshLoader.m_VBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            terrainMesh.size()*sizeof(Vector3f),
            terrainMesh.data(),
            GL_STATIC_DRAW);

    m_meshLoader.m_loadingVBO = VBO2;
    glBindBuffer(GL_ARRAY_BUFFER, m_meshLoader.m_loadingVBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            terrainMesh.size()*sizeof(Vector3f),
            terrainMesh.data(),
            GL_STATIC_DRAW);

}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}


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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
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
    int vertexCount = std::pow((TerrainMeshLoader::granularity-1), 2)*3*2;
    glBindBuffer(GL_ARRAY_BUFFER, m_meshLoader.m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}
