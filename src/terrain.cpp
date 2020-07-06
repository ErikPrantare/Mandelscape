#include <vector>
#include <cmath>
#include <complex>
#include <thread>
#include <memory>

#include <glad/glad.h>

#include "terrain.hpp"
#include "utils.hpp"
#include "shader.hpp"

Terrain::Terrain()
{
    loadMesh(m_loadingOffset, m_scale, &m_currentMeshPoints);
    loadMesh(m_loadingOffset, m_scale, &m_loadingMeshPoints);

    m_mesh.setVertices(m_currentMeshPoints);
    m_loadingMesh.setVertices(m_currentMeshPoints);

    auto meshIndices = generateMeshIndices();
    m_mesh.setIndices(meshIndices);
    m_loadingMesh.setIndices(meshIndices);

    m_vertexShader.attachTo(m_shaderProgram);
    m_shallowFragShader.attachTo(m_shaderProgram);
    m_shaderProgram.compile();

    startLoading();
}

Terrain::~Terrain()
{
    m_loadingProcess.wait();
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
    std::visit(util::Overload{changeIterationCount, util::unaryNOP}, event);
}

void
Terrain::loadMesh(
        glm::dvec2 offset,
        double const scale,
        std::vector<glm::vec3>* const buffer)
{
    int constexpr nrIndices = granularity * granularity;

    if(buffer->size() != nrIndices) {
        buffer->resize(nrIndices);
    }

    int constexpr doublingInterval = 40;

    // The default capture is for compatibility with MSVC, it doesn't seem to
    // get constexpr fully
    auto const stepSize = [&](int i) {
        return std::pow(2.0, std::abs(i - granularity / 2) / doublingInterval);
    };

    auto const& quantized = [](double x, double stepSize) {
        return std::floor(x / stepSize) * stepSize;
    };

    double meshSpan = 0.0;
    for(int i = 0; i < granularity; ++i) {
        meshSpan += stepSize(i);
    }

    double const discreteScale = std::pow(2.0, int(log2(scale)));
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
        loadMesh(m_loadingOffset, m_scale, &m_loadingMeshPoints);
    });
}

glm::dvec2
Terrain::updateMesh(double const x, double const z, double const scale)
{
    int const uploadSize = std::min(
            uploadChunkSize,
            (int)(m_currentMeshPoints.size() - m_loadIndex));
    m_loadingMesh.setVertices(m_currentMeshPoints, m_loadIndex, uploadSize);
    m_loadIndex += uploadSize;

    const bool uploadingDone = m_loadIndex >= (int)m_currentMeshPoints.size();

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
            swap(m_mesh, m_loadingMesh);

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

    for(int x = 0; x < granularity - 1; x++) {
        for(int z = 0; z < granularity - 1; z++) {
            meshIndices.push_back(z + x * granularity);
            meshIndices.push_back((z + 1) + x * granularity);
            meshIndices.push_back(z + (x + 1) * granularity);

            meshIndices.push_back((z + 1) + x * granularity);
            meshIndices.push_back((z + 1) + (x + 1) * granularity);
            meshIndices.push_back(z + (x + 1) * granularity);
        }
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

    m_mesh.render();
}
