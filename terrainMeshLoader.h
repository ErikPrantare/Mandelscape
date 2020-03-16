#ifndef MANDELLANDSCAPE_TERRAINMESHLOADER_H
#define MANDELLANDSCAPE_TERRAINMESHLOADER_H

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <complex>
#include <future>

#include <GL/glew.h>

#include "math3d.h"

class
TerrainMeshLoader
{
public:
    static constexpr int granularity = 400;

    TerrainMeshLoader();
    ~TerrainMeshLoader();

    const std::vector<Vector3f>&
    updateMeshPoints(double, double, double);

    static double
    heightAt(const std::complex<double>&);

    void
    render();

private:
    static constexpr int iterations = 100;

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
    getMeshIndices();
};

template<typename T>
static bool
isDone(const std::future<T>& f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

#endif
