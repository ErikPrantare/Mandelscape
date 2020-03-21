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

GLuint worldLocation;

Camera camera;
Vector3f velocity(0.0f, 0.0f, 0.0f);

bool autoZoom                    = false;
float zoomAmount                 = 0;
float persistentZoomDirection    = 0;
float zoom                       = 1.0f;
float constexpr G_MOVEMENT_SPEED = 1.f;

Terrain* terrain = nullptr;

static void
renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);

    terrain->render();

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

static void
updateScene()
{
    float constexpr zoomVelocity = 1.f;

    static float lastTimeStep = glutGet(GLUT_ELAPSED_TIME);

    float deltaMilliseconds = glutGet(GLUT_ELAPSED_TIME) - lastTimeStep;
    lastTimeStep            = glutGet(GLUT_ELAPSED_TIME);

    float deltaSeconds = deltaMilliseconds / 1000.f;

    if(autoZoom) {
        zoom = 1.f / terrain->heightAt({camera.getPos().x, camera.getPos().z});
    }
    else {
        zoomAmount += persistentZoomDirection;
        zoom *= 1.f + zoomVelocity * deltaSeconds * zoomAmount;
    }

    camera.move((1.f / zoom) * deltaSeconds * velocity);
    terrain->updateMesh(camera.getPos().x, camera.getPos().z, zoom);

    Pipeline world;
    camera.setSize(1.0f / zoom);
    camera.setY(
        1.0f / zoom
        + terrain->heightAt({camera.getPos().x, camera.getPos().z}));
    world.setCamera(camera);
    Matrix4f const transformationMatrix = world.getTrans();
    glUniformMatrix4fv(
        worldLocation,
        1,
        GL_TRUE,
        &transformationMatrix.m[0][0]);
    glutPostRedisplay();

    zoomAmount = 0.f;
}

static void
handleInputDown(unsigned char c, int, int)
{
    switch(c) {
    case 'w':
        velocity.z += G_MOVEMENT_SPEED;
        break;
    case 'a':
        velocity.x += -G_MOVEMENT_SPEED;
        break;
    case 's':
        velocity.z += -G_MOVEMENT_SPEED;
        break;
    case 'd':
        velocity.x += G_MOVEMENT_SPEED;
        break;
    case 'j':
        persistentZoomDirection += 1.f;
        break;
    case 'k':
        persistentZoomDirection += -1.f;
        break;
    case 'o':
        autoZoom = !autoZoom;
        break;
    case 'r':
        terrain->updateMesh(camera.getPos().x, camera.getPos().z, zoom);
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
        velocity.z += -G_MOVEMENT_SPEED;
        break;
    case 'a':
        velocity.x += G_MOVEMENT_SPEED;
        break;
    case 's':
        velocity.z += G_MOVEMENT_SPEED;
        break;
    case 'd':
        velocity.x += -G_MOVEMENT_SPEED;
        break;
    case 'j':
        persistentZoomDirection += -1.f;
        break;
    case 'k':
        persistentZoomDirection += 1.f;
        break;
    default:
        break;
    }
}

static void
handleMouseMove(int x, int y)
{
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

    camera.lookAt(lookAt);
    if(x != 512 || y != 512) {
        glutWarpPointer(512, 512);
        mouseX = 512;
        mouseY = 512;
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
            zoomAmount += 1.f;
        }
        break;
    case wheelDown:
        if(state == GLUT_DOWN) {
            zoomAmount += -1.f;
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

    std::string vs = readFile("shader.vert");
    std::string fs = readFile("shader.frag");

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

    worldLocation = glGetUniformLocation(shaderProgram, "world");
    if(worldLocation == 0xFFFFFFFF) {
        std::cerr << "Failed to find variable world" << std::endl;
        exit(1);
    }
}

int
main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("test");
    glutSetKeyRepeat(false);

    camera.setSize(1024, 1024);
    camera.setClip(0.1, 10'000'000);
    camera.setFOV(pi / 2);
    camera.lookAt(Vector3f(0.0f, 0.0f, 1.0f));
    camera.setUp(Vector3f(0.0f, 1.0f, 0.0f));
    camera.setPos({1.0f, 0, 1.0f});

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

    terrain = new Terrain();

    compileShaders();

    glutMainLoop();

    return 0;
}
