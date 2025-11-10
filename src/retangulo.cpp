#include "retangulo.hpp"

unsigned int Retangulo::s_VAO = 0;
unsigned int Retangulo::s_VBO = 0;

Retangulo::Retangulo(const glm::vec2& pos, float largura, float altura, const glm::vec3& cor) {
    m_posicao = glm::vec3(pos.x, pos.y, 0.0f);
    m_escala = glm::vec2(largura, altura);
    m_cor = cor;
    m_rotacao = 0.0f;
    m_selecionado = false;
}

Retangulo::~Retangulo() {
}

void Retangulo::desenhar() const {
    glBindVertexArray(s_VAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);
}

bool Retangulo::foiSelecionado(const glm::vec2& pontoMouseMundo) {
    glm::mat4 invModel = glm::inverse(getMatrizModelo());
    glm::vec4 localPos = invModel * glm::vec4(pontoMouseMundo.x, pontoMouseMundo.y, 0.0, 1.0);

    if (localPos.x >= -0.5f && localPos.x <= 0.5f &&
        localPos.y >= -0.5f && localPos.y <= 0.5f) {
        return true;
    }
    return false;
}

float Retangulo::calcularArea() const {
    return m_escala.x * m_escala.y;
}

float Retangulo::calcularPerimetro() const {
    return 2.0f * (m_escala.x + m_escala.y);
}

void Retangulo::setupMesh() {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
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

void Retangulo::deleteMesh() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
}