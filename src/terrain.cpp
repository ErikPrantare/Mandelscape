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
Terrain::createLoader() -> SheetLoader
{
    // CPP20 {.x = ...}
    auto pointLoaderArgs        = SheetLoader::Args{};
    pointLoaderArgs.granularity = granularity;
    pointLoaderArgs.offset      = m_loadingOffset;
    pointLoaderArgs.scale       = m_scale;
    pointLoaderArgs.iterations  = m_iterations;
    pointLoaderArgs.function    = m_pointData;
    pointLoaderArgs.buffer      = std::move(m_buffer);

    return {std::move(pointLoaderArgs)};
}

Terrain::Terrain()
{
    m_loader = createLoader();
    m_loader.start();
    m_buffer = m_loader.get();

    m_mesh.newAttribute<glm::vec3>(0);
    m_loadingMesh.newAttribute<glm::vec3>(0);
    m_mesh.setAttribute(0, m_buffer->position);
    m_loadingMesh.setAttribute(0, m_buffer->position);

    // value
    m_mesh.newAttribute<GLfloat>(1);
    m_loadingMesh.newAttribute<GLfloat>(1);
    m_mesh.setAttribute(1, m_buffer->value);
    m_loadingMesh.setAttribute(1, m_buffer->value);

    // inside
    m_mesh.newAttribute<GLint>(2);
    m_loadingMesh.newAttribute<GLint>(2);
    m_mesh.setAttribute(2, m_buffer->inside);
    m_loadingMesh.setAttribute(2, m_buffer->inside);

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

    m_loader = createLoader();
    m_loader.start();
}

auto
Terrain::setIterations(int iterations) noexcept -> void
{
    m_iterations = iterations;
}

auto
Terrain::uploadChunk() -> void
{
    auto const uploadSize =
            std::min(uploadChunkSize, (int)(m_buffer->size - m_uploadIndex));

    m_loadingMesh
            .setAttribute(0, m_buffer->position, m_uploadIndex, uploadSize);
    m_loadingMesh.setAttribute(1, m_buffer->value, m_uploadIndex, uploadSize);
    m_loadingMesh.setAttribute(2, m_buffer->inside, m_uploadIndex, uploadSize);
    m_uploadIndex += uploadSize;
}

auto
Terrain::updateMesh(double const x, double const z, double const scale) -> void
{
    if(m_uploading) {
        auto doneUploading = m_uploadIndex >= m_buffer->size;
        if(!doneUploading) {
            uploadChunk();
            return;
        }
        m_uploading = false;

        m_uploadIndex = 0;

        swap(m_mesh, m_loadingMesh);

        m_offset        = m_loadingOffset;
        m_loadingOffset = {x, 0.0, z};
        m_scale         = scale;

        m_loader = createLoader();
        m_loader.start();
    }
    else if(m_loader.done()) {
        m_uploadIndex = 0;
        m_buffer      = m_loader.get();
        m_uploading   = true;
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
    m_pointData           = algorithm::fromLua(code);
    m_pointDataHeightFunc = algorithm::fromLua(code);

    m_loader = createLoader();
    m_loader.start();
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
    return util::toGpuVec(m_offset);
}

auto
Terrain::render(ShaderProgram* shaderProgram) -> void
{
    shaderProgram->setUniformInt("iterations", m_iterations);
    shaderProgram->setUniformVec2("offset", {m_offset.x, m_offset.z});

    m_mesh.render();
}
