#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

#include <vector>
#include <memory>
#include <complex>
#include <future>
#include <functional>
#include <variant>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "utils.h"
#include "event.h"

class Terrain {
public:
    Terrain();
    ~Terrain();

    void
    handleEvent(Event event);

    glm::vec2
    updateMesh(double const, double const, double const);

    void
    setIterations(int const);

    double
    heightAt(std::complex<double> const&);

    void
    render();

private:
    enum class State { Loading, Uploading };
    State m_state = State::Loading;

    static int constexpr granularity     = 400;
    int m_iterations                     = 100;
    static int constexpr uploadChunkSize = 90'000;

    GLuint m_VBO, m_loadingVBO, m_IBO;

    unsigned int m_loadIndex = 0;

    std::future<void> m_loadingProcess;

    glm::vec2 m_offset;
    glm::vec2 m_loadingOffset;
    double m_scale;

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
