#include <vector>
#include <cmath>
#include <complex>

#include <GL/glew.h>

#include "terrainMeshLoader.h"


std::vector<Vector3f>
TerrainMeshLoader::getMeshPoints(double m_x, double m_z, double m_scale)
{
    std::vector<Vector3f> ps;
    for(int x = 0; x < granularity; x++)
    for(int z = 0; z < granularity; z++) {
        double discScale = std::pow(2.0, int(log2(m_scale)));
        double discX = int(m_x*discScale/4)/discScale*4;
        double discZ = int(m_z*discScale/4)/discScale*4;
        double xPos = (x/(granularity/16.0d)-8.0d)/discScale + discX;
        double zPos = (z/(granularity/16.0d)-8.0d)/discScale + discZ;

        ps.emplace_back(xPos, heightAt({xPos, zPos}), zPos);
    }

    return ps;
}


std::vector<GLuint>
TerrainMeshLoader::getMeshIndices()
{
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

    return indices;
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
