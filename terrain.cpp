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
        float xPos = (x/(granularity/16.0f)-8.0f)/m_scale + m_x;
        float zPos = (z/(granularity/16.0f)-8.0f)/m_scale + m_z;
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
    
    //main cardioid check
    float q = pow(c.real()-0.25f, 2.0f) + c.imag()*c.imag();
    if(q*(q+(c.real()-0.25f)) < 0.25f*c.imag()*c.imag()) {
        return 0.0f;
    }

    //period-2 bulb check
    if((c.real()+1.0f)*c.real()+1.0f + c.imag()*c.imag() < 0.625f) {
        return 0.0f;
    }

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
    std::vector<int> terrainIndices = getMeshIndices();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
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
