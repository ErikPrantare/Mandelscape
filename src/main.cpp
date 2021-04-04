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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
#include "persistentActionMap.hpp"
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
loadTerrain(Terrain&, ShaderController&, ShaderProgram&) -> void;

auto
initControls() -> std::pair<MomentaryActionsMap, PersistentActionMap>;

auto
initControlsDvorak() -> std::pair<MomentaryActionsMap, PersistentActionMap>;

auto
main(int argc, char* argv[]) -> int
try {
    auto const args = std::vector(argv, argv + argc);

    auto window = Window({1368, 768});

    // nativefiledialog init
    auto nfdGuard = NFD::Guard();

    auto terrain = Terrain();
    auto player  = Player();

    auto [momentaryMap, persistentMap] = [&args] {
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
            Terrain::valueAttributeLocation);

    auto shaderController  = ShaderController(shaderProgram);
    auto uniformController = UniformController();

    auto serializationController =
            createSerializationController(player, window, uniformController);

    auto time            = 0.0;
    double lastTimepoint = glfwGetTime();

    while(window.update()) {
        double const currentTimepoint = glfwGetTime();
        double const dt               = currentTimepoint - lastTimepoint;
        lastTimepoint                 = currentTimepoint;

        while(auto const eventOpt = window.nextEvent()) {
            auto const event = eventOpt.value();

            persistentMap.updateState(event);

            for(auto const& action : momentaryMap(event)) {
                window.handleMomentaryAction(action);
                metaController.handleMomentaryAction(action);
                uniformController.handleMomentaryAction(action);
                shaderController.handleMomentaryAction(action);
                serializationController.handleMomentaryAction(action);

                if(action == MomentaryAction{Trigger::LoadTerrainFunctions}) {
                    window.pause(true);
                    loadTerrain(terrain, shaderController, shaderProgram);
                    window.pause(false);
                }
            }
        }

        if(!window.paused()) {
            time += dt;

            metaController.updateState(persistentMap);
            uniformController.updateState(persistentMap, dt);

            auto const pos = PlayerHelper(player).truePosition();
            terrain.updateMesh(pos.x, pos.z, 1.0 / player.scale);
            PlayerHelper(player).updateOffset(terrain.offset());
            player.position.y = terrain.heightAt({pos.x, pos.z});
            metaController.update(&player, dt);

            // Do this after .update, as autozoom is dependent on position.y
            player.position.y *= uniformController.yScale();

            terrain.setIterations(uniformController.iterations());
        }

        shaderController.update(shaderProgram);
        uniformController.update(&shaderProgram);
        shaderProgram.setUniformFloat("time", static_cast<float>(time));
        renderScene(player, window.size(), &shaderProgram, dt);
        terrain.render(&shaderProgram);
    }

    return 0;
}
catch(std::exception const& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "Unexpected error, please create an issue at "
              << "https://github.com/ErikPrantare/Mandelscape"
              << " detailing the cause, or contact patched at FractalForums\n";

    return 1;
}
catch(...) {
    std::cerr << "Unexpected error, please create an issue at "
              << "https://github.com/ErikPrantare/Mandelscape"
              << " detailing the cause, or contact patched at FractalForums\n";

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

    auto const camera = Camera(cameraPosition, lookAt, viewSize, player.scale);
    program->setUniformMatrix4("cameraSpace", camera.cameraSpace());
    program->setUniformMatrix4("projection", camera.projection());
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
loadTerrain(
        Terrain& terrain,
        ShaderController& shaderController,
        ShaderProgram& shaderProgram) -> void
{
    namespace fs = std::filesystem;

    std::vector<util::nfd::FilterItem> filterItems{
            {"Terrain files"_nfd, "lua,frag"_nfd}};

    auto const [paths, result] = util::nfd::openDialogMultiple(
            filterItems,
            fs::current_path() / "presets");

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
createSerializationController(
        Player& player,
        Window& window,
        UniformController& uniformController) -> GenericController
{
    return GenericController().withMomentary(
            [&player, &window, &uniformController](MomentaryAction action) {
                auto const paused = window.paused();

                if(action == MomentaryAction{Trigger::Save}) {
                    window.pause(true);
                    save(player, uniformController);
                }
                else if(action == MomentaryAction{Trigger::Load}) {
                    window.pause(true);
                    load(player, uniformController);
                }

                window.pause(paused);
            });
}

[[nodiscard]] auto
initControls() -> std::pair<MomentaryActionsMap, PersistentActionMap>
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

    auto persistentMap = PersistentActionMap();
    persistentMap.add({Key::W}, PersistentAction::MoveForwards);
    persistentMap.add({Key::S}, PersistentAction::MoveBackwards);
    persistentMap.add({Key::A}, PersistentAction::MoveLeft);
    persistentMap.add({Key::D}, PersistentAction::MoveRight);
    persistentMap.add({MouseButton::Left}, PersistentAction::ZoomIn);
    persistentMap.add({MouseButton::Right}, PersistentAction::ZoomOut);
    persistentMap.add({Key::Up}, PersistentAction::IncreaseParam);
    persistentMap.add({Key::Down}, PersistentAction::DecreaseParam);
    persistentMap.add({Key::Key1}, PersistentAction::ChangeFrequency);
    persistentMap.add({Key::Key2}, PersistentAction::ChangeTotalOffset);
    persistentMap.add({Key::Key3}, PersistentAction::ChangeGreenOffset);
    persistentMap.add({Key::Key4}, PersistentAction::ChangeBlueOffset);
    persistentMap.add({Key::Key5}, PersistentAction::ChangeYScale);

    return {momentaryMap, persistentMap};
}

[[nodiscard]] auto
initControlsDvorak() -> std::pair<MomentaryActionsMap, PersistentActionMap>
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

    auto persistentMap = PersistentActionMap();
    persistentMap.add({Key::Comma}, PersistentAction::MoveForwards);
    persistentMap.add({Key::O}, PersistentAction::MoveBackwards);
    persistentMap.add({Key::A}, PersistentAction::MoveLeft);
    persistentMap.add({Key::E}, PersistentAction::MoveRight);
    persistentMap.add({MouseButton::Left}, PersistentAction::ZoomIn);
    persistentMap.add({MouseButton::Right}, PersistentAction::ZoomOut);
    persistentMap.add({Key::Up}, PersistentAction::IncreaseParam);
    persistentMap.add({Key::Down}, PersistentAction::DecreaseParam);
    persistentMap.add({Key::Key1}, PersistentAction::ChangeFrequency);
    persistentMap.add({Key::Key2}, PersistentAction::ChangeTotalOffset);
    persistentMap.add({Key::Key3}, PersistentAction::ChangeGreenOffset);
    persistentMap.add({Key::Key4}, PersistentAction::ChangeBlueOffset);
    persistentMap.add({Key::Key5}, PersistentAction::ChangeYScale);

    return {momentaryMap, persistentMap};
}
