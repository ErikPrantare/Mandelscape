/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "terrain.hpp"

#include <vector>
#include <cmath>
#include <complex>
#include <thread>
#include <memory>

#include <glad/glad.h>

#include "nfd.hpp"

#include "util.hpp"
#include "shader.hpp"

auto
resize(Points* const points, size_t const size) -> void
{
    points->position.resize(size);
    points->value.resize(size);
    points->inside.resize(size);
    points->size = size;
}

auto
toGpuVec(glm::dvec3 const v) -> glm::vec3
{
    return static_cast<glm::vec3>(v);
}

class PointLoader {
public:
    PointLoader(
            int const granularity,
            glm::dvec3 const offset,
            double const scale,
            int iterations,
            std::function<algorithm::Signature> function) noexcept :
                m_granularity(granularity),
                m_offset(offset),
                m_scale(scale),
                m_iterations(iterations),
                m_function(std::move(function)){};

    auto
    loadPoints(Points* const points) -> void
    {
        // The mesh points need to be clamped in such a way that
        // reloading yields a smooth transition without any jumps.

        auto nrVertices = size_t(m_granularity * m_granularity);
        if(points->size != nrVertices) {
            resize(points, nrVertices);
        }

        auto constexpr doublingInterval = 40;

        // Take longer steps for indices far away from middle
        auto const unscaledStepSize = [this](int i) {
            return std::pow(
                    2.0,
                    std::abs(i - m_granularity / 2) / doublingInterval);
        };

        // CPP20 use ranges
        double const unscaledMeshSize = [this, &unscaledStepSize] {
            auto sum = 0.0;
            for(int i = 0; i < m_granularity; ++i) {
                sum += unscaledStepSize(i);
            }
            return sum;
        }();

        auto const quantizedScale = util::truncateExponent(m_scale, 2.0);
        auto const meshSize       = 300.0 / quantizedScale;

        auto const scaleFactor = meshSize / unscaledMeshSize;
        auto const stepSize    = [scaleFactor, &unscaledStepSize](int i) {
            return scaleFactor * unscaledStepSize(i);
        };

        // Quantize x to grid, with distance stepSize between gridlines.
        auto const quantized = [](double x, double stepSize) {
            return std::floor(x / stepSize) * stepSize;
        };

        glm::vec3 const gpuOffset = toGpuVec(m_offset);

        auto xPos = -meshSize / 2 + m_offset.x;
        for(int x = 0; x < m_granularity; ++x) {
            auto const xQuant = quantized(xPos, stepSize(x));

            auto zPos = -meshSize / 2 + m_offset.z;
            for(int z = 0; z < m_granularity; ++z) {
                auto const zQuant = quantized(zPos, stepSize(z));
                auto const data   = m_function({xQuant, zQuant}, m_iterations);

                points->position[x * m_granularity + z] = glm::vec3(
                        xQuant - gpuOffset.x,
                        data.height,
                        zQuant - gpuOffset.z);

                points->value[x * m_granularity + z] =
                        static_cast<float>(data.value);
                points->inside[x * m_granularity + z] = data.inside ? 1 : 0;
                zPos += stepSize(z);
            }
            xPos += stepSize(x);
        }
    }

private:
    int m_granularity;
    glm::dvec3 m_offset;
    double m_scale;
    int m_iterations;
    std::function<algorithm::Signature> m_function;
};

auto
Terrain::startLoading() -> void
{
    m_loadingProcess = std::async(std::launch::async, [this]() {
        auto pointLoader = PointLoader(
                granularity,
                m_loadingOffset,
                m_scale,
                m_iterations,
                m_pointData);
        pointLoader.loadPoints(&m_points);
    });
}

Terrain::Terrain()
{
    auto pointLoader = PointLoader(
            granularity,
            m_loadingOffset,
            m_scale,
            m_iterations,
            m_pointData);
    pointLoader.loadPoints(&m_points);

    m_mesh.setVertices(m_points.position);
    m_loadingMesh.setVertices(m_points.position);

    // value
    m_mesh.newAttribute(1);
    m_loadingMesh.newAttribute(1);
    m_mesh.setAttribute(1, m_points.value);
    m_loadingMesh.setAttribute(1, m_points.value);

    // inside
    m_mesh.newAttribute(2);
    m_loadingMesh.newAttribute(2);
    m_mesh.setAttribute(2, m_points.inside);
    m_loadingMesh.setAttribute(2, m_points.inside);

    // CPP20 {.imagePath = ...}
    auto textureArgs           = TextureArgs();
    textureArgs.imagePath      = "textures/texture.png";
    textureArgs.generateMipmap = true;
    auto texture               = std::make_shared<Texture>(textureArgs);
    m_mesh.setTexture(texture);
    m_loadingMesh.setTexture(texture);

    auto meshIndices = generateMeshIndices();
    m_mesh.setIndices(meshIndices);
    m_loadingMesh.setIndices(meshIndices);

    startLoading();
}

Terrain::~Terrain()
{
    m_loadingProcess.wait();
}

auto
Terrain::setIterations(int iterations) noexcept -> void
{
    m_iterations = iterations;
}

auto
Terrain::updateMesh(double const x, double const z, double const scale) -> void
{
    if(m_loadIndex < m_points.size) {
        auto const uploadSize =
                std::min(uploadChunkSize, (int)(m_points.size - m_loadIndex));

        m_loadingMesh.setVertices(m_points.position, m_loadIndex, uploadSize);
        m_loadingMesh.setAttribute(1, m_points.value, m_loadIndex, uploadSize);
        m_loadingMesh
                .setAttribute(2, m_points.inside, m_loadIndex, uploadSize);
        m_loadIndex += uploadSize;

        return;
    }

    switch(m_state) {
    case State::Loading: {
        if(util::isDone(m_loadingProcess)) {
            m_loadIndex = 0;

            m_state = State::Uploading;
        }
    } break;

    case State::Uploading: {
        swap(m_mesh, m_loadingMesh);

        m_offset        = m_loadingOffset;
        m_loadingOffset = {x, 0.0, z};
        m_scale         = scale;

        startLoading();

        m_state = State::Loading;
    } break;
    }
}

auto
Terrain::generateMeshIndices() -> std::vector<GLuint>
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

auto
Terrain::loadLua(std::string const& code) -> void
{
    m_loadingProcess.wait();
    m_pointData           = algorithm::fromLua(code);
    m_pointDataHeightFunc = algorithm::fromLua(code);
}

auto
Terrain::heightAt(glm::dvec2 const& pos) -> double
{
    return m_pointDataHeightFunc(pos, m_iterations).height;
}

auto
Terrain::offset() const noexcept -> glm::dvec3
{
    // offset must be consistent with shader offset (32 bit float)
    return toGpuVec(m_offset);
}

auto
Terrain::render(ShaderProgram* shaderProgram) -> void
{
    shaderProgram->setUniformInt("iterations", m_iterations);
    shaderProgram->setUniformVec2("offset", {m_offset.x, m_offset.z});

    m_mesh.render();
}
