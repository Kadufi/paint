#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.hpp"

class ColorPicker {
private:
    unsigned int m_quadVAO;
    unsigned int m_quadVBO;

    bool m_isVisible;
    glm::vec3 m_corResultado;
    
    float m_pinoR;
    float m_pinoG;
    float m_pinoB;

    enum DragState { NONE, DRAG_R, DRAG_G, DRAG_B };
    DragState m_dragState;

    bool m_corFoiAtualizada;

    struct AABB {
        float x1, y1, x2, y2;
    };
    AABB m_barraR, m_barraG, m_barraB, m_boxResultado;
    
    void calcularGeometriaUI(int screenWidth, int screenHeight);
    float getPorcentagemDaPos(float xpos, const AABB& barra);

public:
    ColorPicker();
    ~ColorPicker();

    void setupMesh();
    void deleteMesh();
    void desenhar(Shader& uiShader, int screenWidth, int screenHeight);
    
    bool onMouseClick(double xpos, double ypos, int action);
    void onMouseMove(double xpos, double ypos);
    
    void setCor(const glm::vec3& cor);
    glm::vec3 getCor() const;
    
    void setVisible(bool visible);
    bool isVisible() const;

    bool foiAtualizada();
};