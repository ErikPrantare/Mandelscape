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
    Terrain&
    operator=(const Terrain&) = delete;
    Terrain(Terrain&&)        = delete;
    Terrain&
    operator=(Terrain&&) = delete;

    ~Terrain();

    auto
    handleMomentaryAction(MomentaryAction const& action) -> void;

    auto
    updateMesh(double, double, double) -> glm::dvec3;

    auto
    heightAt(glm::dvec2 const&) -> double;

    auto
    render() -> void;

    ShaderProgram&
    shaderProgram();

private:
    static int constexpr granularity     = 400;
    int m_iterations                     = 100;
    static int constexpr uploadChunkSize = 90'000;

    // Must be 32 bit float, 64 bit will cause alignment issues with shader
    glm::vec3 m_offset        = glm::vec3{0.0, 0.0, 0.0};
    glm::vec3 m_loadingOffset = glm::vec3{0.0, 0.0, 0.0};

    double m_scale = 1.0;

    enum class State { Loading, Uploading };
    State m_state = State::Loading;

    ShaderProgram m_shaderProgram = ShaderProgram();

    VertexShader m_vertexShader =
            VertexShader::fromFile("shaders/shader.vert");

    int m_currentFragmentShader                     = 0;
    std::array<FragmentShader, 2> m_fragmentShaders = {
            FragmentShader::fromFile("shaders/shader.frag"),
            FragmentShader::fromFile("shaders/deepShader.frag")};

    Texture m_texture = Texture("textures/texture.png", GL_TEXTURE0);

    Mesh m_mesh        = Mesh();
    Mesh m_loadingMesh = Mesh();

    int m_loadIndex = 0;

    std::future<void> m_loadingProcess;

    std::vector<glm::vec3> m_currentMeshPoints;
    std::vector<glm::vec3> m_loadingMeshPoints;

    auto
    startLoading() -> void;

    static auto
    generateMeshIndices() -> std::vector<GLuint>;

    auto
    loadMesh(glm::dvec3, double, std::vector<glm::vec3>*) -> void;
};

#endif
