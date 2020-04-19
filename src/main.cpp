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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

long double constexpr pi = glm::pi<long double>();

void
renderScene(Terrain& terrain, Texture& texture);

void
dispatchEvent(
        Event const& event,
        Config* const config,
        Camera* const camera,
        Window* const window,
        float const movementSpeed,
        float* const zoomAmount,
        float* const persistentZoomDirection,
        glm::vec3* const velocity);

void
updateScene(
        Window const& window,
        Camera* const camera,
        Terrain* const terrain,
        Config const& config,
        ShaderProgram* const program,
        glm::vec2 const& terrainOffset,
        float const persistentZoomDirection,
        glm::vec3 const& velocity,
        float* const zoomAmount);

void
handleInputDown(
        Config* const config,
        Camera const& camera,
        Window* const window,
        KeyDown const& key,
        float const movementSpeed,
        float* const persistentZoomDirection,
        glm::vec3* const velocity);

void
handleInputUp(
        KeyUp const& key,
        float const movementSpeed,
        float* const persistentZoomDirection,
        glm::vec3* const velocity);

void
handleMouseMove(
        Config const& config,
        Camera* const camera,
        int const x,
        int const y);

void
handleMouseButtons(int button, float* const zoomAmount);

Config
initConfig();

int
main(int argc, char** argv)
{
    float persistentZoomDirection = 0;
    float const movementSpeed     = 1.f;
    float zoomAmount              = 0.f;
    glm::vec3 velocity(0.0, 0.0, 0.0);

    Config config = initConfig();

    Window window(config);

    Camera camera(config);
    glm::vec2 terrainOffset(0.0, 0.0);
    ShaderProgram shaderProgram;

    auto const setMeshOffset = [&camera,
                                &terrainOffset,
                                &shaderProgram](double x, double z) mutable {
        double dx = x - terrainOffset.x;
        double dz = z - terrainOffset.y;
        camera.setPosition(
                {camera.position().x - dx, 0.0, camera.position().z - dz});
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

    EventDispatcher eventDispatcher;

    eventDispatcher.registerKeyDown([&](KeyDown const& key) {
        handleInputDown(
                &config,
                camera,
                &window,
                key,
                movementSpeed,
                &persistentZoomDirection,
                &velocity);
    });

    while(window.update()) {
        util::untilNullopt<Event>(
                [&window] { return window.nextEvent(); },
                [&config,
                 &camera,
                 &window,
                 movementSpeed,
                 &zoomAmount,
                 &persistentZoomDirection,
                 &velocity,
                 &eventDispatcher](Event const& event) {
                    eventDispatcher.dispatch(event);
                    dispatchEvent(
                            event,
                            &config,
                            &camera,
                            &window,
                            movementSpeed,
                            &zoomAmount,
                            &persistentZoomDirection,
                            &velocity);
                });

        updateScene(
                window,
                &camera,
                &terrain,
                config,
                &shaderProgram,
                terrainOffset,
                persistentZoomDirection,
                velocity,
                &zoomAmount);

        renderScene(terrain, texture);
    }

    return 0;
}

void
renderScene(Terrain& terrain, Texture& texture)
{
    glEnableVertexAttribArray(0);

    texture.makeActiveOn(GL_TEXTURE0);
    terrain.render();

    glDisableVertexAttribArray(0);
}

void
dispatchEvent(
        Event const& event,
        Config* const config,
        Camera* const camera,
        Window* const window,
        float const movementSpeed,
        float* const zoomAmount,
        float* const persistentZoomDirection,
        glm::vec3* const velocity)
{
    auto const visitors = util::overload{
            [&](KeyDown const& key) {
                // NOTE: handled in eventDispatcher in main.cpp
            },
            [&](KeyUp const& key) {
                handleInputUp(
                        key,
                        movementSpeed,
                        persistentZoomDirection,
                        velocity);
            },
            [&](MouseMove const& movement) {
                handleMouseMove(*config, camera, movement.x, movement.y);
            },
            [&](MouseButtonDown const& button) {
                handleMouseButtons(button.button, zoomAmount);
            },
            [&](MouseButtonUp const&) {
            }};
    std::visit(visitors, event);
}

void
updateScene(
        Window const& window,
        Camera* const camera,
        Terrain* const terrain,
        Config const& config,
        ShaderProgram* const program,
        glm::vec2 const& terrainOffset,
        float const persistentZoomDirection,
        glm::vec3 const& velocity,
        float* const zoomAmount)
{
    float constexpr zoomVelocity = 1.f;

    static float lastTimepoint   = glfwGetTime();
    const float currentTimepoint = glfwGetTime();

    const float dt = currentTimepoint - lastTimepoint;
    lastTimepoint  = currentTimepoint;

    static float currentTime = 0.0;
    currentTime += dt;

    camera->move(dt * velocity);
    const float posX = camera->position().x + terrainOffset.x;
    const float posZ = camera->position().z + terrainOffset.y;

    static float zoom     = 1.0f;
    const float elevation = terrain->heightAt({posX, posZ});

    if(config.get<Settings::AutoZoom>()) {
        zoom = 1.f / elevation;
    }
    else {
        *zoomAmount += persistentZoomDirection;
        zoom *= 1.f + dt * zoomVelocity * (*zoomAmount);
    }

    camera->setScale(1.0f / zoom);

    terrain->updateMesh(posX, posZ, zoom);

    static util::LowPassFilter filterHeight(elevation, 0.01f);

    camera->setCameraHeight(filterHeight(elevation, dt));

    program->setUniformMatrix4("cameraSpace", camera->cameraSpace());
    program->setUniformMatrix4("projection", camera->projection());
    program->setUniformFloat("time", currentTime);

    *zoomAmount = 0.0;
}

void
handleInputDown(
        Config* const config,
        Camera const& camera,
        Window* const window,
        KeyDown const& key,
        float const movementSpeed,
        float* persistentZoomDirection,
        glm::vec3* const velocity)
{
    switch(key.key) {
    case GLFW_KEY_W: {
        velocity->z += movementSpeed;
    } break;
    case GLFW_KEY_A: {
        velocity->x += -movementSpeed;
    } break;
    case GLFW_KEY_S: {
        velocity->z += -movementSpeed;
    } break;
    case GLFW_KEY_D: {
        velocity->x += movementSpeed;
    } break;
    case GLFW_KEY_J: {
        *persistentZoomDirection += 1.f;
    } break;
    case GLFW_KEY_K: {
        *persistentZoomDirection += -1.f;
    } break;
    case GLFW_KEY_O: {
        config->on<Settings::AutoZoom>(std::logical_not<bool>());
    } break;
    case GLFW_KEY_H: {
        config->on<Settings::UseDeepShader>(std::logical_not<bool>());
    } break;
    case GLFW_KEY_Q: {
        window->close();
    } break;
    default:
        break;
    }
}

void
handleInputUp(
        KeyUp const& key,
        float const movementSpeed,
        float* const persistentZoomDirection,
        glm::vec3* const velocity)
{
    switch(key.key) {
    case GLFW_KEY_W: {
        velocity->z += -movementSpeed;
    } break;
    case GLFW_KEY_A: {
        velocity->x += movementSpeed;
    } break;
    case GLFW_KEY_S: {
        velocity->z += movementSpeed;
    } break;
    case GLFW_KEY_D: {
        velocity->x += -movementSpeed;
    } break;
    case GLFW_KEY_J: {
        *persistentZoomDirection += -1.f;
    } break;
    case GLFW_KEY_K: {
        *persistentZoomDirection += 1.f;
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

void
handleMouseButtons(int button, float* const zoomAmount)
{
    int constexpr wheelUp   = 3;
    int constexpr wheelDown = 4;

    switch(button) {
    case wheelUp:
        *zoomAmount += 1.f;
        break;
    case wheelDown:
        *zoomAmount += -1.f;
        break;
    default:
        break;
    }
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

    return conf;
}
