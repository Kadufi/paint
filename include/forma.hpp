#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Forma {
protected:
    glm::vec3 m_posicao; 
    
    float m_rotacao;    
    
    glm::vec2 m_escala;
    
    glm::vec3 m_cor;
    
    bool m_selecionado;

public:

    Forma();
    
    virtual ~Forma();

    virtual void desenhar() const = 0; 

    virtual bool foiSelecionado(const glm::vec2& pontoMouseMundo) = 0;
    
    virtual float calcularArea() const = 0;
    
    virtual float calcularPerimetro() const = 0;

    void setCor(const glm::vec3& cor);
    glm::vec3 getCor() const;
    void setSelecionado(bool s);
    bool estaSelecionado() const;

    void transladar(const glm::vec2& delta);
    void rotacionar(float deltaAnguloRad);
    void escalar(const glm::vec2& fator);
    void setEscalaAbsoluta(const glm::vec2& escalaAbsoluta);

    glm::mat4 getMatrizModelo() const;
    glm::vec2 getPosicao() const;
};