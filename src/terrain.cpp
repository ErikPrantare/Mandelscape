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
Terrain::createLoaderArgs() -> SheetLoader::Args
{
    // CPP20 {.x = ...}
    auto loaderArgs        = SheetLoader::Args{};
    loaderArgs.granularity = granularity;
    loaderArgs.offset      = m_loadingOffset;
    loaderArgs.scale       = m_scale;
    loaderArgs.iterations  = m_iterations;
    loaderArgs.function    = m_pointData;
    loaderArgs.buffer      = std::move(m_buffer);

    return loaderArgs;
}

Terrain::Terrain()
{
    m_meshPoints = SheetLoader::createProcess(createLoaderArgs());
    m_buffer     = m_meshPoints.get();

    // CPP20 std::vector<T> attributeBuffer
    auto const initAttribute = [this](auto const& attributeBuffer,
                                      auto const attributeLocation) {
        using ValueType = typename std::remove_reference_t<
                decltype(attributeBuffer)>::value_type;

        m_mesh.newAttribute<ValueType>(attributeLocation);
        m_loadingMesh.newAttribute<ValueType>(attributeLocation);
        m_mesh.setAttribute(attributeLocation, attributeBuffer);
        m_loadingMesh.setAttribute(attributeLocation, attributeBuffer);
    };

    initAttribute(m_buffer->position, positionAttributeLocation);
    initAttribute(m_buffer->value, valueAttributeLocation);
    initAttribute(m_buffer->inside, insideAttributeLocation);
    initAttribute(m_buffer->normal, normalAttributeLocation);

    // CPP20 {.imagePath = ...}
    auto textureArgs           = TextureArgs();
    textureArgs.imagePath      = "textures/stripes.png";
    textureArgs.uniformName    = "stripes";
    textureArgs.generateMipmap = true;
    textureArgs.index          = 1;
    auto stripeTexture         = std::make_shared<Texture>(textureArgs);
    m_mesh.addTexture(stripeTexture);
    m_loadingMesh.addTexture(stripeTexture);

    auto meshIndices = generateMeshIndices();
    m_mesh.setIndices(meshIndices);
    m_loadingMesh.setIndices(meshIndices);

    m_meshPoints = SheetLoader::createProcess(createLoaderArgs());
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

    auto const uploadAttribute = [uploadSize, this](
                                         auto const& attributeBuffer,
                                         auto const attributeLocation) {
        m_loadingMesh.setAttribute(
                attributeLocation,
                attributeBuffer,
                m_uploadIndex,
                uploadSize);
    };

    uploadAttribute(m_buffer->position, positionAttributeLocation);
    uploadAttribute(m_buffer->value, valueAttributeLocation);
    uploadAttribute(m_buffer->inside, insideAttributeLocation);
    uploadAttribute(m_buffer->normal, normalAttributeLocation);
    m_uploadIndex += uploadSize;
}

auto
Terrain::updateMesh(glm::dvec2 const middle, double const scale) -> void
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
        m_loadingOffset = {middle.x, 0.0, middle.y};
        m_scale         = scale;

        m_meshPoints = SheetLoader::createProcess(createLoaderArgs());
    }
    else if(util::isDone(m_meshPoints)) {
        m_uploadIndex = 0;
        m_buffer      = m_meshPoints.get();
        m_uploading   = true;
    }
}

auto
Terrain::generateMeshIndices() -> std::vector<GLuint>
{
    std::vector<GLuint> meshIndices;
    meshIndices.reserve(static_cast<size_t>(granularity) * granularity * 6);

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
Terrain::render(ShaderProgram& shaderProgram) -> void
{
    shaderProgram.setUniformInt("iterations", m_iterations);
    shaderProgram.setUniformVec2("offset", {m_offset.x, m_offset.z});

    m_mesh.render(shaderProgram);
}

auto
Terrain::bindAttributeLocations(ShaderProgram& shaderProgram) -> void
{
    shaderProgram.bindAttributeLocation(
            "pos",
            Terrain::positionAttributeLocation);
    shaderProgram.bindAttributeLocation(
            "val",
            Terrain::valueAttributeLocation);
    shaderProgram.bindAttributeLocation(
            "inside_",
            Terrain::insideAttributeLocation);
    shaderProgram.bindAttributeLocation(
            "normal_",
            Terrain::normalAttributeLocation);
}
