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

        ps.emplace_back(xPos, iters({xPos, zPos}), zPos);
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


double
Terrain::iters(const std::complex<double>& c)
{
    std::complex<double> z(0.0d, 0.0d); 
    std::complex<double> dz(0.0d, 0.0d);
    
    //main cardioid check
    double q = pow(c.real()-0.25d, 2.0d) + c.imag()*c.imag();
    if(q*(q+(c.real()-0.25d)) < 0.25d*c.imag()*c.imag()) {
        return 0.0d;
    }

    //period-2 bulb check
    if((c.real()+1.0d)*c.real()+1.0d + c.imag()*c.imag() < 0.625d) {
        return 0.0d;
    }

    int i = 0;
    for(; i < iterations; ++i) {
        dz = 2.0d*z*dz + 1.0d;
        z = z*z + c;

        if(std::abs(z) > 256) {
            double r = std::abs(z);
            double dr = std::abs(dz);
            double de = 2.0d*r*std::log(r)/dr; //estimated distance from set
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
