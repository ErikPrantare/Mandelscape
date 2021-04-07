#include "sheetLoader.hpp"

#include "util.hpp"

auto
calculateNormal(Points& points) -> void
{
    for(auto& n : points.normal) {
        n = {0.0, 0.0, 0.0};
    }
    for(int x = 0; x < 400 - 1; x++) {
        for(int z = 0; z < 400 - 1; z++) {
            auto const p1     = points.position[z + 400 * x];
            auto const p2     = points.position[z + 1 + 400 * x];
            auto const p3     = points.position[z + 400 * (x + 1)];
            auto const normal = glm::normalize(glm::cross(p1 - p2, p1 - p3));
            points.normal[z + x * 400] += 0.125f * normal;
            points.normal[z + 1 + x * 400] += 0.125f * normal;
            points.normal[z + (x + 1) * 400] += 0.125f * normal;
        }
    }
    for(int x = 0; x < 400 - 1; x++) {
        for(int z = 0; z < 400 - 1; z++) {
            auto const p1     = points.position[(z + 1) + 400 * x];
            auto const p2     = points.position[(z + 1) + 400 * (x + 1)];
            auto const p3     = points.position[z + 400 * (x + 1)];
            auto const normal = glm::normalize(glm::cross(p1 - p2, p1 - p3));
            points.normal[(z + 1) + 400 * x] += 0.125f * normal;
            points.normal[(z + 1) + 400 * (x + 1)] += 0.125f * normal;
            points.normal[z + 400 * (x + 1)] += 0.125f * normal;
        }
    }
}

auto
resize(Points& points, size_t const size) -> void
{
    points.position.resize(size);
    points.value.resize(size);
    points.inside.resize(size);
    points.normal.resize(size);
    points.size = size;
}

auto
SheetLoader::createProcess(Args&& args) -> std::future<std::unique_ptr<Points>>
{
    return std::async(
            std::launch::async,
            [](Args&& args) {
                auto loader = SheetLoader{std::move(args)};
                loader();
                calculateNormal(*loader.m_args.buffer);
                return std::move(loader.m_args.buffer);
            },
            std::move(args));
}

SheetLoader::SheetLoader(Args&& args) : m_args(std::move(args))
{
    // The mesh points need to be clamped in such a way that
    // reloading yields a smooth transition without any jumps.

    auto constexpr doublingInterval = 40;

    // Take longer steps for indices far away from middle
    auto const unscaledStepSize = [this](int i) {
        return std::pow(
                2.0,
                std::abs(i - m_args.granularity / 2) / doublingInterval);
    };

    // CPP20 use ranges
    double const unscaledMeshSize = [this, &unscaledStepSize] {
        auto sum = 0.0;
        for(int i = 0; i < m_args.granularity; ++i) {
            sum += unscaledStepSize(i);
        }
        return sum;
    }();

    auto const quantizedScale = util::truncateExponent(m_args.scale, 2.0);
    auto const meshSize       = 300.0 / quantizedScale;

    auto const scaleFactor = meshSize / unscaledMeshSize;
    auto const stepSize    = [scaleFactor, &unscaledStepSize](int i) {
        return scaleFactor * unscaledStepSize(i);
    };

    // Quantize x to grid, with distance stepSize between gridlines.
    auto const quantized = [](double x, double stepSize) {
        return std::floor(x / stepSize) * stepSize;
    };

    auto xPos = -meshSize / 2 + m_args.offset.x;
    for(int x = 0; x < m_args.granularity; ++x) {
        m_xPos.push_back(quantized(xPos, stepSize(x)));
        xPos += stepSize(x);
    }

    auto zPos = -meshSize / 2 + m_args.offset.z;
    for(int z = 0; z < m_args.granularity; ++z) {
        m_zPos.push_back(quantized(zPos, stepSize(z)));
        zPos += stepSize(z);
    }
}

auto
SheetLoader::operator()() -> void
{
    size_t nrVertices = m_args.granularity * m_args.granularity;
    if(m_args.buffer->size != nrVertices) {
        resize(*m_args.buffer, nrVertices);
    }

    glm::vec3 const gpuOffset = util::toGpuVec(m_args.offset);

    size_t index = 0;
    for(auto const& x : m_xPos) {
        for(auto const& z : m_zPos) {
            auto const data = m_args.function({x, z}, m_args.iterations);

            m_args.buffer->position[index] =
                    glm::vec3(x - gpuOffset.x, data.height, z - gpuOffset.z);
            m_args.buffer->value[index]  = static_cast<float>(data.value);
            m_args.buffer->inside[index] = data.inside ? 1 : 0;
            ++index;
        }
    }
}
