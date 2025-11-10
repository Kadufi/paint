#pragma once

#include "forma.hpp"
#include <glad/glad.h>

class Circulo : public Forma {
private:
    static unsigned int s_VAO;
    static unsigned int s_VBO;
    static const int s_NUM_SEGMENTOS = 64;

public:
    Circulo(const glm::vec2& pos, float raio, const glm::vec3& cor);
    virtual ~Circulo();

    static void setupMesh();
    static void deleteMesh();

    virtual void desenhar() const override;
    virtual bool foiSelecionado(const glm::vec2& pontoMouseMundo) override;
    virtual float calcularArea() const override;
    virtual float calcularPerimetro() const override;
};