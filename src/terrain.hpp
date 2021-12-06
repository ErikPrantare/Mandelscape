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

#ifndef MANDELLANDSCAPE_TERRAIN_HPP
#define MANDELLANDSCAPE_TERRAIN_HPP

#include <vector>
#include <memory>
#include <complex>
#include <tuple>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "util.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "momentaryAction.hpp"
#include "stateMap.hpp"
#include "algorithm.hpp"
#include "sheetLoader.hpp"

class Terrain {
public:
    Terrain();

    auto
    updateMesh(glm::dvec2 middle, double scale) -> void;

    [[nodiscard]] auto
    heightAt(glm::dvec2 const&) -> double;

    [[nodiscard]] auto
    offset() const noexcept -> glm::dvec3;

    auto
    render(ShaderProgram& shaderProgram) -> void;

    auto
    loadLua(std::string const& code) -> void;

    auto
    setIterations(int iterations) noexcept -> void;

    auto static bindAttributeLocations(ShaderProgram& shaderProgram) -> void;

    static GLuint constexpr positionAttributeLocation = 0;
    static GLuint constexpr valueAttributeLocation    = 1;
    static GLuint constexpr insideAttributeLocation   = 2;
    static GLuint constexpr normalAttributeLocation   = 3;

private:
    static int constexpr granularity     = 400;
    int m_iterations                     = 100;
    static int constexpr uploadChunkSize = 90'000;

    glm::dvec3 m_offset        = glm::dvec3{0.0, 0.0, 0.0};
    glm::dvec3 m_loadingOffset = glm::dvec3{0.0, 0.0, 0.0};

    double m_scale = 1.0;

    // duplication bc two different threads. Gotta clean up this mess...
    std::function<algorithm::Signature> m_pointData = algorithm::mandelbrot;
    std::function<algorithm::Signature> m_pointDataHeightFunc =
            algorithm::mandelbrot;

    bool m_uploading = false;

    Mesh m_mesh        = Mesh();
    Mesh m_loadingMesh = Mesh();

    size_t m_uploadIndex = 0;

    std::unique_ptr<Points> m_buffer{new Points{}};
    SheetLoader::FuturePoints m_meshPoints;

    static auto
    generateMeshIndices() -> std::vector<GLuint>;

    [[nodiscard]] auto
    createLoaderArgs() -> SheetLoader::Args;

    auto
    uploadChunk() -> void;
};

#endif
