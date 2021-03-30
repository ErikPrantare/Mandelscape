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

struct Points {
    std::vector<glm::vec3> position;
    std::vector<float> value;

    // int instead of bool because of vector<bool> specialization
    std::vector<int> inside;

    size_t size = 0;
};

class PointLoader {
public:
    struct Args {
        int granularity;
        glm::dvec3 offset;
        double scale;
        int iterations;
        std::function<algorithm::Signature> function;
        std::unique_ptr<Points> buffer;
    };

    PointLoader(Args&& args);
    ~PointLoader();

    PointLoader(PointLoader const&) = delete;
    PointLoader(PointLoader&&)      = default;
    auto
    operator=(PointLoader const&) -> PointLoader& = delete;
    auto
    operator=(PointLoader&&) -> PointLoader& = default;

    auto
    start() -> void;

    [[nodiscard]] auto
    done() const -> bool;

    [[nodiscard]] auto
    get() -> std::unique_ptr<Points>;

private:
    auto
    operator()() -> void;

    int m_granularity;
    glm::dvec3 m_offset;
    double m_scale;
    int m_iterations;
    std::function<algorithm::Signature> m_function;
    std::unique_ptr<Points> m_buffer;

    std::vector<double> m_xPos;
    std::vector<double> m_zPos;

    std::future<void> m_process;
};

class Terrain {
public:
    Terrain();

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

    static int constexpr valueAttributeLocation = 1;

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

    PointLoader m_loader = {{}};

    std::unique_ptr<Points> m_buffer{new Points{}};

    static auto
    generateMeshIndices() -> std::vector<GLuint>;

    [[nodiscard]] auto
    createLoader() -> PointLoader;

    auto
    uploadChunk() -> void;
};

#endif
