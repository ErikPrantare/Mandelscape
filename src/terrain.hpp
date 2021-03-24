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
#include <future>
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
#include "persistentActionMap.hpp"
#include "algorithm.hpp"

class Terrain {
public:
    Terrain();
    Terrain(const Terrain&) = delete;
    auto
    operator=(const Terrain&) -> Terrain& = delete;
    Terrain(Terrain&&)                    = delete;
    auto
    operator=(Terrain&&) -> Terrain& = delete;

    ~Terrain();

    auto
    updateMesh(double x, double z, double scale) -> void;

    [[nodiscard]] auto
    heightAt(glm::dvec2 const&) -> double;

    [[nodiscard]] auto
    offset() const noexcept -> glm::dvec3;

    auto
    render(ShaderProgram* shaderProgram) -> void;

    auto
    loadLua(std::string const& code) -> void;

    auto
    setIterations(int iterations) noexcept -> void;

    static int constexpr colorLocation = 1;

private:
    static int constexpr granularity     = 400;
    int m_iterations                     = 100;
    static int constexpr uploadChunkSize = 90'000;

    glm::dvec3 m_offset        = glm::dvec3{0.0, 0.0, 0.0};
    glm::dvec3 m_loadingOffset = glm::dvec3{0.0, 0.0, 0.0};

    double m_scale = 1.0;

    // duplication bc two different threads. Gotta clean up this mess...
    lua_State* m_luaPointData           = nullptr;
    lua_State* m_luaPointDataHeightFunc = nullptr;

    std::function<algorithm::PointData(glm::dvec2 const&, int iterations)>
            m_pointData = algorithm::mandelbrot;
    std::function<algorithm::PointData(glm::dvec2 const&, int iterations)>
            m_pointDataHeightFunc = algorithm::mandelbrot;

    enum class State { Loading, Uploading };
    State m_state = State::Loading;

    Mesh m_mesh        = Mesh();
    Mesh m_loadingMesh = Mesh();

    size_t m_loadIndex = 0;

    std::future<void> m_loadingProcess;

    struct Points {
        std::vector<glm::vec3> position;
        std::vector<float> value;

        // int instead of bool because of vector<bool> specialization
        std::vector<int> inside;

        size_t size = 0;
    };

    static auto
    resize(Points* points, size_t size) -> void;

    Points m_points;

    auto
    startLoading() -> void;

    static auto
    generateMeshIndices() -> std::vector<GLuint>;

    auto
    loadMesh(glm::dvec3 offset, double scale, Points* points) -> void;
};

#endif
