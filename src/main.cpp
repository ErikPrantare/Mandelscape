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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>

#define NFD_NATIVE
#include <nfd.hpp>

#include "util.hpp"
#include "camera.hpp"
#include "terrain.hpp"
#include "window.hpp"
#include "player.hpp"
#include "playerHelper.hpp"
#include "walkController.hpp"
#include "autoController.hpp"
#include "metaController.hpp"
#include "stateMap.hpp"
#include "momentaryActionsMap.hpp"
#include "shaderController.hpp"
#include "genericController.hpp"
#include "serialization.hpp"

auto
renderScene(
        Player const& player,
        glm::ivec2 viewSize,
        ShaderProgram* program,
        double dt) -> void;

[[nodiscard]] auto
createSerializationController(Player&, Window&, UniformController&)
        -> GenericController;

auto
pickTerrainFunction(Terrain&, ShaderController&, ShaderProgram&) -> void;

auto
saveImage(std::vector<unsigned char>& pixels, glm::ivec2 size) -> void;

auto
saveFramebufferAntialiased(Framebuffer& buffer) -> void;

auto
initControls() -> std::pair<MomentaryActionsMap, StateMap>;

auto
initControlsDvorak() -> std::pair<MomentaryActionsMap, StateMap>;

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
    shaderProgram.bindAttributeLocation(
            "pos",
            Terrain::positionAttributeLocation);
    shaderProgram.bindAttributeLocation(
            "val",
            Terrain::valueAttributeLocation);
    shaderProgram.bindAttributeLocation(
            "inside_",
            Terrain::insideAttributeLocation);
    shaderProgram.bindAttributeLocation(
            "normal_",
            Terrain::normalAttributeLocation);

    auto shaderController  = ShaderController(shaderProgram);
    auto uniformController = UniformController();

    auto serializationController =
            createSerializationController(player, window, uniformController);

    auto time            = 0.0;
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

    auto const render = [&skybox,
                         &shaderController,
                         &shaderProgram,
                         &uniformController,
                         &terrain,
                         &player,
                         &window](double currentTime, double dt) {
        skybox.activateOn(shaderProgram);
        shaderController.update(shaderProgram);
        uniformController.update(&shaderProgram);
        shaderProgram.setUniformFloat("time", static_cast<float>(currentTime));
        renderScene(player, window.size(), &shaderProgram, dt);
        terrain.render(shaderProgram);
    };

    auto const handleAuxilliaryActions = [&render,
                                          &window,
                                          &shaderController,
                                          &terrain,
                                          &shaderProgram](
                                                 MomentaryAction const& action,
                                                 double currentTime,
                                                 double dt) {
        if(action == MomentaryAction{Trigger::LoadTerrainFunctions}) {
            auto resumeGuard = window.suspend();
            pickTerrainFunction(terrain, shaderController, shaderProgram);
        }
        else if(action == MomentaryAction{Trigger::TakeScreenshot}) {
            auto screenshotBuffer = Framebuffer(2 * window.size());
            screenshotBuffer.bind();
            glViewport(0, 0, 2 * window.size().x, 2 * window.size().y);

            render(currentTime, dt);

            saveFramebufferAntialiased(screenshotBuffer);

            Framebuffer::unbind();
            glViewport(0, 0, window.size().x, window.size().y);
        }
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
                serializationController.handleMomentaryAction(action);
                handleAuxilliaryActions(action, time, dt);
            }
        }

        if(!window.paused()) {
            time += dt;

            metaController.updateState(stateMap);
            uniformController.updateState(stateMap, dt);

            auto const pos = PlayerHelper(player).truePosition();
            terrain.updateMesh({pos.x, pos.z}, 1.0 / player.scale);
            PlayerHelper(player).updateOffset(terrain.offset());
            player.position.y = terrain.heightAt({pos.x, pos.z});
            metaController.update(&player, dt);

            // Do this after .update, as autozoom is dependent on position.y
            player.position.y *= uniformController.yScale();

            terrain.setIterations(uniformController.iterations());
        }

        render(time, dt);
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
        ShaderProgram* const program,
        double dt)
{
    glm::dvec3 cameraPosition = player.position;
    cameraPosition.y += player.scale;
    static util::LowPassFilter filteredHeight(cameraPosition.y, 0.01);
    cameraPosition.y = filteredHeight(cameraPosition.y, dt);

    // + util::pi, because -z is regarded as the default lookAt forward
    auto const lookAt = glm::rotate(
                                glm::dmat4(1.0),
                                player.lookAtOffset.x + util::pi,
                                {0.0, 1.0, 0.0})
                        * glm::rotate(
                                glm::dmat4(1.0),
                                player.lookAtOffset.y,
                                {1.0, 0.0, 0.0})
                        * glm::dvec4(0.0, 0.0, 1.0, 0.0);

    // CPP20 {.x = ...}
    auto const camera =
            Camera({cameraPosition, lookAt, viewSize, player.scale});
    program->setUniformMatrix4("cameraSpace", camera.cameraSpace());
    program->setUniformMatrix4("projection", camera.projection());
    program->setUniformVec3("lookAt", lookAt);
    program->setUniformVec3("playerPos", cameraPosition);
}

auto operator""_nfd(char const* str, size_t size)
        -> std::filesystem::path::string_type
{
    return {str, str + size};
}

auto
save(Player const& player, UniformController const& uniformController) -> void
{
    namespace fs = std::filesystem;

    std::vector<util::nfd::FilterItem> filterItems{
            {"Lua files"_nfd, "lua"_nfd}};

    auto const [path, result] = util::nfd::saveDialog(
            filterItems,
            fs::current_path(),
            "save.lua"_nfd);

    if(result != NFD_OKAY) {
        return;
    }

    std::ofstream out(path);
    serialize(out, player, "player");
    serialize(out, uniformController, "uniformController");
}

auto
load(Player& player, UniformController& uniformController) -> void
{
    namespace fs = std::filesystem;

    std::vector<util::nfd::FilterItem> filterItems{
            {"Lua files"_nfd, "lua"_nfd}};

    auto const [path, result] =
            util::nfd::openDialog(filterItems, fs::current_path());

    if(result != NFD_OKAY) {
        return;
    }

    std::ifstream in(path);
    lua_State* l = luaL_newstate();
    luaL_dostring(l, util::getContents(in).c_str());

    lua_getglobal(l, "player");
    player = util::lua::to<Player>(l, -1);
    lua_pop(l, 1);

    lua_getglobal(l, "uniformController");
    uniformController = util::lua::to<UniformController>(l, -1);
    lua_close(l);
}

auto
pickTerrainFunction(
        Terrain& terrain,
        ShaderController& shaderController,
        ShaderProgram& shaderProgram) -> void
{
    namespace fs = std::filesystem;

    std::vector<util::nfd::FilterItem> filterItems{
            {"Terrain files"_nfd, "lua,frag"_nfd}};

    auto const [paths, result] = util::nfd::openDialogMultiple(
            filterItems,
            fs::current_path() / "landscape-scripts");

    if(result != NFD_OKAY) {
        return;
    }

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

auto
saveImage(std::vector<unsigned char>& pixels, glm::ivec2 size) -> void
{
    std::time_t const t = std::time(nullptr);
    std::tm const tm    = *std::localtime(&t);
    std::stringstream buffer;
    buffer << std::put_time(&tm, "%Y_%m_%d-%H_%M_%S");

    namespace fs          = std::filesystem;
    std::string const dir = "screenshots";
    if(!fs::is_directory(dir) || !fs::exists(dir)) {
        fs::create_directory(dir);
    }

    std::string filename = dir + "/" + buffer.str() + ".png";

    stbi_flip_vertically_on_write(1);
    stbi_write_png(filename.c_str(), size.x, size.y, 3, pixels.data(), 0);
}

auto
saveFramebufferAntialiased(Framebuffer& buffer) -> void
{
    auto const renderedSize    = buffer.size();
    auto const antiAliasedSize = renderedSize / 2;
    auto const pixels          = buffer.readPixels();

    // CPP23 3z * outputSize.x
    std::vector<unsigned char> antiAliasedImage(
            3 * static_cast<long>(antiAliasedSize.x) * antiAliasedSize.y);

    stbir_resize_uint8(
            pixels.data(),
            renderedSize.x,
            renderedSize.y,
            0,
            antiAliasedImage.data(),
            antiAliasedSize.x,
            antiAliasedSize.y,
            0,
            3);

    saveImage(antiAliasedImage, antiAliasedSize);
}
auto
createSerializationController(
        Player& player,
        Window& window,
        UniformController& uniformController) -> GenericController
{
    return GenericController().withMomentary(
            [&player, &window, &uniformController](MomentaryAction action) {
                if(action == MomentaryAction{Trigger::Save}) {
                    auto const resumeGuard = window.suspend();
                    save(player, uniformController);
                }
                else if(action == MomentaryAction{Trigger::Load}) {
                    auto const resumeGuard = window.suspend();
                    load(player, uniformController);
                }
            });
}

[[nodiscard]] auto
initControls() -> std::pair<MomentaryActionsMap, StateMap>
{
    using namespace Input;

    auto momentaryMap = MomentaryActionsMap();
    momentaryMap.add({Key::C}, Trigger::ToggleAutoWalk);
    momentaryMap.add({Key::O}, Trigger::ToggleAutoZoom);
    momentaryMap.add({Key::K}, Trigger::IncreaseIterations);
    momentaryMap.add({Key::J}, Trigger::DecreaseIterations);
    momentaryMap.add({Key::F}, Trigger::ToggleFastMode);
    momentaryMap.add({Key::H}, Trigger::SwitchShader);
    momentaryMap.add({Key::P}, Trigger::TogglePause);
    momentaryMap.add({Key::X}, Trigger::TakeScreenshot);
    momentaryMap.add({Key::Q}, Trigger::CloseWindow);
    momentaryMap.add({Key::Escape}, Trigger::CloseWindow);
    momentaryMap.add({Key::S, (int)Mod::Control}, Trigger::Save);
    momentaryMap.add({Key::O, (int)Mod::Control}, Trigger::Load);
    momentaryMap.add(
            {Key::L, (int)Mod::Control},
            Trigger::LoadTerrainFunctions);

    auto stateMap = StateMap();
    stateMap.add({Key::W}, State::MovingForwards);
    stateMap.add({Key::S}, State::MovingBackwards);
    stateMap.add({Key::A}, State::MovingLeft);
    stateMap.add({Key::D}, State::MovingRight);
    stateMap.add({MouseButton::Left}, State::ZoomingIn);
    stateMap.add({MouseButton::Right}, State::ZoomingOut);
    stateMap.add({Key::Up}, State::IncreasingParameter);
    stateMap.add({Key::Down}, State::DecreasingParameter);
    stateMap.add({Key::Key1}, State::ChangingFrequency);
    stateMap.add({Key::Key2}, State::ChangingTotalOffset);
    stateMap.add({Key::Key3}, State::ChangingGreenOffset);
    stateMap.add({Key::Key4}, State::ChangingBlueOffset);
    stateMap.add({Key::Key5}, State::ChangingYScale);

    return {momentaryMap, stateMap};
}

[[nodiscard]] auto
initControlsDvorak() -> std::pair<MomentaryActionsMap, StateMap>
{
    using namespace Input;

    auto momentaryMap = MomentaryActionsMap();
    momentaryMap.add({Key::J}, Trigger::ToggleAutoWalk);
    momentaryMap.add({Key::R}, Trigger::ToggleAutoZoom);
    momentaryMap.add({Key::T}, Trigger::IncreaseIterations);
    momentaryMap.add({Key::H}, Trigger::DecreaseIterations);
    momentaryMap.add({Key::F}, Trigger::ToggleFastMode);
    momentaryMap.add({Key::D}, Trigger::SwitchShader);
    momentaryMap.add({Key::P}, Trigger::TogglePause);
    momentaryMap.add({Key::X}, Trigger::TakeScreenshot);
    momentaryMap.add({Key::Q}, Trigger::CloseWindow);
    momentaryMap.add({Key::Escape}, Trigger::CloseWindow);
    momentaryMap.add({Key::S, (int)Mod::Control}, Trigger::Save);
    momentaryMap.add({Key::O, (int)Mod::Control}, Trigger::Load);
    momentaryMap.add(
            {Key::L, (int)Mod::Control},
            Trigger::LoadTerrainFunctions);
    momentaryMap.add({Key::L}, Trigger::ToggleLighting);

    auto stateMap = StateMap();
    stateMap.add({Key::Comma}, State::MovingForwards);
    stateMap.add({Key::O}, State::MovingBackwards);
    stateMap.add({Key::A}, State::MovingLeft);
    stateMap.add({Key::E}, State::MovingRight);
    stateMap.add({MouseButton::Left}, State::ZoomingIn);
    stateMap.add({MouseButton::Right}, State::ZoomingOut);
    stateMap.add({Key::Up}, State::IncreasingParameter);
    stateMap.add({Key::Down}, State::DecreasingParameter);
    stateMap.add({Key::Key1}, State::ChangingFrequency);
    stateMap.add({Key::Key2}, State::ChangingTotalOffset);
    stateMap.add({Key::Key3}, State::ChangingGreenOffset);
    stateMap.add({Key::Key4}, State::ChangingBlueOffset);
    stateMap.add({Key::Key5}, State::ChangingYScale);

    return {momentaryMap, stateMap};
}
