#include "forma.hpp"

/*
- Construtor da classe base
*/
Forma::Forma() : 
    m_posicao(0.0f, 0.0f, 0.0f), // Posição inicial (0,0,0)
    m_rotacao(0.0f),              // Rotação inicial (0 graus)
    m_escala(1.0f, 1.0f),         // Escala inicial (100%)
    m_cor(1.0f, 1.0f, 1.0f),      // Cor inicial 
    m_selecionado(false)          // Não selecionado
{
    
}

/*
- Destrutor
*/
Forma::~Forma() {
}

void Forma::setCor(const glm::vec3& cor) {
    m_cor = cor;
}

glm::vec3 Forma::getCor() const {
    return m_cor;
}

void Forma::setSelecionado(bool s) {
    m_selecionado = s;
}

bool Forma::estaSelecionado() const {
    return m_selecionado;
}

/*
- Adiciona um delta à posição atual.
*/
void Forma::transladar(const glm::vec2& delta) {
    m_posicao.x += delta.x;
    m_posicao.y += delta.y;
}

/*
- Adiciona um ângulo (em radianos) à rotação atual.
*/
void Forma::rotacionar(float deltaAnguloRad) {
    m_rotacao += deltaAnguloRad;
}

/*
- Multiplica a escala atual por um 'fator'.
*/
void Forma::escalar(const glm::vec2& fator) {
    m_escala.x *= fator.x;
    m_escala.y *= fator.y;
}

/*
- Define um valor absoluto para a escala (não usado no momento), seria pra resetar escala
*/
void Forma::setEscalaAbsoluta(const glm::vec2& escalaAbsoluta) {
    m_escala = escalaAbsoluta;
}


/*
- Lê os atributos m_posicao, m_rotacao e m_escala
- e os combina em uma única Matriz Model.
- É esta matriz que é enviada ao shader (uniform u_model) e diz
- à GPU como mover/girar/escalar o "carimbo" (VAO) genérico.
*/
glm::mat4 Forma::getMatrizModelo() const {

    // 1. Começa com uma matriz "neutra" (identidade)
    glm::mat4 modelo = glm::mat4(1.0f);

    // 2. Aplica a translação (Move o objeto)
    modelo = glm::translate(modelo, m_posicao);

    // 3. Aplica a rotação (Gira em torno do eixo Z)
    modelo = glm::rotate(modelo, m_rotacao, glm::vec3(0.0f, 0.0f, 1.0f));

    // 4. Aplica a escala (Estica/encolhe o objeto)
    modelo = glm::scale(modelo, glm::vec3(m_escala.x, m_escala.y, 1.0f));
    
    // 5. Retorna a matriz final combinada
    return modelo;
}

/*
- Get simples para a posição.
*/
glm::vec2 Forma::getPosicao() const {
    return glm::vec2(m_posicao.x, m_posicao.y);
}