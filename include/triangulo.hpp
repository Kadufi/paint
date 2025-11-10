#pragma once

#include "forma.hpp"
#include <glad/glad.h>

class Triangulo : public Forma {
private:
    static unsigned int s_VAO;
    static unsigned int s_VBO;

public:
    
    Triangulo(const glm::vec2& pos, float lado, const glm::vec3& cor);
    
    virtual ~Triangulo();

    static void setupMesh();
    
    static void deleteMesh();

    virtual void desenhar() const override; 
    
    virtual bool foiSelecionado(const glm::vec2& pontoMouseMundo) override;
    
    virtual float calcularArea() const override;
    
    virtual float calcularPerimetro() const override;
};