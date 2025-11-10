#include "forma.hpp"

Forma::Forma() : 
    m_posicao(0.0f, 0.0f, 0.0f),
    m_rotacao(0.0f),            
    m_escala(1.0f, 1.0f),       
    m_cor(1.0f, 1.0f, 0.0f),    
    m_selecionado(false)  
          
{
    
}

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

void Forma::transladar(const glm::vec2& delta) {
    m_posicao.x += delta.x;
    m_posicao.y += delta.y;
}

void Forma::rotacionar(float deltaAnguloRad) {
    m_rotacao += deltaAnguloRad;
}

void Forma::escalar(const glm::vec2& fator) {
    m_escala.x *= fator.x;
    m_escala.y *= fator.y;
}

void Forma::setEscalaAbsoluta(const glm::vec2& escalaAbsoluta) {
    m_escala = escalaAbsoluta;
}

glm::mat4 Forma::getMatrizModelo() const {

    glm::mat4 modelo = glm::mat4(1.0f);

    modelo = glm::translate(modelo, m_posicao);

    modelo = glm::rotate(modelo, m_rotacao, glm::vec3(0.0f, 0.0f, 1.0f));

    modelo = glm::scale(modelo, glm::vec3(m_escala.x, m_escala.y, 1.0f));
    
    return modelo;
}

glm::vec2 Forma::getPosicao() const {
    return glm::vec2(m_posicao.x, m_posicao.y);
}