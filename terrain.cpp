#include <chrono>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "terrain.h"
#include "math3d.h"


Terrain::Terrain()
{
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

std::vector<Vector3f>
Terrain::getMeshPoints()
{
    std::vector<Vector3f> ps;
    for(int x = 0; x < granularity; x++)
    for(int z = 0; z < granularity; z++) {
        double discScale = std::pow(2.0, int(log2(m_scale)));
        double discX = int(m_x*discScale/4)/discScale*4;
        double discZ = int(m_z*discScale/4)/discScale*4;
        double xPos = (x/(granularity/16.0d)-8.0d)/discScale + discX;
        double zPos = (z/(granularity/16.0d)-8.0d)/discScale + discZ;

        ps.emplace_back(xPos, heightAt({xPos, zPos}), zPos);
    }

    return ps;
}


std::vector<int>
Terrain::getMeshIndices()
{
    std::vector<int> indices;
    for(int x = 0; x < granularity-1; x++)
        for(int z = 0; z < granularity-1; z++) {
            indices.push_back(z+x*granularity);
            indices.push_back(z+(x+1)*granularity);
            indices.push_back((z+1)+x*granularity);

            indices.push_back((z+1)+x*granularity);
            indices.push_back(z+(x+1)*granularity);
            indices.push_back((z+1)+(x+1)*granularity);
        }

    return indices;
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

    static auto last = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::now();

    if((now - last).count() > 16558939*60) {
        last = now; 
        createVertexBuffer();
    } 
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
    std::vector<Vector3f> terrainMesh = 
        m_meshLoader.getMeshPoints(m_x, m_z, m_scale);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            terrainMesh.size()*sizeof(Vector3f),
            terrainMesh.data(),
            GL_STATIC_DRAW);
}


void
Terrain::render()
{
    int vertexCount = std::pow((granularity-1), 2)*3*2;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}
