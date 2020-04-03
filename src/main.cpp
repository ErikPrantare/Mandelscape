#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "math3d.h"
#include "utils.h"
#include "camera.h"
#include "terrain.h"
#include "config.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint G_TEXTURE_LOCATION;
GLuint G_CAMERA_SPACE;
GLuint G_PROJECTION;
GLuint G_OFFSET;

Settings::Config G_CONFIG;

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

Terrain* G_TERRAIN = nullptr;

static void
renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);

    glBindTexture(GL_TEXTURE_2D, G_TEXTURE_LOCATION);
    G_TERRAIN->render();

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

class LowPassFilter {
public:
    [[nodiscard]] float
    operator()(float const& value)
    {
        m_filteredValue *= m_filterAmount;
        m_filteredValue += (1.0f - m_filterAmount) * value;
        return m_filteredValue;
    }

private:
    static float constexpr m_filterAmount = 0.9f;
    float m_filteredValue                 = 0.0f;
};

static void
updateScene()
{
    float constexpr zoomVelocity = 1.f;

    static float lastTimepoint = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
    float currentTimepoint     = glutGet(GLUT_ELAPSED_TIME) / 1000.f;

    float dt      = currentTimepoint - lastTimepoint;
    lastTimepoint = currentTimepoint;

    G_CAMERA.setScale(1.0f / G_ZOOM);
    G_CAMERA.move(dt * G_VELOCITY);
    float posX = G_CAMERA.position().x + G_MESH_OFFSET_X;
    float posZ = G_CAMERA.position().z + G_MESH_OFFSET_Z;

    if(G_AUTO_ZOOM) {
        G_ZOOM = 1.f / G_TERRAIN->heightAt({posX, posZ});
    }
    else {
        G_ZOOM_AMOUNT += G_PERSISTENT_ZOOM_DIRECTION;
        G_ZOOM *= 1.f + dt * zoomVelocity * G_ZOOM_AMOUNT;
    }

    G_TERRAIN->updateMesh(posX, posZ, G_ZOOM);

    static LowPassFilter filterHeight;

    G_CAMERA.setCameraHeight(filterHeight(G_TERRAIN->heightAt({posX, posZ})));

    Matrix4f const cameraSpace = G_CAMERA.cameraSpace();
    Matrix4f const projection  = G_CAMERA.projection();
    glUniformMatrix4fv(G_CAMERA_SPACE, 1, GL_TRUE, &cameraSpace.m[0][0]);
    glUniformMatrix4fv(G_PROJECTION, 1, GL_TRUE, &projection.m[0][0]);
    glUniform2f(G_OFFSET, G_MESH_OFFSET_X, G_MESH_OFFSET_Z);
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
addShader(
        GLuint shaderProgram,
        const std::string& shaderCode,
        GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);

    if(shaderObj == 0) {
        std::cerr << "Error creating shader type " << shaderType << std::endl;
        exit(1);
    }

    const GLchar* p[1];
    p[0] = shaderCode.c_str();
    GLint lengths[1];
    lengths[0] = shaderCode.size();
    glShaderSource(shaderObj, 1, p, lengths);
    glCompileShader(shaderObj);
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderObj, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Error compiling shader type " << shaderType << ": "
                  << "'" << infoLog << "'" << std::endl;

        exit(1);
    }

    glAttachShader(shaderProgram, shaderObj);
}

static void
compileShaders()
{
    GLuint shaderProgram = glCreateProgram();

    if(shaderProgram == 0) {
        std::cerr << "Error creating shader program" << std::endl;
        exit(1);
    }

    std::string vs = readFile("shaders/shader.vert");
    std::string fs = readFile("shaders/shader.frag");

    addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint success         = 0;
    GLchar errorLog[1024] = {0};

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), nullptr, errorLog);

        std::cerr << "Error linking shader program: " << errorLog << std::endl;
        exit(1);
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), nullptr, errorLog);

        std::cerr << "Invalid shader program: " << errorLog << std::endl;
        exit(1);
    }

    glUseProgram(shaderProgram);

    auto loadShader = [&shaderProgram](std::string name, GLuint& loc) {
        loc = glGetUniformLocation(shaderProgram, name.c_str());
        if(loc == 0xFFFFFFFF) {
            std::cerr << "Failed to find variable " << loc << std::endl;
            exit(1);
        }
    };

    loadShader("cameraSpace", G_CAMERA_SPACE);
    loadShader("projection", G_PROJECTION);
    loadShader("offset", G_OFFSET);

    int width, height, nrChannels;
    unsigned char* const data =
            stbi_load("textures/texture.png", &width, &height, &nrChannels, 4);
    if(!data) {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }

    glGenTextures(1, &G_TEXTURE_LOCATION);
    glBindTexture(GL_TEXTURE_2D, G_TEXTURE_LOCATION);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
}

Settings::Config
initConfig()
{
    Settings::Config conf;
    conf.set<Settings::WindowWidth>(1366);
    conf.set<Settings::WindowHeight>(768);

    return conf;
}

int
main(int argc, char** argv)
{
    G_CONFIG = initConfig();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(
            G_CONFIG.get<Settings::WindowHeight>(),
            G_CONFIG.get<Settings::WindowWidth>());
    glutInitWindowPosition(100, 100);
    glutCreateWindow("test");
    glutSetKeyRepeat(false);
    glutSetCursor(GLUT_CURSOR_NONE);

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

    G_TERRAIN = new Terrain(setMeshOffset);

    compileShaders();

    glutWarpPointer(
            G_CONFIG.get<Settings::WindowWidth>() / 2,
            G_CONFIG.get<Settings::WindowHeight>() / 2);
    glutMainLoop();

    return 0;
}
