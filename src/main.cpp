#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <variant>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "event.hpp"
#include "metaController.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "terrain.hpp"
#include "config.hpp"
#include "texture.hpp"
#include "window.hpp"
#include "player.hpp"
#include "walkController.hpp"
#include "autoController.hpp"
#include "persistentActionMap.hpp"
#include "momentaryActionsMap.hpp"

long double constexpr pi = glm::pi<long double>();

void
renderScene(
        Terrain& terrain,
        const Player& player,
        Config const& config,
        double dt);

auto
initControls() -> std::pair<MomentaryActionsMap, PersistentActionMap>
{
    auto momentaryMap = MomentaryActionsMap();
    momentaryMap.add(Input::Key::C, TriggerAction::ToggleAutoWalk);
    momentaryMap.add(Input::Key::O, TriggerAction::ToggleAutoZoom);
    momentaryMap.add(Input::Key::I, TriggerAction::IncreaseIterations);
    momentaryMap.add(Input::Key::U, TriggerAction::DecreaseIterations);
    momentaryMap.add(Input::Key::P, TriggerAction::TogglePause);
    momentaryMap.add(Input::Key::Q, TriggerAction::CloseWindow);

    auto persistentMap = PersistentActionMap();
    persistentMap.add(Input::Key::W, PersistentAction::MoveForwards);
    persistentMap.add(Input::Key::S, PersistentAction::MoveBackwards);
    persistentMap.add(Input::Key::A, PersistentAction::MoveLeft);
    persistentMap.add(Input::Key::D, PersistentAction::MoveRight);
    persistentMap.add(Input::Key::J, PersistentAction::ZoomIn);
    persistentMap.add(Input::Key::K, PersistentAction::ZoomOut);

    return {momentaryMap, persistentMap};
}

int
main(int, char**)
{
    auto config = initConfig();
    auto window = Window(config);

    auto terrain = Terrain();
    auto player  = Player();

    auto [momentaryMap, persistentMap] = initControls();

    auto autoControllHeightFunc = [&terrain](glm::dvec2 x) {
        return terrain.heightAt(x);
    };

    auto metacontroller = MetaController{
            std::make_unique<WalkController>(),
            std::make_unique<AutoController>(autoControllHeightFunc)};

    double lastTimepoint = glfwGetTime();
    while(window.update()) {
        const double currentTimepoint = glfwGetTime();
        const double dt               = currentTimepoint - lastTimepoint;
        lastTimepoint                 = currentTimepoint;

        while(auto eventOpt = window.nextEvent()) {
            auto const event = eventOpt.value();

            persistentMap.updateState(event);
            auto const actions = momentaryMap(event);
            for(auto const& action : actions) {
                window.handleMomentaryAction(action);
                terrain.handleMomentaryAction(action);
                metacontroller.handleMomentaryAction(action);
            }
        }

        if(!window.paused()) {
            metacontroller.updateState(persistentMap);

            auto pos = player.position + player.positionOffset;
            auto terrainOffset =
                    terrain.updateMesh(pos.x, pos.z, 1.0 / player.scale);
            auto dOffset          = terrainOffset - player.positionOffset;
            player.positionOffset = terrainOffset;
            player.position -= dOffset;
            player.position.y = terrain.heightAt({pos.x, pos.z});
            metacontroller.update(&player, dt);
        }

        renderScene(terrain, player, config, dt);
    }

    return 0;
}

void
renderScene(
        Terrain& terrain,
        const Player& player,
        Config const& config,
        double dt)
{
    auto camera = Camera(config);
    camera.setScale(player.scale);

    glm::vec3 cameraPosition = player.position;
    cameraPosition.y += player.scale;

    static util::LowPassFilter filteredHeight(cameraPosition.y, 0.01);
    cameraPosition.y = filteredHeight(cameraPosition.y, dt);

    camera.setPosition(cameraPosition);

    // + double(pi), because -z is regarded as the default lookAt forward
    camera.lookAt(
            glm::rotate(
                    glm::dmat4(1.0),
                    player.lookAtOffset.x + double(pi),
                    {0.0, 1.0, 0.0})
            * glm::rotate(
                    glm::dmat4(1.0),
                    player.lookAtOffset.y,
                    {1.0, 0.0, 0.0})
            * glm::dvec4(0.0, 0.0, 1.0, 0.0));

    auto& program = terrain.shaderProgram();
    program.setUniformMatrix4("cameraSpace", camera.cameraSpace());
    program.setUniformMatrix4("projection", camera.projection());

    terrain.render();
}
