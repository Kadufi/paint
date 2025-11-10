#pragma once

#include "forma.hpp"
#include <glad/glad.h>
#include <vector>

class Poligono : public Forma {
private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    std::vector<glm::vec3> m_verticesLocais;

    void atualizarVBO();

public:
    Poligono(const std::vector<glm::vec3>& pontos, const glm::vec3& cor);
    virtual ~Poligono();

    virtual void desenhar() const override;
    virtual bool foiSelecionado(const glm::vec2& pontoMouseMundo) override;
    virtual float calcularArea() const override;
    virtual float calcularPerimetro() const override;
    
    void setVertices(const std::vector<glm::vec3>& novosPontosMundo);
};