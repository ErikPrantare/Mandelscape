#include <vector>
#include <cmath>
#include <complex>
#include <thread>
#include <memory>

#include <glad/glad.h>

#include "terrain.h"
#include "utils.h"
#include "shader.h"

Terrain::Terrain() :
            m_offset{0.0, 0.0},
            m_loadingOffset{0.0, 0.0},
            m_scale{1.0},
            m_texture("textures/texture.png"),
            m_currentMeshPoints{std::make_shared<std::vector<glm::vec3>>()},
            m_loadingMeshPoints{std::make_shared<std::vector<glm::vec3>>()}
{
    loadMesh(m_loadingOffset, m_scale, m_currentMeshPoints.get());
    loadMesh(m_loadingOffset, m_scale, m_loadingMeshPoints.get());

    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);
    glGenBuffers(1, &m_loadingVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            m_currentMeshPoints->size() * sizeof(glm::vec3),
            m_currentMeshPoints->data(),
            GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_loadingVBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            m_loadingMeshPoints->size() * sizeof(glm::vec3),
            m_loadingMeshPoints->data(),
            GL_DYNAMIC_DRAW);

    auto meshIndices = generateMeshIndices();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            meshIndices.size() * sizeof(meshIndices[0]),
            meshIndices.data(),
            GL_STATIC_DRAW);

    m_vertexShader.attachTo(m_shaderProgram);
    m_shallowFragShader.attachTo(m_shaderProgram);
    m_shaderProgram.compile();

    startLoading();
}

Terrain::~Terrain()
{
    m_loadingProcess.wait();
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteBuffers(1, &m_loadingVBO);
}

ShaderProgram&
Terrain::shaderProgram()
{
    return m_shaderProgram;
};

void
Terrain::handleEvent(Event event)
{
    auto const changeIterationCount = [this](KeyDown const keyEvent) {
        switch(keyEvent.code) {
        case GLFW_KEY_I: {
            m_iterations += 20;
        } break;
        case GLFW_KEY_U: {
            m_iterations -= 20;
        } break;
        case GLFW_KEY_H: {
            switch(m_nextFrag) {
            case NextFrag::Shallow: {
                m_shallowFragShader.attachTo(m_shaderProgram);
                m_nextFrag = NextFrag::Deep;
            } break;

            case NextFrag::Deep: {
                m_deepFragShader.attachTo(m_shaderProgram);
                m_nextFrag = NextFrag::Shallow;
            } break;
            }
            m_shaderProgram.compile();
        } break;
        }
    };
    std::visit(util::overload{changeIterationCount, util::unaryNOP}, event);
}

void
Terrain::loadMesh(
        glm::vec2 offset,
        double const _scale,
        std::vector<glm::vec3>* const buffer)
{
    int constexpr nrIndices = granularity * granularity;

    if(buffer->size() != nrIndices) {
        buffer->resize(nrIndices);
    }

    int constexpr doublingInterval = 40;

    auto const stepSize = [](int i) {
        return std::pow(2.0, std::abs(i - granularity / 2) / doublingInterval);
    };

    auto const& quantized = [](double x, double stepSize) {
        return std::floor(x / stepSize) * stepSize;
    };

    double meshSpan = 0.0;
    for(int i = 0; i < granularity; ++i) {
        meshSpan += stepSize(i);
    }

    double const discreteScale = std::pow(2.0, int(log2(_scale)));
    double const normMeshSpan  = 300.0 / discreteScale;

    double const normFactor = normMeshSpan / meshSpan;
    auto const normStepSize = [normFactor, stepSize](int i) {
        return normFactor * stepSize(i);
    };

    double xPos = -normMeshSpan / 2 + offset.x;
    for(int x = 0; x < granularity; ++x) {
        double const xQuant = quantized(xPos, normStepSize(x));

        double zPos = -normMeshSpan / 2 + offset.y;
        for(int z = 0; z < granularity; ++z) {
            double const zQuant            = quantized(zPos, normStepSize(z));
            (*buffer)[x * granularity + z] = glm::vec3(
                    xQuant - offset.x,
                    heightAt({xQuant, zQuant}),
                    zQuant - offset.y);

            zPos += normStepSize(z);
        }
        xPos += normStepSize(x);
    }
}

void
Terrain::startLoading()
{
    m_loadingProcess = std::async(std::launch::async, [this]() {
        loadMesh(m_loadingOffset, m_scale, m_loadingMeshPoints.get());
    });
}

bool
uploadMeshChunk(
        std::vector<glm::vec3> const& sourceMesh,
        GLuint const destinationVBO,
        size_t const index,
        size_t const maxChunkSize)
{
    if(index >= sourceMesh.size()) {
        return true;
    }

    glm::vec3 const* position = sourceMesh.data() + index;

    int chunkSize = std::min(maxChunkSize, sourceMesh.size() - index);

    glBindBuffer(GL_ARRAY_BUFFER, destinationVBO);
    glBufferSubData(
            GL_ARRAY_BUFFER,
            index * sizeof(glm::vec3),
            chunkSize * sizeof(glm::vec3),
            position);

    return (index + chunkSize) >= sourceMesh.size();
}

glm::vec2
Terrain::updateMesh(double const x, double const z, double const scale)
{
    const bool uploadingDone = uploadMeshChunk(
            *m_currentMeshPoints,
            m_loadingVBO,
            m_loadIndex,
            uploadChunkSize);

    m_loadIndex += uploadChunkSize;

    if(uploadingDone) {
        switch(m_state) {
        case State::Loading: {
            if(util::isDone(m_loadingProcess)) {
                std::swap(m_currentMeshPoints, m_loadingMeshPoints);
                m_loadIndex = 0;

                m_state = State::Uploading;
            }
        } break;

        case State::Uploading: {
            std::swap(m_VBO, m_loadingVBO);

            m_offset        = m_loadingOffset;
            m_loadingOffset = {x, z};
            m_scale         = scale;

            startLoading();

            m_state = State::Loading;
        } break;
        }
    }

    return m_offset;
}

int
Terrain::iterations() const
{
    return m_iterations;
}

std::vector<GLuint>
Terrain::generateMeshIndices()
{
    std::vector<GLuint> meshIndices;
    meshIndices.reserve(granularity * granularity * 6);

    for(int x = 0; x < granularity - 1; x++)
        for(int z = 0; z < granularity - 1; z++) {
            meshIndices.push_back(z + x * granularity);
            meshIndices.push_back((z + 1) + x * granularity);
            meshIndices.push_back(z + (x + 1) * granularity);

            meshIndices.push_back((z + 1) + x * granularity);
            meshIndices.push_back((z + 1) + (x + 1) * granularity);
            meshIndices.push_back(z + (x + 1) * granularity);
        }

    return meshIndices;
}

double
Terrain::heightAt(std::complex<double> const& c)
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

    for(int i = 0; i < m_iterations; ++i) {
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
    m_shaderProgram.setUniformInt("iterations", m_iterations);
    m_shaderProgram.setUniformVec2("offset", m_offset.x, m_offset.y);
    m_texture.makeActiveOn(GL_TEXTURE0);

    int vertexCount = std::pow((granularity - 1), 2) * 3 * 2;
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}
