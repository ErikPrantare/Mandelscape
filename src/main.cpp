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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

float constexpr G_CLIPPING_PLANE_NEAR = 0.1f;
float constexpr G_CLIPPING_PLANE_FAR  = 10'000'000.0f;

long double constexpr pi = glm::pi<long double>();

float constexpr G_FOV = pi / 2;

glm::vec3 G_VELOCITY(0.0f, 0.0f, 0.0f);

float G_ZOOM_AMOUNT               = 0;
float G_PERSISTENT_ZOOM_DIRECTION = 0;
float G_ZOOM                      = 1.0f;
float constexpr G_MOVEMENT_SPEED  = 1.f;

float G_MESH_OFFSET_X = 0;
float G_MESH_OFFSET_Z = 0;

static void
renderScene(Window& window, Terrain& terrain, Texture& texture)
{
    glEnableVertexAttribArray(0);

    texture.makeActiveOn(GL_TEXTURE0);
    terrain.render();

    glDisableVertexAttribArray(0);
}

void
dispatchEvent(
        Event const& event,
        Config& config,
        Terrain& terrain,
        Camera& camera,
        Window& window);

static void
updateScene(
        Window& window,
        Camera& camera,
        Terrain& terrain,
        Config& config,
        ShaderProgram& program)
{
    util::untilNullopt<Event>(
            [&window] { return window.nextEvent(); },
            dispatchEvent,
            config,
            terrain,
            camera,
            window);

    float constexpr zoomVelocity = 1.f;

    static float lastTimepoint   = glfwGetTime();
    const float currentTimepoint = glfwGetTime();

    const float dt = currentTimepoint - lastTimepoint;
    lastTimepoint  = currentTimepoint;

    camera.setScale(1.0f / G_ZOOM);
    camera.move(dt * G_VELOCITY);
    const float posX = camera.position().x + G_MESH_OFFSET_X;
    const float posZ = camera.position().z + G_MESH_OFFSET_Z;

    const float elevation = terrain.heightAt({posX, posZ});

    if(config.get<Settings::AutoZoom>()) {
        G_ZOOM = 1.f / elevation;
    }
    else {
        G_ZOOM_AMOUNT += G_PERSISTENT_ZOOM_DIRECTION;
        G_ZOOM *= 1.f + dt * zoomVelocity * G_ZOOM_AMOUNT;
    }

    terrain.updateMesh(posX, posZ, G_ZOOM);

    static util::LowPassFilter filterHeight(elevation, 0.01f);

    camera.setCameraHeight(filterHeight(elevation, dt));

    program.setUniformMatrix4("cameraSpace", camera.cameraSpace());
    program.setUniformMatrix4("projection", camera.projection());
    program.setUniformVec2("offset", G_MESH_OFFSET_X, G_MESH_OFFSET_Z);
    program.setUniformInt("iterations", config.get<Settings::Iterations>());

    G_ZOOM_AMOUNT = 0.f;
}

static void
handleInputDown(
        Config& config,
        Terrain& terrain,
        Camera& camera,
        Window& window,
        KeyDown const& key)
{
    switch(key.key) {
    case GLFW_KEY_W: {
        G_VELOCITY.z += G_MOVEMENT_SPEED;
    } break;
    case GLFW_KEY_A: {
        G_VELOCITY.x += -G_MOVEMENT_SPEED;
    } break;
    case GLFW_KEY_S: {
        G_VELOCITY.z += -G_MOVEMENT_SPEED;
    } break;
    case GLFW_KEY_D: {
        G_VELOCITY.x += G_MOVEMENT_SPEED;
    } break;
    case GLFW_KEY_J: {
        G_PERSISTENT_ZOOM_DIRECTION += 1.f;
    } break;
    case GLFW_KEY_K: {
        G_PERSISTENT_ZOOM_DIRECTION += -1.f;
    } break;
    case GLFW_KEY_O: {
        config.on<Settings::AutoZoom>(std::logical_not<bool>());
    } break;
    case GLFW_KEY_R: {
        terrain.updateMesh(camera.position().x, camera.position().z, G_ZOOM);
    } break;
    case GLFW_KEY_H: {
        config.on<Settings::UseDeepShader>(std::logical_not<bool>());
    } break;
    case GLFW_KEY_Q: {
        window.close();
    } break;
    case GLFW_KEY_I: {
        config.on<Settings::Iterations>([](auto x) { return x + 20; });
    } break;
    case GLFW_KEY_U: {
        config.on<Settings::Iterations>([](auto x) { return x - 20; });
    } break;
    default:
        break;
    }
}

static void
handleInputUp(KeyUp const& key)
{
    switch(key.key) {
    case GLFW_KEY_W: {
        G_VELOCITY.z += -G_MOVEMENT_SPEED;
    } break;
    case GLFW_KEY_A: {
        G_VELOCITY.x += G_MOVEMENT_SPEED;
    } break;
    case GLFW_KEY_S: {
        G_VELOCITY.z += G_MOVEMENT_SPEED;
    } break;
    case GLFW_KEY_D: {
        G_VELOCITY.x += -G_MOVEMENT_SPEED;
    } break;
    case GLFW_KEY_J: {
        G_PERSISTENT_ZOOM_DIRECTION += -1.f;
    } break;
    case GLFW_KEY_K: {
        G_PERSISTENT_ZOOM_DIRECTION += 1.f;
    } break;
    default:
        break;
    }
}

static void
handleMouseMove(Config const& config, Camera& camera, int const x, int const y)
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

    camera.lookAt(lookAt);
}

static void
handleMouseButtons(int const button)
{
    int constexpr wheelUp   = 3;
    int constexpr wheelDown = 4;

    switch(button) {
    case wheelUp:
        G_ZOOM_AMOUNT += 1.f;
        break;
    case wheelDown:
        G_ZOOM_AMOUNT += -1.f;
        break;
    default:
        break;
    }
}

void
dispatchEvent(
        Event const& event,
        Config& config,
        Terrain& terrain,
        Camera& camera,
        Window& window)
{
    auto const visitors = util::overload{
            [&](KeyDown const& key) {
                handleInputDown(config, terrain, camera, window, key);
            },
            [&](KeyUp const& key) { handleInputUp(key); },
            [&](MouseMove const& movement) {
                handleMouseMove(config, camera, movement.x, movement.y);
            },
            [&](MouseButtonDown const& button) {
                handleMouseButtons(button.button);
            },
            [&](MouseButtonUp const&) {
            }};
    std::visit(visitors, event);
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

int
main(int argc, char** argv)
{
    Config config = initConfig();

    Window window(config);

    Camera camera(config);

    auto const setMeshOffset = [&camera](double x, double z) mutable {
        float dx = x - G_MESH_OFFSET_X;
        float dz = z - G_MESH_OFFSET_Z;
        camera.setPosition(
                {camera.position().x - dx, 0.0, camera.position().z - dz});
        G_MESH_OFFSET_X = x;
        G_MESH_OFFSET_Z = z;
    };

    Texture texture("textures/texture.png");
    Terrain terrain(setMeshOffset);

    Shader const vertexShader =
            Shader::fromFile("shaders/shader.vert", GL_VERTEX_SHADER);
    Shader const fragmentShader =
            Shader::fromFile("shaders/shader.frag", GL_FRAGMENT_SHADER);

    ShaderProgram program;

    vertexShader.attachTo(program);
    fragmentShader.attachTo(program);
    program.compile();

    config.onStateChange<Settings::UseDeepShader>([&program](bool deep) {
        static Shader const shallowShader =
                Shader::fromFile("shaders/shader.frag", GL_FRAGMENT_SHADER);

        static Shader const deepShader =
                Shader::fromFile("shaders/deepShader.frag", GL_FRAGMENT_SHADER);

        if(deep)
            deepShader.attachTo(program);
        else
            shallowShader.attachTo(program);

        program.compile();
    });

    config.onStateChange<Settings::Iterations>(
            [&terrain](int x) { terrain.setIterations(x); });

    while(window.update()) {
        updateScene(window, camera, terrain, config, program);
        renderScene(window, terrain, texture);
    }

    return 0;
}
