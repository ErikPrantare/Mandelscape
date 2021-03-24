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
Terrain::resize(Terrain::Points* const points, size_t const size) -> void
{
    points->position.resize(size);
    points->value.resize(size);
    points->inside.resize(size);
    points->size = size;
}

Terrain::Terrain()
{
    loadMesh(m_loadingOffset, m_scale, &m_points);

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
luaPointData(lua_State* l) -> std::function<algorithm::Signature>
{
    return [l](glm::dvec2 const& pos, int iterations) {
        lua_getglobal(l, "pointData");
        lua_pushnumber(l, pos.x);
        lua_pushnumber(l, pos.y);
        lua_pushnumber(l, iterations);
        if(lua_pcall(l, 3, 1, 0) != 0) {
            throw std::runtime_error(lua_tostring(l, -1));
        }
        auto p = algorithm::PointData();

        lua_getfield(l, -1, "height");
        if(lua_isnumber(l, -1) == 0) {
            throw std::runtime_error(
                    "lua function pointData didn't return a table"
                    " containing a number named \"height\"!");
        }
        p.height = lua_tonumber(l, -1);
        lua_pop(l, 1);

        lua_getfield(l, -1, "inside");
        if(lua_isboolean(l, -1) != 0) {
            p.inside = static_cast<bool>(lua_toboolean(l, -1));
        }
        lua_pop(l, 1);

        lua_getfield(l, -1, "value");
        if(lua_isnumber(l, -1) != 0) {
            p.value = lua_tonumber(l, -1);
        }
        lua_pop(l, 2);
        return p;
    };
}

auto
Terrain::setIterations(int iterations) noexcept -> void
{
    m_iterations = iterations;
}

auto
toGpuVec(glm::dvec3 const v) -> glm::vec3
{
    return static_cast<glm::vec3>(v);
}

auto
Terrain::loadMesh(glm::dvec3 offset, double const scale, Points* const points)
        -> void
{
    // The mesh points need to be clamped in such a way that
    // reloading yields a smooth transition without any jumps.

    auto constexpr nrVertices = granularity * granularity;

    if(points->size != nrVertices) {
        resize(points, nrVertices);
    }

    auto constexpr doublingInterval = 40;

    // Take longer steps for indices far away from middle
    auto const unscaledStepSize = [](int i) {
        return std::pow(2.0, std::abs(i - granularity / 2) / doublingInterval);
    };

    // CPP20 use ranges
    double const unscaledMeshSize = [&unscaledStepSize] {
        auto sum = 0.0;
        for(int i = 0; i < granularity; ++i) {
            sum += unscaledStepSize(i);
        }
        return sum;
    }();

    auto const quantizedScale = std::pow(2.0, int(log2(scale)));
    auto const meshSize       = 300.0 / quantizedScale;

    auto const scaleFactor = meshSize / unscaledMeshSize;
    auto const stepSize    = [scaleFactor, &unscaledStepSize](int i) {
        return scaleFactor * unscaledStepSize(i);
    };

    // Quantize x to grid, with distance stepSize between gridlines.
    auto const quantized = [](double x, double stepSize) {
        return std::floor(x / stepSize) * stepSize;
    };

    glm::vec3 const gpuOffset = toGpuVec(offset);

    auto xPos = -meshSize / 2 + offset.x;
    for(int x = 0; x < granularity; ++x) {
        auto const xQuant = quantized(xPos, stepSize(x));

        auto zPos = -meshSize / 2 + offset.z;
        for(int z = 0; z < granularity; ++z) {
            auto const zQuant = quantized(zPos, stepSize(z));
            auto const data   = m_pointData({xQuant, zQuant}, m_iterations);

            points->position[x * granularity + z] = glm::vec3(
                    xQuant - gpuOffset.x,
                    data.height,
                    zQuant - gpuOffset.z);

            points->value[x * granularity + z] =
                    static_cast<float>(data.value);
            points->inside[x * granularity + z] = data.inside ? 1 : 0;
            zPos += stepSize(z);
        }
        xPos += stepSize(x);
    }
}

auto
Terrain::startLoading() -> void
{
    m_loadingProcess = std::async(std::launch::async, [this]() {
        loadMesh(m_loadingOffset, m_scale, &m_points);
    });
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
    if(m_luaPointData != nullptr) {
        lua_close(m_luaPointData);
        lua_close(m_luaPointDataHeightFunc);
    }
    m_luaPointData           = luaL_newstate();
    m_luaPointDataHeightFunc = luaL_newstate();
    luaopen_math(m_luaPointData);
    luaopen_math(m_luaPointDataHeightFunc);
    if(luaL_dostring(m_luaPointData, code.c_str()) != 0) {
        throw std::runtime_error(lua_tostring(m_luaPointData, -1));
    }
    if(luaL_dostring(m_luaPointDataHeightFunc, code.c_str()) != 0) {
        throw std::runtime_error(lua_tostring(m_luaPointData, -1));
    }

    m_pointData           = luaPointData(m_luaPointData);
    m_pointDataHeightFunc = luaPointData(m_luaPointDataHeightFunc);
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
