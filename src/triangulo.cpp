#include "triangulo.hpp"
#include <iostream>

unsigned int Triangulo::s_VAO = 0;
unsigned int Triangulo::s_VBO = 0;

Triangulo::Triangulo(const glm::vec2& pos, float lado, const glm::vec3& cor) {
    m_posicao = glm::vec3(pos.x, pos.y, 0.0f);
    m_escala = glm::vec2(lado, lado);
    m_cor = cor;
    m_rotacao = 0.0f;
    m_selecionado = false;
}

Triangulo::~Triangulo() {

}

void Triangulo::desenhar() const {
    glBindVertexArray(s_VAO);
    
    glDrawArrays(GL_LINE_LOOP, 0, 3);
    
    glBindVertexArray(0);
}

bool Triangulo::foiSelecionado(const glm::vec2& pontoMouseMundo) {

    glm::mat4 invModel = glm::inverse(getMatrizModelo());
    glm::vec4 localPos = invModel * glm::vec4(pontoMouseMundo.x, pontoMouseMundo.y, 0.0, 1.0);

    if (localPos.x >= -0.5 && localPos.x <= 0.5 && localPos.y >= -0.5 && localPos.y <= 0.5) {
        return true;
    }
    return false;
}

float Triangulo::calcularArea() const {
    return 0.5f * m_escala.x * m_escala.y;
}

float Triangulo::calcularPerimetro() const {
    return 0.0f;
}

void Triangulo::setupMesh() {

    float vertices[] = {
         0.0f,    0.5f, 0.0f,
        -0.433f, -0.25f, 0.0f,
         0.433f, -0.25f, 0.0f 
    };

    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);

    glBindVertexArray(s_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void Triangulo::deleteMesh() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
}