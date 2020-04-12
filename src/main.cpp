#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "math3d.h"
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

Config G_CONFIG;

// XXX: Gives segmentation fault if not pointer
// Let it be pointer for now, and just remove it from
// global namespace when we switch to GLFW
std::unique_ptr<ShaderProgram> G_SHADER_PROGRAM = nullptr;
std::unique_ptr<Texture> G_TEXTURE              = nullptr;

float constexpr G_CLIPPING_PLANE_NEAR = 0.1f;
float constexpr G_CLIPPING_PLANE_FAR  = 10'000'000.0f;

float constexpr G_FOV = pi / 2;

Camera G_CAMERA;

Vector3f G_VELOCITY(0.0f, 0.0f, 0.0f);

bool G_AUTO_ZOOM                  = false;
float G_ZOOM_AMOUNT               = 0;
float G_PERSISTENT_ZOOM_DIRECTION = 0;
float G_ZOOM                      = 1.0f;
float constexpr G_MOVEMENT_SPEED  = 1.f;

float G_MESH_OFFSET_X = 0;
float G_MESH_OFFSET_Z = 0;

std::unique_ptr<Terrain> G_TERRAIN = nullptr;

static void
renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);

    G_TEXTURE->makeActiveOn(GL_TEXTURE0);
    G_TERRAIN->render();

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

class LowPassFilter {
public:
    LowPassFilter(float init, float amount) :
                m_filteredValue(init),
                m_amount(amount){};

    float
    operator()(float const newValue, float const weight = 1.0f)
    {
        const float factor = std::pow(m_amount, weight);

        m_filteredValue = factor * m_filteredValue + (1.0f - factor) * newValue;
        return m_filteredValue;
    }

private:
    float m_filteredValue;
    float const m_amount;
};

static void
updateScene()
{
    float constexpr zoomVelocity = 1.f;

    static float lastTimepoint   = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
    const float currentTimepoint = glutGet(GLUT_ELAPSED_TIME) / 1000.f;

    const float dt = currentTimepoint - lastTimepoint;
    lastTimepoint  = currentTimepoint;

    G_CAMERA.setScale(1.0f / G_ZOOM);
    G_CAMERA.move(dt * G_VELOCITY);
    const float posX = G_CAMERA.position().x + G_MESH_OFFSET_X;
    const float posZ = G_CAMERA.position().z + G_MESH_OFFSET_Z;

    const float elevation = G_TERRAIN->heightAt({posX, posZ});

    if(G_AUTO_ZOOM) {
        G_ZOOM = 1.f / elevation;
    }
    else {
        G_ZOOM_AMOUNT += G_PERSISTENT_ZOOM_DIRECTION;
        G_ZOOM *= 1.f + dt * zoomVelocity * G_ZOOM_AMOUNT;
    }

    G_TERRAIN->updateMesh(posX, posZ, G_ZOOM);

    static LowPassFilter filterHeight(elevation, 0.01f);

    G_CAMERA.setCameraHeight(filterHeight(elevation, dt));

    G_SHADER_PROGRAM->setUniform("cameraSpace", G_CAMERA.cameraSpace());
    G_SHADER_PROGRAM->setUniform("projection", G_CAMERA.projection());
    G_SHADER_PROGRAM->setUniform("offset", G_MESH_OFFSET_X, G_MESH_OFFSET_Z);

    glutPostRedisplay();
    G_ZOOM_AMOUNT = 0.f;
}

static void
handleInputDown(unsigned char c, int, int)
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
        G_AUTO_ZOOM = !G_AUTO_ZOOM;
        break;
    case 'r':
        G_TERRAIN->updateMesh(
                G_CAMERA.position().x,
                G_CAMERA.position().z,
                G_ZOOM);
        break;
    case 'h':
        G_CONFIG.on<Settings::UseDeepShader>(std::logical_not<bool>());
        break;
    case 'q':
        exit(0);
        break;
    default:
        break;
    }
}

static void
handleInputUp(unsigned char c, int, int)
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
handleMouseMove(int x, int y)
{
    int halfWindowSizeX = G_CONFIG.get<Settings::WindowWidth>() / 2;
    int halfWindowSizeY = G_CONFIG.get<Settings::WindowHeight>() / 2;

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

    Vector3f lookAt = rotationMatrix({0.0f, -rotationX, 0.0f})
                      * rotationMatrix({rotationY, 0.0f, 0.0f})
                      * Vector3f(0.0f, 0.0f, 1.0f);

    G_CAMERA.lookAt(lookAt);

    if(x != halfWindowSizeX || y != halfWindowSizeY) {
        glutWarpPointer(halfWindowSizeX, halfWindowSizeY);
        mouseX = halfWindowSizeX;
        mouseY = halfWindowSizeY;
    }
}

static void
handleMouseButtons(int button, int state, int x, int y)
{
    int constexpr wheelUp   = 3;
    int constexpr wheelDown = 4;

    switch(button) {
    case wheelUp:
        if(state == GLUT_DOWN) {
            G_ZOOM_AMOUNT += 1.f;
        }
        break;
    case wheelDown:
        if(state == GLUT_DOWN) {
            G_ZOOM_AMOUNT += -1.f;
        }
        break;
    default:
        break;
    }
}

static void
initializeGlutCallbacks()
{
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(handleInputDown);
    glutKeyboardUpFunc(handleInputUp);
    glutPassiveMotionFunc(handleMouseMove);
    glutMouseFunc(handleMouseButtons);
}

static void
compileShaders()
{
    static Shader const vertexShader =
            Shader::fromFile("shaders/shader.vert", GL_VERTEX_SHADER);
    static Shader const fragmentShader =
            Shader::fromFile("shaders/shader.frag", GL_FRAGMENT_SHADER);

    G_SHADER_PROGRAM = std::make_unique<ShaderProgram>();

    vertexShader.attachTo(*G_SHADER_PROGRAM);
    fragmentShader.attachTo(*G_SHADER_PROGRAM);
    G_SHADER_PROGRAM->compile();
}

Config
initConfig()
{
    Config conf;
    conf.set<Settings::WindowWidth>(1366);
    conf.set<Settings::WindowHeight>(768);
    conf.set<Settings::UseDeepShader>(false);

    conf.onStateChange<Settings::UseDeepShader>([](bool deep) {
        static Shader const shallowShader =
                Shader::fromFile("shaders/shader.frag", GL_FRAGMENT_SHADER);

        static Shader const deepShader =
                Shader::fromFile("shaders/deepShader.frag", GL_FRAGMENT_SHADER);

        if(deep)
            deepShader.attachTo(*G_SHADER_PROGRAM);
        else
            shallowShader.attachTo(*G_SHADER_PROGRAM);

        G_SHADER_PROGRAM->compile();
    });

    return conf;
}

int
main(int argc, char** argv)
{
    G_CONFIG = initConfig();

    glutInit(&argc, argv);
    Window window(G_CONFIG);

    initializeGlutCallbacks();

    GLenum res = glewInit();
    if(res != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
        return 1;
    }

    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 1, 1, 0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(10'000'000.0f);

    G_CAMERA =
            Camera(G_CONFIG.get<Settings::WindowWidth>(),
                   G_CONFIG.get<Settings::WindowHeight>(),
                   G_CLIPPING_PLANE_NEAR,
                   G_CLIPPING_PLANE_FAR,
                   G_FOV);

    auto const setMeshOffset = [](double x, double z) {
        float dx = x - G_MESH_OFFSET_X;
        float dz = z - G_MESH_OFFSET_Z;
        G_CAMERA.setPosition(
                {G_CAMERA.position().x - dx, 0.0, G_CAMERA.position().z - dz});
        G_MESH_OFFSET_X = x;
        G_MESH_OFFSET_Z = z;
    };

    G_TEXTURE = std::make_unique<Texture>("textures/texture.png");
    G_TERRAIN = std::make_unique<Terrain>(setMeshOffset);

    compileShaders();

    glutWarpPointer(
            G_CONFIG.get<Settings::WindowWidth>() / 2,
            G_CONFIG.get<Settings::WindowHeight>() / 2);
    glutMainLoop();

    return 0;
}
