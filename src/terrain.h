#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

#include <vector>
#include <memory>
#include <complex>
#include <future>
#include <functional>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Terrain {
public:
    Terrain();
    Terrain(std::function<void(double, double)> const& setMeshOffset);
    ~Terrain();

    std::vector<glm::vec3> const&
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

    static int constexpr granularity    = 400;
    int m_iterations                    = 100;
    static int constexpr uploadChunkSize = 90'000;

    std::function<void(double, double)> m_setMeshOffset;

    GLuint m_VBO, m_loadingVBO, m_IBO;

    unsigned int m_loadIndex = 0;

    std::future<void> m_loadingProcess;

    double m_x;
    double m_z;
    double m_scale;

    std::shared_ptr<std::vector<glm::vec3>> m_currentMeshPoints;
    std::shared_ptr<std::vector<glm::vec3>> m_loadingMeshPoints;

    void
    startLoading();

    std::vector<GLuint>
    generateMeshIndices();

    void
    loadMesh(
            double const,
            double const,
            double const,
            std::vector<glm::vec3>* const);
};

#endif
