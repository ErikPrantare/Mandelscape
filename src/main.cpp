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

#include <string>
#include <tuple>
#include <stdexcept>
#include <filesystem>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define NFD_NATIVE
#include <nfd.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>

#include "util.hpp"
#include "camera.hpp"
#include "terrain.hpp"
#include "window.hpp"
#include "player.hpp"
#include "walkController.hpp"
#include "autoController.hpp"
#include "metaController.hpp"
#include "stateMap.hpp"
#include "momentaryActionsMap.hpp"
#include "shaderController.hpp"
#include "genericController.hpp"
#include "uniformController.hpp"
#include "serialization.hpp"
#include "keymaps.hpp"

auto
renderScene(Player const& player, glm::ivec2 viewSize, ShaderProgram* program)
        -> void;

auto
pickTerrainFunction(Terrain&, ShaderController&, ShaderProgram&) -> void;

auto
savePreset(Player const& player, UniformController const& uniformController)
        -> void;

auto
loadPresetFrom(
        std::filesystem::path const& path,
        Player& player,
        UniformController& uniformController) -> void;

auto
loadPreset(Player& player, UniformController& uniformController) -> void;

auto
main(int argc, char* argv[]) -> int
try {
    auto const args = std::vector(argv, argv + argc);

    auto window = Window({1368, 768});

    // nativefiledialog init
    auto nfdGuard = NFD::Guard();

    auto terrain = Terrain();
    auto player  = Player();

    auto [momentaryMap, stateMap] = [&args] {
        if(args.size() == 2 && args[1] == std::string("--dvorak")) {
            return initControlsDvorak();
        }

        return initControls();
    }();

    auto autoControllHeightFunc = [&terrain](glm::dvec2 x) {
        return terrain.heightAt(x);
    };

    auto metaController = MetaController{
            std::make_unique<WalkController>(),
            std::make_unique<AutoController>(autoControllHeightFunc)};

    auto shaderProgram = ShaderProgram();
    Terrain::bindAttributeLocations(shaderProgram);

    auto shaderController  = ShaderController(shaderProgram);
    auto uniformController = UniformController();

    loadPresetFrom("presets/default.lua", player, uniformController);

    auto elapsedTime     = 0.0;
    double lastTimepoint = glfwGetTime();

    // CPP20 {.x = ...}
    auto skybox = Texture(CubemapArgs{
            {
                    "textures/skybox/right.jpg",
                    "textures/skybox/left.jpg",
                    "textures/skybox/top.jpg",
                    "textures/skybox/bottom.jpg",
                    "textures/skybox/front.jpg",
                    "textures/skybox/back.jpg",
            },
            2,
            "skybox"});

    auto const render =
            [&skybox, &shaderProgram, &terrain, &player](glm::ivec2 size) {
                skybox.activateOn(shaderProgram);
                renderScene(player, size, &shaderProgram);
                terrain.render(shaderProgram);
            };

    // CPP20 concept on renderFunction
    auto const renderToFramebuffer = [](auto const& renderFunction,
                                        Framebuffer& framebuffer) {
        framebuffer.bind();
        glViewport(0, 0, framebuffer.size().x, framebuffer.size().y);
        renderFunction(framebuffer.size());
    };

    auto const handleAuxilliaryTriggers = [&render,
                                           &window,
                                           &shaderController,
                                           &terrain,
                                           &shaderProgram,
                                           &player,
                                           &uniformController,
                                           &renderToFramebuffer](
                                                  Trigger const& trigger) {
        switch(trigger) {
        case Trigger::LoadTerrainFunctions: {
            auto const suspendGuard = window.suspend();
            pickTerrainFunction(terrain, shaderController, shaderProgram);
        } break;
        case Trigger::TakeScreenshot: {
            auto screenshotBuffer = Framebuffer(2 * window.size());
            renderToFramebuffer(render, screenshotBuffer);

            savePng(std::filesystem::path("screenshots")
                            / (util::currentDatetimeString() + ".png"),
                    downsample(screenshotBuffer.readImage()));

            Framebuffer::bindDefaultBuffer();
            glViewport(0, 0, window.size().x, window.size().y);
        } break;
        case Trigger::Save: {
            auto const suspendGuard = window.suspend();
            savePreset(player, uniformController);
        } break;
        case Trigger::Load: {
            auto const suspendGuard = window.suspend();
            loadPreset(player, uniformController);
        } break;
        default:
            break;
        }
    };

    auto const handleAuxilliaryActions =
            [&handleAuxilliaryTriggers](MomentaryAction const& action) {
                util::dispatch(action, handleAuxilliaryTriggers);
            };

    while(window.update()) {
        double const currentTimepoint = glfwGetTime();
        double const dt               = currentTimepoint - lastTimepoint;
        lastTimepoint                 = currentTimepoint;

        while(auto const eventOpt = window.nextEvent()) {
            auto const event = eventOpt.value();

            stateMap.updateState(event);

            for(auto const& action : momentaryMap(event)) {
                window.handleMomentaryAction(action);
                metaController.handleMomentaryAction(action);
                uniformController.handleMomentaryAction(action);
                shaderController.handleMomentaryAction(action);
                handleAuxilliaryActions(action);
            }
        }

        if(!window.paused()) {
            elapsedTime += dt;

            metaController.updateState(stateMap);
            uniformController.updateState(stateMap, dt);

            auto const pos = player.truePosition();
            terrain.updateMesh({pos.x, pos.z}, 1.0 / player.scale());
            player.updateOffset(terrain.offset());
            auto const terrainHeight = terrain.heightAt({pos.x, pos.z});
            player.updateFeetAltitude(terrainHeight);
            metaController.update(player, dt);

            // Do this after .update, as autozoom is dependent on position.y
            player.updateFeetAltitude(
                    uniformController.yScale() * terrainHeight);
            player.update(dt);

            terrain.setIterations(uniformController.iterations());
        }

        shaderController.update(shaderProgram);
        uniformController.update(shaderProgram);
        shaderProgram.setUniformFloat("time", static_cast<float>(elapsedTime));
        render(window.size());
    }

    return 0;
}
catch(std::exception const& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Unexpected error, please create an issue at "
              << "https://github.com/ErikPrantare/Mandelscape "
              << "detailing the cause, or contact patched on FractalForums, "
              << "or send a mail to erik@prantare.xyz\n";

    return 1;
}
catch(...) {
    std::cerr << "Unexpected error, please create an issue at "
              << "https://github.com/ErikPrantare/Mandelscape "
              << "detailing the cause, or contact patched on FractalForums, "
              << "or sand a mail to erik@prantare.xyz\n";

    return 1;
}

void
renderScene(
        Player const& player,
        glm::ivec2 const viewSize,
        ShaderProgram* const program)
{
    // CPP20 {.x = ...}
    auto const camera = player.getCamera(viewSize);
    program->setUniformMatrix4("cameraSpace", camera.cameraSpace());
    program->setUniformMatrix4("projection", camera.projection());
    program->setUniformVec3("lookAt", camera.lookAt());
    program->setUniformVec3("playerPos", camera.position());
}

auto
savePreset(Player const& player, UniformController const& uniformController)
        -> void
{
    namespace fs = std::filesystem;
    using namespace util::nfd::literal;

    std::vector<util::nfd::FilterItem> filterItems{
            {"Lua files"_nfd, "lua"_nfd}};

    auto const presetDirectory = fs::current_path() / "presets";
    auto const searchDirectory = fs::is_directory(presetDirectory)
                                         ? presetDirectory
                                         : fs::current_path();

    auto const path = util::nfd::saveDialog(
            filterItems,
            searchDirectory,
            "my-preset.lua"_nfd);

    if(!path) {
        return;
    }

    std::ofstream out(*path);
    serialize(out, player, "player");
    out << "\n\n";
    serialize(out, uniformController, "uniformController");
}

auto
loadPresetFrom(
        std::filesystem::path const& path,
        Player& player,
        UniformController& uniformController) -> void
{
    std::ifstream in(path);
    lua_State* l = luaL_newstate();
    luaL_dostring(l, util::getContents(in).c_str());

    lua_getglobal(l, "player");
    player = deserialize<Player>(l, -1);
    lua_pop(l, 1);

    lua_getglobal(l, "uniformController");
    uniformController = deserialize<UniformController>(l, -1);
    lua_close(l);
}

auto
loadPreset(Player& player, UniformController& uniformController) -> void
{
    namespace fs = std::filesystem;
    using namespace util::nfd::literal;

    std::vector<util::nfd::FilterItem> filterItems{
            {"Lua files"_nfd, "lua"_nfd}};

    std::optional const path =
            util::nfd::openDialog(filterItems, fs::current_path() / "presets");

    if(!path) {
        return;
    }

    loadPresetFrom(*path, player, uniformController);
}

auto
pickTerrainFunction(
        Terrain& terrain,
        ShaderController& shaderController,
        ShaderProgram& shaderProgram) -> void
{
    namespace fs = std::filesystem;
    using namespace util::nfd::literal;

    std::vector<util::nfd::FilterItem> filterItems{
            {"Terrain files"_nfd, "lua,frag"_nfd}};

    auto const paths = util::nfd::openDialogMultiple(
            filterItems,
            fs::current_path() / "landscape-scripts");

    for(auto const& path : paths) {
        if(util::endsWith(path.native(), "shape.lua"_nfd)) {
            std::ifstream in(path);
            terrain.loadLua(util::getContents(in));
        }
        else if(util::endsWith(path.native(), "value.frag"_nfd)) {
            std::ifstream in(path);
            shaderController.setValueFunction(
                    shaderProgram,
                    util::getContents(in));
        }
        else if(util::endsWith(path.native(), "color.frag"_nfd)) {
            std::ifstream in(path);
            shaderController.setColorFunction(
                    shaderProgram,
                    util::getContents(in));
        }
    }
}
