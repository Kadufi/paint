#include "circulo.hpp"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

unsigned int Circulo::s_VAO = 0;
unsigned int Circulo::s_VBO = 0;

Circulo::Circulo(const glm::vec2& pos, float raio, const glm::vec3& cor) {
    m_posicao = glm::vec3(pos.x, pos.y, 0.0f);
    m_escala = glm::vec2(raio, raio);
    m_cor = cor;
    m_rotacao = 0.0f;
    m_selecionado = false;
}

Circulo::~Circulo() {
}

void Circulo::desenhar() const {
    glBindVertexArray(s_VAO);
    glDrawArrays(GL_LINE_LOOP, 0, s_NUM_SEGMENTOS);
    glBindVertexArray(0);
}

bool Circulo::foiSelecionado(const glm::vec2& pontoMouseMundo) {
    glm::mat4 invModel = glm::inverse(getMatrizModelo());
    glm::vec4 localPos = invModel * glm::vec4(pontoMouseMundo.x, pontoMouseMundo.y, 0.0, 1.0);

    float distQuadrada = (localPos.x * localPos.x) + (localPos.y * localPos.y);

    if (distQuadrada <= 1.0f) {
        return true;
    }
    return false;
}

float Circulo::calcularArea() const {
    float raio = m_escala.x;
    return M_PI * raio * raio;
}

float Circulo::calcularPerimetro() const {
    float raio = m_escala.x;
    return 2.0f * M_PI * raio;
}

void Circulo::setupMesh() {
    std::vector<float> vertices;
    float anguloInc = 2.0f * M_PI / (float)s_NUM_SEGMENTOS;

    for (int i = 0; i < s_NUM_SEGMENTOS; ++i) {
        float angulo = i * anguloInc;
        vertices.push_back(cos(angulo));
        vertices.push_back(sin(angulo));
        vertices.push_back(0.0f);
    }

    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);

    glBindVertexArray(s_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Circulo::deleteMesh() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
}