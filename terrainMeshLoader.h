#ifndef MANDELLANDSCAPE_TERRAINMESHLOADER_H
#define MANDELLANDSCAPE_TERRAINMESHLOADER_H

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <GL/glew.h>

#include "math3d.h"

class
TerrainMeshLoader
{
public:
    TerrainMeshLoader();
    ~TerrainMeshLoader();

    std::vector<Vector3f>
    getMeshPoints(double, double, double);

    std::vector<GLuint>
    getMeshIndices();

    static double
    heightAt(const std::complex<double>&);
    static constexpr int granularity = 100;
    static constexpr int iterations = 100;
private:
    
    std::vector<GLuint> m_meshIndices;

    std::shared_ptr<std::vector<Vector3f>> m_currentMeshPoints;
    std::shared_ptr<std::vector<Vector3f>> m_loadingMeshPoints;

    std::thread m_worker;
    std::mutex m_loadMutex;
    std::mutex m_changeParams;
    std::unique_lock<std::mutex> m_loadLock;
    std::condition_variable m_loadCond;
    bool m_readyToLoad = false;
    bool m_destruct = false;

    double m_x = 0.0d;
    double m_z = 0.0d; 
    double m_scale = 1.0d;
    
    void meshLoading();
};


#endif
