#ifndef MANDELLANDSCAPE_TERRAIN_H
#define MANDELLANDSCAPE_TERRAIN_H

#include <vector>
#include <complex>

class
Terrain
{
public:
    std::vector<Vector3f>
    getMeshPoints() {
        std::vector<Vector3f> ps;
        for(int x = 0; x < granularity; x++)
        for(int z = 0; z < granularity; z++) {
            float xPos = x/(granularity/4.0f)-2.0f;
            float zPos = z/(granularity/4.0f)-2.0f;
            ps.emplace_back(
                    xPos,
                    iters(std::complex<float>(xPos, zPos)), 
                    zPos);
        }

        return ps;
    }

    std::vector<int>
    getMeshIndices() {
        std::vector<int> indices;
        for(int x = 0; x < granularity-1; x++)
        for(int z = 0; z < granularity-1; z++) {
            indices.push_back(z+x*granularity);
            indices.push_back(z+(x+1)*granularity);
            indices.push_back((z+1)+x*granularity);
            indices.push_back(z+(x+1)*granularity);
            indices.push_back((z+1)+x*granularity);
            indices.push_back((z+1)+(x+1)*granularity);
        }

        return indices;
    }

    static constexpr int granularity = 1000;
private:
    static constexpr int iterations = 100;

    float
    iters(std::complex<float> c) {
        std::complex<float> z(0.0f, 0.0f); 

        for(int i = 0; i < iterations; ++i) {
            z = z*z + c;
            if(std::abs(z) > 256) {
                float val = 
                    i - std::log(std::log(std::abs(z)))/std::log(2.0f) - 4.0f;
                return std::log10(9*val/iterations + 1.0f);
            }
        }
        
        return 1;
    }
};

#endif
