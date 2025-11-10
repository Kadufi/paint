#include "triangulo.hpp"
#include <iostream>

/*
- Inicialização das variáveis estáticas (o "Carimbo")
- s_VAO e s_VBO são 'static
*/
unsigned int Triangulo::s_VAO = 0;
unsigned int Triangulo::s_VBO = 0;

/*
- Construtor do Triangulo
- Recebe a posição do centro (pos)
- Recebe 'lado', que é usado para uma escala uniforme (lado x lado)
- Define os atributos herdados
*/
Triangulo::Triangulo(const glm::vec2& pos, float lado, const glm::vec3& cor) {
    m_posicao = glm::vec3(pos.x, pos.y, 0.0f);
    m_escala = glm::vec2(lado, lado);
    m_cor = cor;
    m_rotacao = 0.0f;
    m_selecionado = false;
}

/*
- Destrutor
*/
Triangulo::~Triangulo() {

}

/*
- Função de desenho
- Binda o s_VAO
- Desenha com GL_LINE_LOOP
*/
void Triangulo::desenhar() const {
    glBindVertexArray(s_VAO);
    
    glDrawArrays(GL_LINE_LOOP, 0, 3);
    
    glBindVertexArray(0);
}

/*
- Função de seleção
- Converte o clique do "Mundo" para o "Espaço Local" do objeto
- Faz um teste simples de "Bounding Box" (caixa)
- Como o "carimbo" equilátero (de -0.433 a 0.433) cabe dentro
- de uma caixa de -0.5 a 0.5, esse teste de seleção é uma
*/
bool Triangulo::foiSelecionado(const glm::vec2& pontoMouseMundo) {

    glm::mat4 invModel = glm::inverse(getMatrizModelo());
    glm::vec4 localPos = invModel * glm::vec4(pontoMouseMundo.x, pontoMouseMundo.y, 0.0, 1.0);

    if (localPos.x >= -0.5 && localPos.x <= 0.5 && localPos.y >= -0.5 && localPos.y <= 0.5) {
        return true;
    }
    return false;
}

/*
- Área
*/
float Triangulo::calcularArea() const {
    // A base do carimbo é ~0.866, altura é 0.75. Área ~ 0.32
    // A fórmula correta seria: (0.32f * m_escala.x * m_escala.y)
    // 0.5f é uma aproximação da área do carimbo isósceles anterior
    return 0.5f * m_escala.x * m_escala.y;
}

/*
- Perímetro (placeholder)
*/
float Triangulo::calcularPerimetro() const {
    return 0.0f;
}

/*
- Configura o "Carimbo" (s_VAO e s_VBO) da classe Triangulo
- Cria um triângulo equilátero "unitário" centrado
*/
void Triangulo::setupMesh() {

    float vertices[] = {
         0.0f,    0.5f, 0.0f,  // Topo
        -0.433f, -0.25f, 0.0f, // Base esquerda
         0.433f, -0.25f, 0.0f  // Base direita
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

/*
- Limpa o "Carimbo" (s_VAO e s_VBO) da classe Triangulo
*/
void Triangulo::deleteMesh() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
}