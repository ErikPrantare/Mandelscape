#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "math3d.h"
#include "utils.h"
#include "pipeline.h"
#include "camera.h"
#include "terrain.h"

GLuint VBO;
GLuint IBO;
GLuint terrainVBO;
GLuint terrainIBO;
GLuint worldLocation;

Camera camera;
Vector3f velocity(0.0f,0.0f,0.0f);

float scale = 1.0f;

static void
renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);

    int vertexCount = std::pow((Terrain::granularity-1), 2)*3*2;
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

static void
updateScene()
{
    static float offset = 0;
    static float rotation;
    Pipeline world;
    //world.setPos(std::sin(offset), std::cos(offset), 1.0);
    world.setPos(0.0f, 0.0f, 4.0f);
    //world.rotate(0.0f, rotation/10.0f, 0.0f);
    world.scale(10*scale, 10, 10*scale);
    camera.move(20.0f*velocity);
    world.setCamera(camera);
    offset += 0.01;
    rotation += 0.02;
    glUniformMatrix4fv(worldLocation, 1, GL_TRUE, &world.getTrans().m[0][0]);
    glutPostRedisplay();
}

static void
handleInputDown(unsigned char c, int, int)
{
    switch(c) {
    case 'w':
        velocity.z += 0.01f;
        break;
    case 'a':
        velocity.x += -0.01f;
        break;
    case 's':
        velocity.z += -0.01f;
        break;
    case 'd':
        velocity.x += 0.01f;
        break;
    case 'j':
        velocity.y += -0.01f;
        break;
    case 'k':
        velocity.y += 0.01f;
        break;
    case 'o':
        scale *= 1.1f;
        break;
    }
}

static void
handleInputUp(unsigned char c, int, int)
{
    switch(c) {
    case 'w':
        velocity.z += -0.01f;
        break;
    case 'a':
        velocity.x += 0.01f;
        break;
    case 's':
        velocity.z += 0.01f;
        break;
    case 'd':
        velocity.x += -0.01f;
        break;
    case 'j':
        velocity.y += 0.01f;
        break;
    case 'k':
        velocity.y += -0.01f;
    }
}

static void
initializeGlutCallbacks()
{
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(handleInputDown);
    glutKeyboardUpFunc(handleInputUp);
}

static void
createVertexBuffer()
{
    std::vector<Vector3f> terrain = Terrain().getMeshPoints();

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            terrain.size()*sizeof(Vector3f),
            terrain.data(),
            GL_STATIC_DRAW);
}

static void
createIndexBuffer()
{
    std::vector<int> terrainIndices = Terrain().getMeshIndices();

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            terrainIndices.size()*sizeof(int),
            terrainIndices.data(),
            GL_STATIC_DRAW);
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
        std::cerr 
            << "Error compiling shader type "
            << shaderType << ": "
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

    std::string vs = readFile("shader.vs");
    std::string fs = readFile("shader.fs");

    addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar errorLog[1024] = {0};

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(
                shaderProgram, 
                sizeof(errorLog), 
                nullptr, 
                errorLog);

        std::cerr << "Error linking shader program: " << errorLog << std::endl;
        exit(1);
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(
                shaderProgram, 
                sizeof(errorLog), 
                nullptr, 
                errorLog);

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
main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("test");
    glutSetKeyRepeat(false);

    camera.setSize(1024, 1024);
    camera.setClip(0.1, 100);
    camera.setFOV(pi/2);
    camera.lookAt(Vector3f(0.0f, 0.0f, 1.0f));
    camera.setUp(Vector3f(0.0f, 1.0f, 0.0f));

    initializeGlutCallbacks();

    GLenum res = glewInit();
    if(res != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
        return 1;
    }

    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 0, 0, 0);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(100.0f);

    createVertexBuffer();
    createIndexBuffer();

    compileShaders();

    glutMainLoop();

    return 0;
}
