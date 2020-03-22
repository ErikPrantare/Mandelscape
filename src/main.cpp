#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "math3d.h"
#include "utils.h"
#include "pipeline.h"
#include "camera.h"
#include "terrain.h"

GLuint G_WORLD_LOCATION;

GLuint constexpr G_WINDOW_SIZE_X = 1366;
GLuint constexpr G_WINDOW_SIZE_Y = 768;

float constexpr G_CLIPPING_PLANE_NEAR = 0.1f;
float constexpr G_CLIPPING_PLANE_FAR  = 10'000'000.0f;

float constexpr G_FOV = pi / 2;

Camera G_CAMERA(
    G_WINDOW_SIZE_X,
    G_WINDOW_SIZE_Y,
    G_CLIPPING_PLANE_NEAR,
    G_CLIPPING_PLANE_FAR,
    G_FOV);

Vector3f G_VELOCITY(0.0f, 0.0f, 0.0f);

bool G_AUTO_ZOOM                  = false;
float G_ZOOM_AMOUNT               = 0;
float G_PERSISTENT_ZOOM_DIRECTION = 0;
float G_ZOOM                      = 1.0f;
float constexpr G_MOVEMENT_SPEED  = 1.f;

Terrain* G_TERRAIN = nullptr;

static void
renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);

    G_TERRAIN->render();

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

static void
updateScene() {
    float constexpr zoomVelocity = 1.f;

    static float lastTimepoint = glutGet(GLUT_ELAPSED_TIME)/1000.f;
    float currentTimepoint = glutGet(GLUT_ELAPSED_TIME)/1000.f;

    float dt = currentTimepoint - lastTimepoint;
    lastTimepoint = currentTimepoint;

    if(G_AUTO_ZOOM) {
        G_ZOOM = 1.f
                 / G_TERRAIN->heightAt(
                     {G_CAMERA.position().x, G_CAMERA.position().z});
    }
    else {
        G_ZOOM_AMOUNT += G_PERSISTENT_ZOOM_DIRECTION;
        G_ZOOM *= 1.f + dt*zoomVelocity * G_ZOOM_AMOUNT;
    }

    G_CAMERA.setScale(1.0f / G_ZOOM);
    G_CAMERA.move(dt*G_VELOCITY);

    G_TERRAIN->updateMesh(G_CAMERA.position().x, G_CAMERA.position().z, G_ZOOM);
    G_CAMERA.setCameraHeight(
        G_TERRAIN->heightAt({G_CAMERA.position().x, G_CAMERA.position().z}));

    Pipeline world;
    world.setCamera(G_CAMERA);
    Matrix4f const transformationMatrix = world.getTrans();
    glUniformMatrix4fv(
        G_WORLD_LOCATION,
        1,
        GL_TRUE,
        &transformationMatrix.m[0][0]);
    glutPostRedisplay();

    G_ZOOM_AMOUNT = 0.f;
}

static void
handleInputDown(unsigned char c, int, int) {
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
handleInputUp(unsigned char c, int, int) {
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
handleMouseMove(int x, int y) {
    static int mouseX = 0, mouseY = 0;
    int deltaX = x - mouseX;
    int deltaY = y - mouseY;
    mouseX     = x;
    mouseY     = y;

    static float rotationX = 0.0f;
    rotationX += deltaX / 100.0f;
    static float rotationY = 0.0f;
    rotationY += deltaY / 100.f;
    rotationY = std::clamp(rotationY, float(-pi / 2), float(pi / 2));

    Vector3f lookAt = rotationMatrix({0.0f, -rotationX, 0.0f})
                      * rotationMatrix({rotationY, 0.0f, 0.0f})
                      * Vector3f(0.0f, 0.0f, 1.0f);

    G_CAMERA.lookAt(lookAt);
    if(x != 512 || y != 512) {
        glutWarpPointer(512, 512);
        mouseX = 512;
        mouseY = 512;
    }
}

static void
handleMouseButtons(int button, int state, int x, int y) {
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
initializeGlutCallbacks() {
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
    GLenum shaderType) {
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
compileShaders() {
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

    G_WORLD_LOCATION = glGetUniformLocation(shaderProgram, "world");
    if(G_WORLD_LOCATION == 0xFFFFFFFF) {
        std::cerr << "Failed to find variable world" << std::endl;
        exit(1);
    }
}

int
main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(G_WINDOW_SIZE_X, G_WINDOW_SIZE_Y);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("test");
    glutSetKeyRepeat(false);

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

    G_TERRAIN = new Terrain();

    compileShaders();

    glutMainLoop();

    return 0;
}
