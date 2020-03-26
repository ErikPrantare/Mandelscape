#include <vector>
#include <cmath>
#include <complex>
#include <thread>
#include <memory>

#include <GL/glew.h>

#include "terrain.h"

Terrain::Terrain() :
            m_x{0.0},
            m_z{0.0},
            m_scale{1.0},
            m_currentMeshPoints{std::make_shared<std::vector<Vector3f>>()},
            m_loadingMeshPoints{std::make_shared<std::vector<Vector3f>>()}
{
    loadMesh(m_x, m_z, m_scale, m_currentMeshPoints.get());
    loadMesh(m_x, m_z, m_scale, m_loadingMeshPoints.get());

    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);
    glGenBuffers(1, &m_loadingVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            m_currentMeshPoints->size() * sizeof(Vector3f),
            m_currentMeshPoints->data(),
            GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_loadingVBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            m_loadingMeshPoints->size() * sizeof(Vector3f),
            m_loadingMeshPoints->data(),
            GL_DYNAMIC_DRAW);

    auto meshIndices = generateMeshIndices();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            meshIndices.size() * sizeof(meshIndices[0]),
            meshIndices.data(),
            GL_STATIC_DRAW);

    startLoading();
}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteBuffers(1, &m_loadingVBO);
}

void
Terrain::startLoading()
{
    m_loadingProcess = std::async(
            std::launch::async,
            loadMesh,
            m_x,
            m_z,
            m_scale,
            m_loadingMeshPoints.get());
}

void
Terrain::loadMesh(
        double _x,
        double _z,
        double _scale,
        std::vector<Vector3f>* buffer)
{
    constexpr int nrIndices = granularity * granularity;

    if(buffer->size() != nrIndices) {
        buffer->resize(nrIndices);
    }

    const double discScale   = std::pow(2.0, int(log2(_scale)));
    const double scaleFactor = Terrain::granularity * discScale;

    constexpr int doublingInterval = 40;
    constexpr int exponentOffset   = -5;

    const auto& stepSize = [scaleFactor](int i) {
        return std::pow(
                       2.0,
                       std::abs(i - granularity / 2) / doublingInterval
                               - exponentOffset)
               / scaleFactor;
    };

    const auto& quantized = [](double x, double stepSize) {
        return std::floor(x / stepSize) * stepSize;
    };

    double meshSpan = 0.0;
    for(int i = 0; i < granularity; ++i) {
        meshSpan += stepSize(i);
    }

    double xPos = _x - meshSpan / 2;
    for(int x = 0; x < granularity; ++x) {
        double xQuant = quantized(xPos, stepSize(x));

        double zPos = _z - meshSpan / 2;
        for(int z = 0; z < granularity; ++z) {
            double zQuant                  = quantized(zPos, stepSize(z));
            (*buffer)[x * granularity + z] = Vector3f(
                    xPos - _x,
                    Terrain::heightAt({xQuant, zQuant}),
                    zQuant - _z);

            zPos += stepSize(z);
        }
        xPos += stepSize(x);
    }
}

bool
uploadMeshChunk(
        const std::vector<Vector3f>& sourceMesh,
        const GLuint& destinationVBO,
        const size_t& index,
        const size_t& maxChunkSize)
{
    if(index >= sourceMesh.size()) {
        return true;
    }

    const Vector3f* position = sourceMesh.data() + index;

    int chunkSize = std::min(maxChunkSize, sourceMesh.size() - index);

    glBindBuffer(GL_ARRAY_BUFFER, destinationVBO);
    glBufferSubData(
            GL_ARRAY_BUFFER,
            index * sizeof(Vector3f),
            chunkSize * sizeof(Vector3f),
            position);

    return (index + chunkSize) >= sourceMesh.size();
}

const std::vector<Vector3f>&
Terrain::updateMesh(double x, double z, double scale)
{
    m_x     = x;
    m_z     = z;
    m_scale = scale;

    const bool uploadingDone = uploadMeshChunk(
            *m_currentMeshPoints,
            m_loadingVBO,
            m_loadIndex,
            uploadChunkSize);

    if(isDone(m_loadingProcess) && uploadingDone) {
        m_currentMeshPoints.swap(m_loadingMeshPoints);
        startLoading();
    }

    m_loadIndex += uploadChunkSize;

    if(uploadingDone) {
        m_loadIndex = 0;
        std::swap(m_VBO, m_loadingVBO);
    }

    return *m_currentMeshPoints;
}

std::vector<GLuint>
Terrain::generateMeshIndices()
{
    std::vector<GLuint> meshIndices;
    meshIndices.reserve(granularity * granularity * 6);

    for(int x = 0; x < granularity - 1; x++)
        for(int z = 0; z < granularity - 1; z++) {
            meshIndices.push_back(z + x * granularity);
            meshIndices.push_back(z + (x + 1) * granularity);
            meshIndices.push_back((z + 1) + x * granularity);

            meshIndices.push_back((z + 1) + x * granularity);
            meshIndices.push_back(z + (x + 1) * granularity);
            meshIndices.push_back((z + 1) + (x + 1) * granularity);
        }

    return meshIndices;
}

double
Terrain::heightAt(const std::complex<double>& c)
{
    std::complex<double> z(0.0, 0.0);
    std::complex<double> dz(0.0, 0.0);

    // main cardioid check
    double q = pow(c.real() - 0.25, 2.0) + c.imag() * c.imag();
    if(q * (q + (c.real() - 0.25)) < 0.25 * c.imag() * c.imag()) {
        return 0.0;
    }

    // period-2 bulb check
    if((c.real() + 1.0) * (c.real() + 1.0) + c.imag() * c.imag()
       < 0.25 * 0.25) {
        return 0.0;
    }

    for(int i = 0; i < iterations; ++i) {
        dz = 2.0 * z * dz + 1.0;
        z  = z * z + c;

        if(std::abs(z) > 256) {
            double r  = std::abs(z);
            double dr = std::abs(dz);
            double de = 2.0 * r * std::log(r) / dr;    // estimated
                                                       // distance from
                                                       // set
            return de;
        }
    }

    return 0.0;
}

void
Terrain::render()
{
    int vertexCount = std::pow((granularity - 1), 2) * 3 * 2;
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}
