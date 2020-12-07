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

class Terrain {
public:
    Terrain();
    Terrain(const Terrain&) = delete;
    auto
    operator=(const Terrain&) -> Terrain& = delete;
    Terrain(Terrain&&)                    = delete;
    auto operator=(Terrain &&) -> Terrain& = delete;

    ~Terrain();

    auto
    handleMomentaryAction(MomentaryAction const& action) -> void;

    auto
    updateMesh(double, double, double) -> glm::dvec3;

    auto
    heightAt(glm::dvec2 const&) -> double;

    auto
    render(ShaderProgram* shaderProgram) -> void;

    static int constexpr colorLocation = 1;

private:
    static int constexpr granularity     = 400;
    int m_iterations                     = 100;
    static int constexpr uploadChunkSize = 90'000;

    glm::dvec3 m_offset        = glm::dvec3{0.0, 0.0, 0.0};
    glm::dvec3 m_loadingOffset = glm::dvec3{0.0, 0.0, 0.0};

    double m_scale = 1.0;

    enum class State { Loading, Uploading };
    State m_state = State::Loading;

    Mesh m_mesh        = Mesh();
    Mesh m_loadingMesh = Mesh();

    size_t m_loadIndex = 0;

    std::future<void> m_loadingProcess;

    struct Points {
        std::vector<glm::vec3> position;
        std::vector<float> value;

        // uchar instead of bool because of vector<bool> specialization
        std::vector<int> inside;

        size_t size = 0;
    };

    auto
    resize(Points* const points, size_t const size) -> void
    {
        points->position.resize(size);
        points->value.resize(size);
        points->inside.resize(size);
        points->size = size;
    }
    Points m_points;

    auto
    startLoading() -> void;

    static auto
    generateMeshIndices() -> std::vector<GLuint>;

    auto
    loadMesh(glm::dvec3 offset, double scale, Points* points) -> void;
};

#endif
