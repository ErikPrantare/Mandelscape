#ifndef MANDELLANDSCAPE_TERRAIN_HPP
#define MANDELLANDSCAPE_TERRAIN_HPP

#include <vector>
#include <memory>
#include <complex>
#include <future>
#include <functional>
#include <variant>
#include <tuple>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "event.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"
#include "texture.hpp"
#include "mesh.hpp"

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
    handleEvent(Event event) -> void;

    auto
    updateMesh(double, double, double) -> glm::dvec2;

    auto
    iterations() const -> int;

    auto
    heightAt(std::complex<double> const&) -> double;

    auto
    render() -> void;

    ShaderProgram&
    shaderProgram();

private:
    static int constexpr granularity     = 400;
    int m_iterations                     = 100;
    static int constexpr uploadChunkSize = 90'000;

    glm::dvec2 m_offset        = glm::dvec2{0.0, 0.0};
    glm::dvec2 m_loadingOffset = glm::dvec2{0.0, 0.0};
    double m_scale             = 1.0;

    enum class State { Loading, Uploading };
    State m_state = State::Loading;

    ShaderProgram m_shaderProgram = ShaderProgram();

    VertexShader m_vertexShader = VertexShader::fromFile("shaders/shader.vert");

    FragmentShader m_shallowFragShader =
            FragmentShader::fromFile("shaders/shader.frag");

    FragmentShader m_deepFragShader =
            FragmentShader::fromFile("shaders/deepShader.frag");

    enum class NextFrag { Shallow, Deep } m_nextFrag = NextFrag::Deep;
    Texture m_texture = Texture("textures/texture.png");

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
    loadMesh(glm::dvec2, double, std::vector<glm::vec3>*) -> void;
};

#endif
