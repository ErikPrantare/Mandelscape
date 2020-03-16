#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

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
Terrain
{
public:
    Terrain();
    ~Terrain();

    const std::vector<Vector3f>&
    updateMesh(double, double, double);

    static double
    heightAt(const std::complex<double>&);

    void
    render();

    static constexpr int granularity = 400;
private:
    static constexpr int iterations = 100;

    GLuint m_VBO, m_loadingVBO, m_IBO;

    std::future<void> m_loadingProcess;
    bool m_doneLoading = false;
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


#endif
