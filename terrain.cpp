#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "terrain.h"
#include "math3d.h"


Terrain::Terrain()
{
}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &terrainVBO);
    glDeleteBuffers(1, &terrainIBO);
}

std::vector<Vector3f>
Terrain::getMeshPoints()
{
    std::vector<Vector3f> ps;
    for(int x = 0; x < granularity; x++)
    for(int z = 0; z < granularity; z++) {
        float xPos = x/(granularity/4.0f)-2.0f;
        float zPos = z/(granularity/4.0f)-2.0f;
        ps.emplace_back(
                xPos,
                iters(std::complex<float>(xPos, zPos)), 
                zPos);
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
            indices.push_back(z+(x+1)*granularity);
            indices.push_back((z+1)+x*granularity);
            indices.push_back((z+1)+(x+1)*granularity);
        }

    return indices;
}


float
Terrain::iters(const std::complex<float>& c)
{
    std::complex<float> z(0.0f, 0.0f); 
    std::complex<float> dz(0.0f, 0.0f);

    int i = 0;
    for(; i < iterations; ++i) {
        dz = 2.0f*z*dz + 1.0f;
        z = z*z + c;

        if(std::abs(z) > 256) {
            float r = std::abs(z);
            float dr = std::abs(dz);
            float de = 2.0f*r*std::log(r)/dr; //estimated distance from set
            return de;
        }
    }

    return 0;
}


void
Terrain::createIndexBuffer()
{
    std::vector<int> terrainIndices = getMeshIndices();

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            terrainIndices.size()*sizeof(int),
            terrainIndices.data(),
            GL_STATIC_DRAW);
}


void
Terrain::createVertexBuffer()
{
    std::vector<Vector3f> terrainMesh = getMeshPoints();

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            terrainMesh.size()*sizeof(Vector3f),
            terrainMesh.data(),
            GL_STATIC_DRAW);
}
