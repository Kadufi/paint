#include "retangulo.hpp"

/*
- Inicialização das variáveis estáticas (o "Carimbo")
- s_VAO e s_VBO são 'static'
*/
unsigned int Retangulo::s_VAO = 0;
unsigned int Retangulo::s_VBO = 0;

/*
- Construtor do Retangulo
- Recebe a posição do centro (pos)
- Recebe largura e altura, que são salvas como m_escala
- Define os atributos herdados
*/
Retangulo::Retangulo(const glm::vec2& pos, float largura, float altura, const glm::vec3& cor) {
    m_posicao = glm::vec3(pos.x, pos.y, 0.0f);
    m_escala = glm::vec2(largura, altura);
    m_cor = cor;
    m_rotacao = 0.0f;
    m_selecionado = false;
}

/*
- Destrutor
*/
Retangulo::~Retangulo() {
}

/*
- Função de desenho
- Binda o s_VAO da classe Retangulo
- Desenha com GL_LINE_LOOP
*/
void Retangulo::desenhar() const {
    glBindVertexArray(s_VAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);
}

/*
- Função de seleção
- Converte o clique do "Mundo" para o "Espaço Local" do objeto
- Como o "carimbo" (s_VBO) é um quadrado 1x1 centrado na origem
- (de -0.5 a 0.5), podemos apenas checar se o clique local está dentro desses limites.
*/
bool Retangulo::foiSelecionado(const glm::vec2& pontoMouseMundo) {
    glm::mat4 invModel = glm::inverse(getMatrizModelo());
    glm::vec4 localPos = invModel * glm::vec4(pontoMouseMundo.x, pontoMouseMundo.y, 0.0, 1.0);

    if (localPos.x >= -0.5f && localPos.x <= 0.5f &&
        localPos.y >= -0.5f && localPos.y <= 0.5f) {
        return true;
    }
    return false;
}

/*
- Área
*/
float Retangulo::calcularArea() const {
    return m_escala.x * m_escala.y;
}

/*
- Perímetro
*/
float Retangulo::calcularPerimetro() const {
    return 2.0f * (m_escala.x + m_escala.y);
}

/*
- Configura o "Carimbo" (s_VAO e s_VBO) da classe Retangulo
- Cria um quadrado 1x1 centrado na origem (de -0.5 a 0.5)
*/
void Retangulo::setupMesh() {
    float vertices[] = {
         0.5f,  0.5f, 0.0f, // Canto superior direito
         0.5f, -0.5f, 0.0f, // Canto inferior direito
        -0.5f, -0.5f, 0.0f, // Canto inferior esquerdo
        -0.5f,  0.5f, 0.0f  // Canto superior esquerdo
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
- Limpa o "Carimbo" (s_VAO e s_VBO) da classe Retangulo
*/
void Retangulo::deleteMesh() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
}