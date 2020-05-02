#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

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

#include "utils.h"
#include "event.h"
#include "shader.h"
#include "shaderProgram.h"
#include "texture.h"

class Terrain {
public:
    Terrain();
    ~Terrain();

    void
    handleEvent(Event event);

    glm::vec2
    updateMesh(double const, double const, double const);

    int
    iterations() const;

    double
    heightAt(std::complex<double> const&);

    void
    render();

    ShaderProgram&
    shaderProgram();

private:
    static int constexpr granularity     = 400;
    int m_iterations                     = 100;
    static int constexpr uploadChunkSize = 90'000;

    glm::vec2 m_offset;
    glm::vec2 m_loadingOffset;
    double m_scale;

    enum class State { Loading, Uploading };
    State m_state = State::Loading;

    ShaderProgram m_shaderProgram = ShaderProgram();

    Shader m_vertexShader =
            Shader::fromFile("shaders/shader.vert", GL_VERTEX_SHADER);

    Shader m_shallowFragShader =
            Shader::fromFile("shaders/shader.frag", GL_FRAGMENT_SHADER);

    Shader m_deepFragShader =
            Shader::fromFile("shaders/deepShader.frag", GL_FRAGMENT_SHADER);

    enum class NextFrag { Shallow, Deep } m_nextFrag = NextFrag::Deep;
    Texture m_texture;

    GLuint m_VBO, m_loadingVBO, m_IBO;

    unsigned int m_loadIndex = 0;

    std::future<void> m_loadingProcess;

    std::shared_ptr<std::vector<glm::vec3>> m_currentMeshPoints;
    std::shared_ptr<std::vector<glm::vec3>> m_loadingMeshPoints;

    void
    startLoading();

    std::vector<GLuint>
    generateMeshIndices();

    void
    loadMesh(glm::vec2 const, double const, std::vector<glm::vec3>* const);
};

#endif
