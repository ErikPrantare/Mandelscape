#include "mesh.hpp"

Mesh::Mesh()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}
