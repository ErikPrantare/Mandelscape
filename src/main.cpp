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
        ShaderProgram* const program,
        glm::vec2 const& terrainOffset,
        Config const& config,
        float dt);

void
updateScene(
        Terrain* const terrain,
        glm::vec2* const terrainOffset,
        Player* const player,
        float dt);

Config
initConfig();

int
main(int argc, char** argv)
{
    Config config = initConfig();

    Window window(config);

    glm::vec2 terrainOffset(0.0, 0.0);
    ShaderProgram shaderProgram;

    Player player;

    Terrain terrain;
    Texture texture("textures/texture.png");

    Shader const vertexShader =
            Shader::fromFile("shaders/shader.vert", GL_VERTEX_SHADER);

    vertexShader.attachTo(shaderProgram);

    config.onStateChange<Settings::UseDeepShader>([&shaderProgram](bool deep) {
        static Shader const shallowShader =
                Shader::fromFile("shaders/shader.frag", GL_FRAGMENT_SHADER);

        static Shader const deepShader =
                Shader::fromFile("shaders/deepShader.frag", GL_FRAGMENT_SHADER);

        if(deep)
            deepShader.attachTo(shaderProgram);
        else
            shallowShader.attachTo(shaderProgram);

        shaderProgram.compile();
    });

    config.onStateChange<Settings::Iterations>(
            [&terrain](int iters) { terrain.setIterations(iters); });

    config.triggerCallbacks();

    EventDispatcher eventDispatcher;

    eventDispatcher.registerCallback<KeyDown>([&](KeyDown const& key) {
        switch(key.code) {
        case GLFW_KEY_I: {
            config.on<Settings::Iterations>([](auto x) { return x + 20; });
        } break;
        case GLFW_KEY_U: {
            config.on<Settings::Iterations>([](auto x) { return x - 20; });
        } break;
        case GLFW_KEY_H: {
            config.on<Settings::UseDeepShader>(std::logical_not<bool>());
        } break;
        default:
            break;
        }
    });

    float lastTimepoint = glfwGetTime();
    while(window.update()) {
        const float currentTimepoint = glfwGetTime();
        const float dt               = currentTimepoint - lastTimepoint;
        lastTimepoint                = currentTimepoint;

        util::untilNullopt<Event>(
                [&window] { return window.nextEvent(); },
                [&eventDispatcher, &player, &window](Event const& event) {
                    eventDispatcher.dispatch(event);
                    player.handleEvent(event);
                    window.handleEvent(event);
                });

        updateScene(&terrain, &terrainOffset, &player, dt);

        renderScene(
                terrain,
                texture,
                player,
                &shaderProgram,
                terrainOffset,
                config,
                dt);
    }

    return 0;
}

void
renderScene(
        Terrain& terrain,
        Texture& texture,
        const Player& player,
        ShaderProgram* const program,
        glm::vec2 const& terrainOffset,
        Config const& config,
        float dt)
{
    glEnableVertexAttribArray(0);

    auto camera = Camera(config);
    camera.setScale(player.m_scale);

    glm::vec3 cameraPosition = player.m_position;
    cameraPosition.y += player.m_scale;

    static util::LowPassFilter filteredHeight(cameraPosition.y, 0.01);
    cameraPosition.y = filteredHeight(cameraPosition.y, dt);

    camera.setPosition(cameraPosition);

    // HACK: -x, look into why it is needed and if it can be resolved cleanly
    camera.lookAt(
            glm::yawPitchRoll(
                    -player.m_lookAtOffset.x,
                    player.m_lookAtOffset.y,
                    0.f)
            * glm::vec4(0.f, 0.f, 1.f, 0.f));

    program->setUniformMatrix4("cameraSpace", camera.cameraSpace());
    program->setUniformMatrix4("projection", camera.projection());
    program->setUniformVec2("offset", terrainOffset.x, terrainOffset.y);
    program->setUniformInt("iterations", config.get<Settings::Iterations>());

    texture.makeActiveOn(GL_TEXTURE0);
    terrain.render();

    glDisableVertexAttribArray(0);
}

void
updateScene(
        Terrain* const terrain,
        glm::vec2* const terrainOffset,
        Player* const player,
        float dt)
{
    player->update(dt);
    float posX = player->m_position.x + terrainOffset->x;
    float posZ = player->m_position.z + terrainOffset->y;

    auto prevOffset = *terrainOffset;
    *terrainOffset  = terrain->updateMesh(posX, posZ, 1.0 / player->m_scale);
    auto dOffset    = *terrainOffset - prevOffset;
    player->m_position.x -= dOffset.x;
    player->m_position.z -= dOffset.y;

    player->m_position.y = terrain->heightAt({posX, posZ});
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
