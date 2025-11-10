#include "circulo.hpp"
#include <vector> 
#include <cmath>

/*
- Define o valor de PI
*/
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
- Inicialização das variáveis estáticas
- s_VAO e s_VBO são 'static'
*/
unsigned int Circulo::s_VAO = 0;
unsigned int Circulo::s_VBO = 0;

/*
- Construtor do Circulo
- Recebe a posição do centro (pos)
- Recebe 'raio', que é salvo como uma escala uniforme (raio x raio)
- Define os atributos herdados
*/
Circulo::Circulo(const glm::vec2& pos, float raio, const glm::vec3& cor) {
    m_posicao = glm::vec3(pos.x, pos.y, 0.0f);
    m_escala = glm::vec2(raio, raio); // O raio é a escala
    m_cor = cor;
    m_rotacao = 0.0f;
    m_selecionado = false;
}

/*
- Destrutor
*/
Circulo::~Circulo() {
}

/*
- Função de desenho
- Binda o s_VAO
- Desenha o contorno conectando todos os segmentos com GL_LINE_LOOP
*/
void Circulo::desenhar() const {
    glBindVertexArray(s_VAO);
    // s_NUM_SEGMENTOS é uma const estática definida em Circulo.hpp
    glDrawArrays(GL_LINE_LOOP, 0, s_NUM_SEGMENTOS);
    glBindVertexArray(0);
}

/*
- Função de seleção
- Converte o clique do "Mundo" para o "Espaço Local" do objeto
- checar a distância do clique local (localPos) até a origem (0,0).
*/
bool Circulo::foiSelecionado(const glm::vec2& pontoMouseMundo) {
    glm::mat4 invModel = glm::inverse(getMatrizModelo());
    glm::vec4 localPos = invModel * glm::vec4(pontoMouseMundo.x, pontoMouseMundo.y, 0.0, 1.0);

    // distância ao quadrado (mais rápida, evita raiz quadrada)
    float distQuadrada = (localPos.x * localPos.x) + (localPos.y * localPos.y);

    // Se a distância ao quadrado for <= 1.0
    // o clique está dentro do círculo unitário.
    if (distQuadrada <= 1.0f) {
        return true;
    }
    return false;
}

/*
- Funções de consulta
- Área = PI * raio^2
- O 'raio' é armazenado em m_escala.x (ou m_escala.y)
*/
float Circulo::calcularArea() const {
    float raio = m_escala.x;
    return M_PI * raio * raio;
}

/*
- Perímetro
*/
float Circulo::calcularPerimetro() const {
    float raio = m_escala.x;
    return 2.0f * M_PI * raio;
}

/*
- Configura o "Carimbo" (s_VAO e s_VBO) da classe Circulo
*/
void Circulo::setupMesh() {
    std::vector<float> vertices;
    // Calcula o incremento do ângulo (um círculo completo 2*PI dividido pelo n° de segmentos)
    float anguloInc = 2.0f * M_PI / (float)s_NUM_SEGMENTOS;

    // Gera os vértices na borda do círculo unitário
    for (int i = 0; i < s_NUM_SEGMENTOS; ++i) {
        float angulo = i * anguloInc;
        // Posição X = cos(angulo)
        vertices.push_back(cos(angulo));
        // Posição Y = sin(angulo)
        vertices.push_back(sin(angulo));
        // Posição Z = 0
        vertices.push_back(0.0f);
    }

    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);

    glBindVertexArray(s_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    // Envia os vertices (em std::vector) para o VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Configura o VAO para ler os vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/*
- Limpa o "Carimbo" (s_VAO e s_VBO) da classe Circulo
*/
void Circulo::deleteMesh() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
}