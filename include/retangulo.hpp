#pragma once

#include "forma.hpp"
#include <glad/glad.h>

class Retangulo : public Forma {
private:
    static unsigned int s_VAO;
    static unsigned int s_VBO;

public:
    Retangulo(const glm::vec2& pos, float largura, float altura, const glm::vec3& cor);
    virtual ~Retangulo();

    static void setupMesh();
    static void deleteMesh();

    virtual void desenhar() const override;
    virtual bool foiSelecionado(const glm::vec2& pontoMouseMundo) override;
    virtual float calcularArea() const override;
    virtual float calcularPerimetro() const override;
};