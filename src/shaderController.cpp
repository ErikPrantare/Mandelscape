/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
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

#include "shaderController.hpp"

#include <variant>

#include "util.hpp"

ShaderController::ShaderController(ShaderProgram* shaderProgram)
{
    m_vertexShader.attachTo(*shaderProgram);
    m_fragmentShaders[m_currentFragmentShader].attachTo(*shaderProgram);
    shaderProgram->compile();
}

auto
ShaderController::handleMomentaryAction(MomentaryAction const& action) noexcept
        -> void
{
    if(action == MomentaryAction{Trigger::SwitchShader}) {
        ++m_currentFragmentShader;
        m_currentFragmentShader %= m_fragmentShaders.size();
        m_switchShader = true;
    }
}

auto
ShaderController::update(ShaderProgram& shaderProgram) -> void
{
    if(m_switchShader) {
        m_switchShader = false;

        recompile(shaderProgram);
    }
}

auto
ShaderController::setValueFunction(
        ShaderProgram& shaderProgram,
        std::string const& code) -> void
{
    m_fragmentShaders = {
            FragmentShader::fromCode(
                    util::getContents(std::ifstream("shaders/head.frag"))
                    + util::getContents(
                            std::ifstream("shaders/shallowLib.frag"))
                    + code
                    + util::getContents(std::ifstream("shaders/color.frag"))
                    + util::getContents(std::ifstream("shaders/shader.frag"))),
            FragmentShader::fromCode(
                    util::getContents(std::ifstream("shaders/head.frag"))
                    + util::getContents(std::ifstream("shaders/deepLib.frag"))
                    + code
                    + util::getContents(std::ifstream("shaders/color.frag"))
                    + util::getContents(
                            std::ifstream("shaders/shader.frag")))};

    recompile(shaderProgram);
}

auto
ShaderController::recompile(ShaderProgram& shaderProgram) const -> void
{
    m_vertexShader.attachTo(shaderProgram);
    m_fragmentShaders[m_currentFragmentShader].attachTo(shaderProgram);
    shaderProgram.compile();
}
