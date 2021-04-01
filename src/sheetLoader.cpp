#include "sheetLoader.hpp"

#include "util.hpp"

auto
resize(Points& points, size_t const size) -> void
{
    points.position.resize(size);
    points.value.resize(size);
    points.inside.resize(size);
    points.size = size;
}

SheetLoader::SheetLoader(Args&& args) :
            m_granularity(args.granularity),
            m_offset(args.offset),
            m_scale(args.scale),
            m_iterations(args.iterations),
            m_function(args.function),
            m_buffer(std::move(args.buffer))
{
    // The mesh points need to be clamped in such a way that
    // reloading yields a smooth transition without any jumps.

    auto constexpr doublingInterval = 40;

    // Take longer steps for indices far away from middle
    auto const unscaledStepSize = [this](int i) {
        return std::pow(
                2.0,
                std::abs(i - m_granularity / 2) / doublingInterval);
    };

    // CPP20 use ranges
    double const unscaledMeshSize = [this, &unscaledStepSize] {
        auto sum = 0.0;
        for(int i = 0; i < m_granularity; ++i) {
            sum += unscaledStepSize(i);
        }
        return sum;
    }();

    auto const quantizedScale = util::truncateExponent(m_scale, 2.0);
    auto const meshSize       = 300.0 / quantizedScale;

    auto const scaleFactor = meshSize / unscaledMeshSize;
    auto const stepSize    = [scaleFactor, &unscaledStepSize](int i) {
        return scaleFactor * unscaledStepSize(i);
    };

    // Quantize x to grid, with distance stepSize between gridlines.
    auto const quantized = [](double x, double stepSize) {
        return std::floor(x / stepSize) * stepSize;
    };

    auto xPos = -meshSize / 2 + m_offset.x;
    for(int x = 0; x < m_granularity; ++x) {
        m_xPos.push_back(quantized(xPos, stepSize(x)));
        xPos += stepSize(x);
    }

    auto zPos = -meshSize / 2 + m_offset.z;
    for(int z = 0; z < m_granularity; ++z) {
        m_zPos.push_back(quantized(zPos, stepSize(z)));
        zPos += stepSize(z);
    }
}

SheetLoader::~SheetLoader()
{
    if(m_process.valid()) {
        m_process.wait();
    }
}

auto
SheetLoader::operator()() -> void
{
    size_t nrVertices = m_granularity * m_granularity;
    if(m_buffer->size != nrVertices) {
        resize(*m_buffer, nrVertices);
    }

    glm::vec3 const gpuOffset = util::toGpuVec(m_offset);

    size_t index = 0;
    for(auto const& x : m_xPos) {
        for(auto const& z : m_zPos) {
            auto const data = m_function({x, z}, m_iterations);

            m_buffer->position[index] =
                    glm::vec3(x - gpuOffset.x, data.height, z - gpuOffset.z);
            m_buffer->value[index]  = static_cast<float>(data.value);
            m_buffer->inside[index] = data.inside ? 1 : 0;
            ++index;
        }
    }
}

auto
SheetLoader::start() -> void
{
    m_process = std::async(std::launch::async, [this]() { (*this)(); });
}

auto
SheetLoader::done() const -> bool
{
    return util::isDone(m_process);
}

auto
SheetLoader::get() -> std::unique_ptr<Points>
{
    m_process.wait();
    return std::unique_ptr<Points>{m_buffer.release()};
}
