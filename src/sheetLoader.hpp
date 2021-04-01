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

#include "glm/vec3.hpp"

#include "algorithm.hpp"

struct Points {
    std::vector<glm::vec3> position;
    std::vector<float> value;

    // int instead of bool because of vector<bool> specialization
    std::vector<int> inside;

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
        std::unique_ptr<Points> buffer;
    };

    SheetLoader(Args&& args);
    ~SheetLoader();

    SheetLoader(SheetLoader const&) = delete;
    SheetLoader(SheetLoader&&)      = default;
    auto
    operator=(SheetLoader const&) -> SheetLoader& = delete;
    auto
    operator=(SheetLoader&&) -> SheetLoader& = default;

    auto
    start() -> void;

    [[nodiscard]] auto
    done() const -> bool;

    [[nodiscard]] auto
    get() -> std::unique_ptr<Points>;

private:
    auto
    operator()() -> void;

    int m_granularity;
    glm::dvec3 m_offset;
    double m_scale;
    int m_iterations;
    std::function<algorithm::Signature> m_function;
    std::unique_ptr<Points> m_buffer;

    std::vector<double> m_xPos;
    std::vector<double> m_zPos;

    std::future<void> m_process;
};

auto
resize(Points& points, size_t size) -> void;

#endif
