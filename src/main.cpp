#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "utils.h"
#include "camera.h"
#include "terrain.h"
#include "config.h"
#include "texture.h"
#include "window.h"
#include "player.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

long double constexpr pi = glm::pi<long double>();

void
renderScene(
        Terrain& terrain,
        const Player& player,
        Config const& config,
        double dt);

Config
initConfig();

int
main(int argc, char** argv)
{
    auto config = initConfig();
    Window window(config);

    Terrain terrain = Terrain();

    Player player;
    double lastTimepoint = glfwGetTime();
    while(window.update()) {
        const double currentTimepoint = glfwGetTime();
        const double dt               = currentTimepoint - lastTimepoint;
        lastTimepoint                 = currentTimepoint;

        while(auto eventOpt = window.nextEvent()) {
            auto const event = eventOpt.value();

            terrain.handleEvent(event);
            player.handleEvent(event);
            window.handleEvent(event);
        }

        auto pos = player.absolutePosition();
        auto terrainOffset =
                terrain.updateMesh(pos.x, pos.z, 1.0 / player.scale());
        player.update(terrainOffset, dt);
        player.setHeight(terrain.heightAt({pos.x, pos.z}));

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
    camera.setScale(player.scale());

    glm::vec3 cameraPosition = player.relativePosition();
    cameraPosition.y += player.scale();

    static util::LowPassFilter filteredHeight(cameraPosition.y, 0.01);
    cameraPosition.y = filteredHeight(cameraPosition.y, dt);

    camera.setPosition(cameraPosition);

    // HACK: -x, look into why it is needed and if it can be resolved cleanly.
    camera.lookAt(
            glm::yawPitchRoll(
                    -player.lookAtOffset().x,
                    player.lookAtOffset().y,
                    0.0)
            * glm::dvec4(0.0, 0.0, 1.0, 0.0));

    auto& program = terrain.shaderProgram();
    program.setUniformMatrix4("cameraSpace", camera.cameraSpace());
    program.setUniformMatrix4("projection", camera.projection());
    static double time = 0.0;
    time += dt;
    program.setUniformFloat("time", time);

    terrain.render();
}

Config
initConfig()
{
    Config conf;
    conf.set<Settings::WindowWidth>(1366);
    conf.set<Settings::WindowHeight>(768);
    conf.set<Settings::ClippingPlaneNear>(0.01);
    conf.set<Settings::ClippingPlaneFar>(150.0);
    conf.set<Settings::FOV>(pi / 2);
    conf.set<Settings::UseDeepShader>(false);
    conf.set<Settings::Iterations>(100);

    return conf;
}
