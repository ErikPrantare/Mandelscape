#include <vector>
#include <cmath>
#include <complex>
#include <thread>
#include <memory>

#include <GL/glew.h>

#include "terrainMeshLoader.h"

static std::vector<Vector3f>
mesh(double, double, double);

TerrainMeshLoader::TerrainMeshLoader()
{
    m_worker = std::thread(&TerrainMeshLoader::meshLoading, this);

    m_currentMeshPoints = std::make_shared<std::vector<Vector3f>>();
    m_loadingMeshPoints = std::make_shared<std::vector<Vector3f>>();
    *m_currentMeshPoints = mesh(m_x, m_z, m_scale);
}


TerrainMeshLoader::~TerrainMeshLoader()
{
    m_destruct = true;
    m_loadCond.notify_all();
    m_worker.join();
}


void
TerrainMeshLoader::meshLoading()
{
    while(!m_destruct) {
        std::unique_lock<std::mutex> lock(m_loadMutex);
        m_loadCond.wait(lock, [this]{return m_readyToLoad || m_destruct;});

        m_loadingMeshPoints = std::make_shared<std::vector<Vector3f>>();
        
        m_changeParams.lock();
        double x = m_x;
        double z = m_z;
        double scale = m_scale;
        m_changeParams.unlock();

        *m_loadingMeshPoints = mesh(x, z, scale);

        m_readyToLoad = false;
    }
}

static std::vector<Vector3f>
mesh(double _x, double _z, double _scale)
{
    std::vector<Vector3f> ps;

    for(int x = 0; x < TerrainMeshLoader::granularity; x++)
    for(int z = 0; z < TerrainMeshLoader::granularity; z++) {
        double discScale = std::pow(2.0, int(log2(_scale)));
        double discX = int(_x*discScale/4)/discScale*4;
        double discZ = int(_z*discScale/4)/discScale*4;
        double xPos = 
            (x/(TerrainMeshLoader::granularity/16.0d)-8.0d)/discScale + discX;
        double zPos = 
            (z/(TerrainMeshLoader::granularity/16.0d)-8.0d)/discScale + discZ;

        ps.emplace_back(xPos, TerrainMeshLoader::heightAt({xPos, zPos}), zPos);
    }

    return ps;
}


const std::vector<Vector3f>&
TerrainMeshLoader::getMeshPoints(double x, double z, double scale)
{
    m_changeParams.lock();
    m_x = x;
    m_z = z;
    m_scale = scale;
    m_changeParams.unlock();

    std::unique_lock<std::mutex> lock(m_loadMutex, std::defer_lock);
    if(lock.try_lock()) {
        m_currentMeshPoints = m_loadingMeshPoints;
        lock.unlock();
    }

    m_readyToLoad = true;
    m_loadCond.notify_all();
    return *m_currentMeshPoints;
}


std::vector<GLuint>
TerrainMeshLoader::getMeshIndices()
{
    if(m_meshIndices.size() != 0) return m_meshIndices;

    std::vector<GLuint> indices;

    for(int x = 0; x < granularity-1; x++)
        for(int z = 0; z < granularity-1; z++) {
            indices.push_back(z+x*granularity);
            indices.push_back(z+(x+1)*granularity);
            indices.push_back((z+1)+x*granularity);

            indices.push_back((z+1)+x*granularity);
            indices.push_back(z+(x+1)*granularity);
            indices.push_back((z+1)+(x+1)*granularity);
        }

    return m_meshIndices = indices;
}


double
TerrainMeshLoader::heightAt(const std::complex<double>& c)
{
    std::complex<double> z(0.0d, 0.0d); 
    std::complex<double> dz(0.0d, 0.0d);
    
    //main cardioid check
    double q = pow(c.real()-0.25d, 2.0d) + c.imag()*c.imag();
    if(q*(q+(c.real()-0.25d)) < 0.25d*c.imag()*c.imag()) {
        return 0.0d;
    }

    //period-2 bulb check
    if((c.real()+1.0d)*c.real()+1.0d + c.imag()*c.imag() < 0.625d) {
        return 0.0d;
    }

    int i = 0;
    for(; i < iterations; ++i) {
        dz = 2.0d*z*dz + 1.0d;
        z = z*z + c;

        if(std::abs(z) > 256) {
            double r = std::abs(z);
            double dr = std::abs(dz);
            double de = 2.0d*r*std::log(r)/dr; //estimated distance from set
            return de;
        }
    }

    return 0;
}


