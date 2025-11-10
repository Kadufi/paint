#include "poligono.hpp"
#include <iostream>

/*
- Construtor
- Define atributos base (escala 1x1, rotação 0)
- Cria o VAO e VBO *próprios* deste objeto (não são estáticos)
- Chama setVertices para calcular a geometria e preencher os buffers
*/
Poligono::Poligono(const std::vector<glm::vec3>& pontosMundo, const glm::vec3& cor) {
    m_escala = glm::vec2(1.0f);
    m_rotacao = 0.0f;
    m_cor = cor;
    m_selecionado = false;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    setVertices(pontosMundo);
}

/*
- Destrutor do Poligono
- Limpa o VAO e VBO únicos que foram criados no construtor
*/
Poligono::~Poligono() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

/*
- Função interna para enviar/atualizar os vértices na GPU
- Envia a lista m_verticesLocais para o m_VBO deste objeto
- Configura o m_VAO para ler o VBO
*/
void Poligono::atualizarVBO() {
    if (m_verticesLocais.empty()) return;
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_verticesLocais.size() * sizeof(glm::vec3), m_verticesLocais.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

/*
- Define/atualiza a geometria do polígono
- Recebe os pontos clicados (em "Coordenadas de Mundo")
- Calcula o centro geométrico e o salva como m_posicao
- Armazena os vértices em "Espaço Local" (m_verticesLocais) subtraindo o centro
- Chama atualizarVBO() para enviar essa geometria local para a GPU
*/
void Poligono::setVertices(const std::vector<glm::vec3>& novosPontosMundo) {
    if (novosPontosMundo.empty()) return;

    glm::vec3 centro(0.0f);
    for(const auto& v : novosPontosMundo) {
        centro += v;
    }
    m_posicao = centro / (float)novosPontosMundo.size();
    
    m_verticesLocais.clear();
    for(const auto& v : novosPontosMundo) {
        m_verticesLocais.push_back(v - m_posicao);
    }
    
    atualizarVBO();
}

/*
- Função de desenho
- Binda o m_VAO
- Desenha com GL_LINE_LOOP
*/
void Poligono::desenhar() const {
    if (m_verticesLocais.empty()) return;

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINE_LOOP, 0, m_verticesLocais.size());
    glBindVertexArray(0);
}

/*
- Função de seleção
- Implementa o Algoritmo do "Tiro" (Ray Casting)
- Conta o número de interseções (par ou ímpar) para ver se o ponto está dentro
*/
bool Poligono::foiSelecionado(const glm::vec2& pontoMouseMundo) {
    
    glm::mat4 invModel = glm::inverse(getMatrizModelo());
    glm::vec4 localPosTemp = invModel * glm::vec4(pontoMouseMundo.x, pontoMouseMundo.y, 0.0, 1.0);
    glm::vec2 localPos = glm::vec2(localPosTemp);

    int n = m_verticesLocais.size();
    if (n < 3) return false;

    bool dentro = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        glm::vec2 pi = glm::vec2(m_verticesLocais[i]);
        glm::vec2 pj = glm::vec2(m_verticesLocais[j]);

        bool cond1 = (pi.y > localPos.y) != (pj.y > localPos.y);
        bool cond2 = (localPos.x < (pj.x - pi.x) * (localPos.y - pi.y) / (pj.y - pi.y) + pi.x);
        
        if (cond1 && cond2) {
            dentro = !dentro;
        }
    }
    
    return dentro;
}

/*
- Funções de consulta (placeholders)
- Ainda não implementadas
*/
float Poligono::calcularArea() const {
    return 0.0f;
}

float Poligono::calcularPerimetro() const {
    return 0.0f;
}