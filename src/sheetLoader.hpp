/* Mandelscape
 * Copyright (C) 2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MANDELSCAPE_SHEETLOADER_HPP
#define MANDELSCAPE_SHEETLOADER_HPP

#include <functional>
#include <future>
#include <vector>
#include <atomic>

#include "glm/vec3.hpp"

#include "algorithm.hpp"

struct Points {
    std::vector<glm::vec3> position;
    std::vector<float> value;

    // int instead of bool because of vector<bool> specialization.
    // The bool specialization would make it impossible to transfer the
    // individual elements to the GPU.
    std::vector<int> inside;
    std::vector<glm::vec3> normal;

    size_t size = 0;
};

class SheetLoader {
public:
    struct Args {
        int granularity;
        glm::dvec3 offset;
        double scale;
        int iterations;
        std::function<algorithm::Signature> function;
        std::unique_ptr<Points> buffer = nullptr;
    };

    // CPP20 std::jthread
    using HaltFlag = std::shared_ptr<std::atomic<bool>>;

    class FuturePoints : public std::future<std::unique_ptr<Points>> {
    public:
        FuturePoints() = default;
        FuturePoints(
                std::future<std::unique_ptr<Points>>&& future,
                HaltFlag& halt) :
                    std::future<std::unique_ptr<Points>>(std::move(future)),
                    m_halt(new HaltFlag(halt))
        {}

    private:
        // CPP20 decltype(lambda)
        struct HaltFlagDestructor {
            auto
            operator()(HaltFlag* halt) noexcept -> void
            {
                **halt = true;
                delete halt;
            }
        };
        std::unique_ptr<HaltFlag, HaltFlagDestructor> m_halt;
    };

    [[nodiscard]] static auto
    createProcess(Args&& args) -> FuturePoints;

    SheetLoader(Args&& args);

private:
    auto
    operator()(HaltFlag const& halt) -> void;

    Args m_args;

    std::vector<double> m_xPos;
    std::vector<double> m_zPos;
};

auto
resize(Points& points, size_t size) -> void;

#endif
