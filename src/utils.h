#ifndef MANDELLANDSCAPE_UTILS_H
#define MANDELLANDSCAPE_UTILS_H

#include <string>
#include <fstream>
#include <future>
#include <cmath>

namespace util {

std::string
readFile(std::string const& filePath);

// CPP20 https://en.cppreference.com/w/cpp/experimental/future/is_ready
template<typename T>
static bool
isDone(std::future<T> const& f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

class LowPassFilter {
public:
    LowPassFilter(float const init, float const amount) :
                m_filteredValue(init),
                m_amount(amount){};

    float
    operator()(float const newValue, float const weight = 1.0f)
    {
        const float factor = std::pow(m_amount, weight);

        m_filteredValue = factor * m_filteredValue + (1.0f - factor) * newValue;
        return m_filteredValue;
    }

private:
    float m_filteredValue;
    float const m_amount;
};

}    // namespace util

#endif    // MANDELLANDSCAPE_UTILS_H
