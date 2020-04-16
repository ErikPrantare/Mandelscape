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
        Camera& camera);

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
            camera);

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

    G_ZOOM_AMOUNT = 0.f;
}

static void
handleInputDown(
        Config& config,
        Terrain& terrain,
        Camera& camera,
        unsigned char c)
{
    switch(c) {
    case 'w':
        G_VELOCITY.z += G_MOVEMENT_SPEED;
        break;
    case 'a':
        G_VELOCITY.x += -G_MOVEMENT_SPEED;
        break;
    case 's':
        G_VELOCITY.z += -G_MOVEMENT_SPEED;
        break;
    case 'd':
        G_VELOCITY.x += G_MOVEMENT_SPEED;
        break;
    case 'j':
        G_PERSISTENT_ZOOM_DIRECTION += 1.f;
        break;
    case 'k':
        G_PERSISTENT_ZOOM_DIRECTION += -1.f;
        break;
    case 'o':
        config.on<Settings::AutoZoom>(std::logical_not<bool>());
        break;
    case 'r':
        terrain.updateMesh(camera.position().x, camera.position().z, G_ZOOM);
        break;
    case 'h':
        config.on<Settings::UseDeepShader>(std::logical_not<bool>());
        break;
    case 'q':
        exit(0);
        break;
    default:
        break;
    }
}

static void
handleInputUp(unsigned char c)
{
    switch(c) {
    case 'w':
        G_VELOCITY.z += -G_MOVEMENT_SPEED;
        break;
    case 'a':
        G_VELOCITY.x += G_MOVEMENT_SPEED;
        break;
    case 's':
        G_VELOCITY.z += G_MOVEMENT_SPEED;
        break;
    case 'd':
        G_VELOCITY.x += -G_MOVEMENT_SPEED;
        break;
    case 'j':
        G_PERSISTENT_ZOOM_DIRECTION += -1.f;
        break;
    case 'k':
        G_PERSISTENT_ZOOM_DIRECTION += 1.f;
        break;
    default:
        break;
    }
}

static void
handleMouseMove(Config& config, Camera& camera, int x, int y)
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

    // if(x != halfWindowSizeX || y != halfWindowSizeY) {
    // glutWarpPointer(halfWindowSizeX, halfWindowSizeY);
    // mouseX = halfWindowSizeX;
    // mouseY = halfWindowSizeY;
    //}
}

static void
handleMouseButtons(int button)
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
        Camera& camera)
{
    auto const visitors = util::overload{
            [&](KeyDown const& key) {
                handleInputDown(config, terrain, camera, key.code);
            },
            [&](KeyUp const& key) { handleInputUp(key.code); },
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

    return conf;
}

int
main(int argc, char** argv)
{
    Config config = initConfig();

    Window window(config);

    Camera camera(
            config.get<Settings::WindowWidth>(),
            config.get<Settings::WindowHeight>(),
            config.get<Settings::ClippingPlaneNear>(),
            config.get<Settings::ClippingPlaneFar>(),
            config.get<Settings::FOV>());

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

    while(!glfwWindowShouldClose(window())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        updateScene(window, camera, terrain, config, program);
        renderScene(window, terrain, texture);
        glfwSwapBuffers(window());
    }

    return 0;
}
