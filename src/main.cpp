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
#include <glm/gtx/euler_angles.hpp>

#include "utils.h"
#include "camera.h"
#include "terrain.h"
#include "config.h"
#include "shader.h"
#include "shaderProgram.h"
#include "texture.h"
#include "window.h"
#include "eventDispatcher.h"
#include "player.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

long double constexpr pi = glm::pi<long double>();

void
renderScene(
        Terrain& terrain,
        Texture& texture,
        const Player& player,
        glm::vec2 const& terrainOffset,
        Config const& config,
        float dt);

Config
initConfig();

int
main(int argc, char** argv)
{
    auto config = initConfig();
    Window window(config);

    Terrain terrain = Terrain();

    EventDispatcher eventDispatcher;

    Player player;
    Texture texture("textures/texture.png");

    float lastTimepoint = glfwGetTime();
    while(window.update()) {
        const float currentTimepoint = glfwGetTime();
        const float dt               = currentTimepoint - lastTimepoint;
        lastTimepoint                = currentTimepoint;

        while(auto eventOpt = window.nextEvent()) {
            auto const event = eventOpt.value();

            eventDispatcher.dispatch(event);
            terrain.handleEvent(event);
            player.handleEvent(event);
            window.handleEvent(event);
        }

        auto pos = player.absolutePosition();
        auto terrainOffset =
                terrain.updateMesh(pos.x, pos.z, 1.0 / player.scale());
        player.update(terrainOffset, dt);
        player.m_position.y = terrain.heightAt({pos.x, pos.z});

        renderScene(terrain, texture, player, terrainOffset, config, dt);
    }

    return 0;
}

void
renderScene(
        Terrain& terrain,
        Texture& texture,
        const Player& player,
        glm::vec2 const& terrainOffset,
        Config const& config,
        float dt)
{
    glEnableVertexAttribArray(0);

    auto camera = Camera(config);
    camera.setScale(player.scale());

    glm::vec3 cameraPosition = player.m_position;
    cameraPosition.y += player.scale();

    static util::LowPassFilter filteredHeight(cameraPosition.y, 0.01);
    cameraPosition.y = filteredHeight(cameraPosition.y, dt);

    camera.setPosition(cameraPosition);

    // HACK: -x, look into why it is needed and if it can be resolved cleanly.
    camera.lookAt(
            glm::yawPitchRoll(
                    -player.lookAtOffset().x,
                    player.lookAtOffset().y,
                    0.f)
            * glm::vec4(0.f, 0.f, 1.f, 0.f));

    ShaderProgram& program = terrain.shaderProgram();
    program.setUniformMatrix4("cameraSpace", camera.cameraSpace());

    program.setUniformMatrix4("projection", camera.projection());

    program.setUniformVec2("offset", terrainOffset.x, terrainOffset.y);

    terrain.shaderProgram().setUniformInt("iterations", terrain.iterations());

    texture.makeActiveOn(GL_TEXTURE0);
    terrain.render();

    glDisableVertexAttribArray(0);
}

Config
initConfig()
{
    Config conf;
    conf.set<Settings::WindowWidth>(1366);
    conf.set<Settings::WindowHeight>(768);
    conf.set<Settings::ClippingPlaneNear>(0.01f);
    conf.set<Settings::ClippingPlaneFar>(150.0f);
    conf.set<Settings::FOV>(pi / 2);
    conf.set<Settings::UseDeepShader>(false);
    conf.set<Settings::Iterations>(100);

    return conf;
}
