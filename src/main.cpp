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
        Camera* const camera,
        ShaderProgram* const program,
        glm::vec2 const& terrainOffset,
        Config const& config,
        float dt);

void
updateScene(
        Terrain* const terrain,
        glm::vec2 const& terrainOffset,
        Player* const player,
        float dt);

void
handleInputDown(
        Config* const config,
        Camera const& camera,
        Window* const window,
        KeyDown const& key);

void
handleMouseMove(
        Config const& config,
        Camera* const camera,
        int const x,
        int const y);

Config
initConfig();

int
main(int argc, char** argv)
{
    Config config = initConfig();

    Window window(config);

    Camera camera(config);
    glm::vec2 terrainOffset(0.0, 0.0);
    ShaderProgram shaderProgram;

    Player player;
    auto const setMeshOffset = [&camera,
                                &player,
                                &terrainOffset,
                                &shaderProgram](double x, double z) mutable {
        double dx         = x - terrainOffset.x;
        double dz         = z - terrainOffset.y;
        player.m_position = {
                player.m_position.x - dx,
                0.0,
                player.m_position.z - dz};
        shaderProgram.setUniformVec2("offset", x, z);
        terrainOffset = {x, z};
    };

    Terrain terrain(setMeshOffset);
    Texture texture("textures/texture.png");

    Shader const vertexShader =
            Shader::fromFile("shaders/shader.vert", GL_VERTEX_SHADER);
    Shader const fragmentShader =
            Shader::fromFile("shaders/shader.frag", GL_FRAGMENT_SHADER);

    vertexShader.attachTo(shaderProgram);
    fragmentShader.attachTo(shaderProgram);
    shaderProgram.compile();

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

    EventDispatcher eventDispatcher;

    eventDispatcher.registerCallback<KeyDown>([&](KeyDown const& key) {
        handleInputDown(&config, camera, &window, key);
    });

    eventDispatcher.registerCallback<MouseMove>([&](MouseMove const& movement) {
        handleMouseMove(config, &camera, movement.x, movement.y);
    });

    float lastTimepoint = glfwGetTime();
    while(window.update()) {
        const float currentTimepoint = glfwGetTime();
        const float dt               = currentTimepoint - lastTimepoint;
        lastTimepoint                = currentTimepoint;

        util::untilNullopt<Event>(
                [&window] { return window.nextEvent(); },
                [&eventDispatcher, &player](Event const& event) {
                    eventDispatcher.dispatch(event);
                    player.handleEvent(event);
                });

        updateScene(&terrain, terrainOffset, &player, dt);

        renderScene(
                terrain,
                texture,
                player,
                &camera,
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
        Camera* const camera,
        ShaderProgram* const program,
        glm::vec2 const& terrainOffset,
        Config const& config,
        float dt)
{
    glEnableVertexAttribArray(0);

    camera->setScale(player.m_scale);

    glm::vec3 cameraPosition = player.m_position;
    cameraPosition.y += player.m_scale;

    static util::LowPassFilter filteredHeight(cameraPosition.y, 0.01);
    cameraPosition.y = filteredHeight(cameraPosition.y, dt);

    camera->setPosition(cameraPosition);

    program->setUniformMatrix4("cameraSpace", camera->cameraSpace());
    program->setUniformMatrix4("projection", camera->projection());
    program->setUniformVec2("offset", terrainOffset.x, terrainOffset.y);
    program->setUniformInt("iterations", config.get<Settings::Iterations>());

    texture.makeActiveOn(GL_TEXTURE0);
    terrain.render();

    glDisableVertexAttribArray(0);
}

void
updateScene(
        Terrain* const terrain,
        glm::vec2 const& terrainOffset,
        Player* const player,
        float dt)
{
    player->update(dt);
    float posX = player->m_position.x + terrainOffset.x;
    float posZ = player->m_position.z + terrainOffset.y;

    // TODO: return new offset instead of callback. Use [[nodiscard]]
    terrain->updateMesh(posX, posZ, 1.0 / player->m_scale);

    player->m_position.y = terrain->heightAt({posX, posZ});
}

void
handleInputDown(
        Config* const config,
        Camera const& camera,
        Window* const window,
        KeyDown const& key)
{
    switch(key.key) {
    case GLFW_KEY_Q: {
        window->close();
    } break;
    case GLFW_KEY_I: {
        config->on<Settings::Iterations>([](auto x) { return x + 20; });
    } break;
    case GLFW_KEY_U: {
        config->on<Settings::Iterations>([](auto x) { return x - 20; });
    } break;
    default:
        break;
    }
}

void
handleMouseMove(
        Config const& config,
        Camera* const camera,
        int const x,
        int const y)
{
    int const halfWindowSizeX = config.get<Settings::WindowWidth>() / 2;
    int const halfWindowSizeY = config.get<Settings::WindowHeight>() / 2;

    static int mouseX = halfWindowSizeX;
    static int mouseY = halfWindowSizeY;

    int deltaX = x - mouseX;
    int deltaY = y - mouseY;
    mouseX     = x;
    mouseY     = y;

    static float rotationX = 16.75997f;
    static float rotationY = 0.94000f;

    rotationX += deltaX / 100.0f;
    rotationY += deltaY / 100.f;
    rotationY = std::clamp(
            rotationY,
            float(-pi / 2 + 0.001),
            float(pi / 2 - 0.001));

    auto const lookAt = glm::rotate(
                                glm::rotate(
                                        glm::mat4(1.0f),
                                        rotationX,
                                        {0.0f, -1.0f, 0.0f}),
                                rotationY,
                                {1.0f, 0.0f, 0.0f})
                        * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    camera->lookAt(lookAt);
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
    conf.set<Settings::AutoZoom>(false);
    conf.set<Settings::Iterations>(100);

    return conf;
}
