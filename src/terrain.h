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

#include "math3d.h"

class Terrain {
public:
    std::function<void(double, double)> m_callback;

    Terrain();
    ~Terrain();

    const std::vector<Vector3f>&
    updateMesh(double, double, double);

    static double
    heightAt(const std::complex<double>&);

    void
    render();
private:
    enum class State {Loading, Uploading};
    State m_state = State::Loading;

    static constexpr int granularity     = 400;
    static constexpr int iterations      = 100;
    static constexpr int uploadChunkSize = 90'000;

    GLuint m_VBO, m_loadingVBO, m_IBO;

    std::future<void> m_loadingProcess;
    unsigned int m_loadIndex = 0;

    double m_x;
    double m_z;
    double m_scale;

    std::shared_ptr<std::vector<Vector3f>> m_currentMeshPoints;
    std::shared_ptr<std::vector<Vector3f>> m_loadingMeshPoints;

    void
    startLoading();

    std::vector<GLuint>
    generateMeshIndices();

    static void
    loadMesh(double, double, double, std::vector<Vector3f>*);
};

template<typename T>
static bool
isDone(const std::future<T>& f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

#endif
