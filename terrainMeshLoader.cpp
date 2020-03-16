#include <vector>
#include <cmath>
#include <complex>
#include <thread>
#include <memory>

#include <GL/glew.h>

#include "terrainMeshLoader.h"

void
loadMesh(double, double, double, std::vector<Vector3f>*);

TerrainMeshLoader::TerrainMeshLoader() :
  m_x{ 0.0 }, m_z{ 0.0 }, m_scale{ 1.0 },
  m_currentMeshPoints{ std::make_shared<std::vector<Vector3f>>() },
  m_loadingMeshPoints{ std::make_shared<std::vector<Vector3f>>() }
{
    loadMesh(m_x, m_z, m_scale, m_currentMeshPoints.get());
    loadMesh(m_x, m_z, m_scale, m_loadingMeshPoints.get());

    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);
    glGenBuffers(1, &m_loadingVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            m_currentMeshPoints->size()*sizeof(Vector3f),
            m_currentMeshPoints->data(),
            GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_loadingVBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            m_loadingMeshPoints->size()*sizeof(Vector3f),
            m_loadingMeshPoints->data(),
            GL_STATIC_DRAW);

    auto meshIndices = getMeshIndices();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            meshIndices.size()*sizeof(meshIndices[0]),
            meshIndices.data(),
            GL_STATIC_DRAW);

    startLoading();
}


TerrainMeshLoader::~TerrainMeshLoader()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteBuffers(1, &m_loadingVBO);
}


void
TerrainMeshLoader::startLoading()
{
    m_loadingProcess = std::async(
                std::launch::async, 
                loadMesh, m_x, m_z, m_scale, m_loadingMeshPoints.get());
}


void
loadMesh(double _x, double _z, double _scale, std::vector<Vector3f> *buffer)
{
    constexpr int granularity = TerrainMeshLoader::granularity;
    std::vector<Vector3f> ps;

    if(buffer->size() != granularity*granularity) {
        buffer->resize(granularity*granularity);
    }

    double discScale = std::pow(2.0, int(log2(_scale)));
    double scaleFactor = TerrainMeshLoader::granularity*discScale/32.0;

    double xOffset = int(_x*scaleFactor)/scaleFactor;
    xOffset -= granularity/2.0/scaleFactor;

    double zOffset = int(_z*scaleFactor)/scaleFactor;
    zOffset -= granularity/2.0/scaleFactor;

    for(int x = 0; x < granularity; x++)
      for(int z = 0; z < granularity; z++) {
        double xPos = x/scaleFactor + xOffset;
        double zPos = z/scaleFactor + zOffset;
 
        (*buffer)[x*granularity + z] =
            Vector3f(xPos, TerrainMeshLoader::heightAt({xPos, zPos}), zPos);
    }
}

const std::vector<Vector3f>&
TerrainMeshLoader::updateMeshPoints(double x, double z, double scale)
{
    m_x = x;
    m_z = z;
    m_scale = scale;

    if(isDone(m_loadingProcess)) {
        m_currentMeshPoints.swap(m_loadingMeshPoints);

        startLoading();

        Vector3f *position = 
            m_currentMeshPoints->data() + m_loadIndex;
        
        int chunkSize = 
            std::min(90'000, int(m_currentMeshPoints->size() - m_loadIndex));

        glBindBuffer(GL_ARRAY_BUFFER, m_loadingVBO);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                m_loadIndex*sizeof(Vector3f),
                chunkSize*sizeof(Vector3f),
                position);

        m_loadIndex += chunkSize;
        
        if(m_loadIndex == m_currentMeshPoints->size()) {
            m_loadIndex = 0;
            std::swap(m_VBO, m_loadingVBO);
        }
    }

    return *m_currentMeshPoints;
}


std::vector<GLuint>
TerrainMeshLoader::getMeshIndices()
{
    std::vector<GLuint> meshIndices; 
    meshIndices.reserve(granularity*granularity);

    for(int x = 0; x < granularity-1; x++)
        for(int z = 0; z < granularity-1; z++) {
            meshIndices.push_back(z+x*granularity);
            meshIndices.push_back(z+(x+1)*granularity);
            meshIndices.push_back((z+1)+x*granularity);

            meshIndices.push_back((z+1)+x*granularity);
            meshIndices.push_back(z+(x+1)*granularity);
            meshIndices.push_back((z+1)+(x+1)*granularity);
        }

    return meshIndices;
}


double
TerrainMeshLoader::heightAt(const std::complex<double>& c)
{
    std::complex<double> z(0.0, 0.0); 
    std::complex<double> dz(0.0, 0.0);
    
    //main cardioid check
    double q = pow(c.real()-0.25, 2.0) + c.imag()*c.imag();
    if(q*(q+(c.real()-0.25)) < 0.25*c.imag()*c.imag()) {
        return 0.0;
    }

    //period-2 bulb check
    if((c.real()+1.0)*c.real()+1.0 + c.imag()*c.imag() < 0.625) {
        return 0.0;
    }

    int i = 0;
    for(; i < iterations; ++i) {
        dz = 2.0*z*dz + 1.0;
        z = z*z + c;

        if(std::abs(z) > 256) {
            double r = std::abs(z);
            double dr = std::abs(dz);
            double de = 2.0*r*std::log(r)/dr; //estimated distance from set
            return de;
        }
    }

    return 0.0;
}


void
TerrainMeshLoader::render()
{
    int vertexCount = std::pow((granularity-1), 2)*3*2;
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}
