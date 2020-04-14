#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <complex>
#include <future>
#include <functional>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "math3d.h"

class Terrain {
public:
    Terrain();
    Terrain(std::function<void(double, double)> const& setMeshOffset);
    ~Terrain();

    std::vector<glm::vec3> const&
    updateMesh(double, double, double);

    static double
    heightAt(const std::complex<double>&);

    void
    render();

private:
    enum class State { Loading, Uploading };
    State m_state = State::Loading;

    static constexpr int granularity     = 400;
    static constexpr int iterations      = 100;
    static constexpr int uploadChunkSize = 90'000;

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

    static void
    loadMesh(double, double, double, std::vector<glm::vec3>*);
};

template<typename T>
static bool
isDone(const std::future<T>& f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

#endif
