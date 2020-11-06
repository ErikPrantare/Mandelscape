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
    render() -> void;

    auto
    shaderProgram() -> ShaderProgram&;

private:
    static int constexpr granularity     = 400;
    int m_iterations                     = 100;
    static int constexpr uploadChunkSize = 90'000;

    glm::dvec3 m_offset        = glm::dvec3{0.0, 0.0, 0.0};
    glm::dvec3 m_loadingOffset = glm::dvec3{0.0, 0.0, 0.0};

    double m_scale = 1.0;

    enum class State { Loading, Uploading };
    State m_state = State::Loading;

    ShaderProgram m_shaderProgram = ShaderProgram();

    VertexShader m_vertexShader =
            VertexShader::fromFile("shaders/shader.vert");

    int m_currentFragmentShader                     = 0;
    std::array<FragmentShader, 2> m_fragmentShaders = {
            FragmentShader::fromFile("shaders/orbit_trap.frag"),
            FragmentShader::fromFile("shaders/deepShader.frag")};

    bool m_fastMode = false;

    Mesh m_mesh        = Mesh();
    Mesh m_loadingMesh = Mesh();

    int m_loadIndex = 0;

    std::future<void> m_loadingProcess;

    std::vector<glm::vec3> m_currentMeshPoints;
    std::vector<glm::vec3> m_loadingMeshPoints;
    std::vector<float> m_colors;
    std::vector<float> m_loadingColors;

    auto
    startLoading() -> void;

    static auto
    generateMeshIndices() -> std::vector<GLuint>;

    auto
    loadMesh(glm::dvec3, double, std::vector<glm::vec3>*, std::vector<float>*)
            -> void;
};

#endif
