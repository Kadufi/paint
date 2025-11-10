#include "colorpicker.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <GLFW/glfw3.h> 

ColorPicker::ColorPicker() : 
    m_quadVAO(0), m_quadVBO(0), m_isVisible(false), 
    m_corResultado(1.0f), m_pinoR(0.0f), m_pinoG(0.0f), m_pinoB(0.0f),
    m_dragState(NONE), m_corFoiAtualizada(false)
{
}

ColorPicker::~ColorPicker() {
}

void ColorPicker::setupMesh() {
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f, 
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    unsigned int EBO;
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void ColorPicker::deleteMesh() {
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadVBO);
}

void ColorPicker::calcularGeometriaUI(int screenWidth, int screenHeight) {
    float uiLargura = 200.0f;
    float uiAltura = 100.0f;
    float padding = 10.0f;
    float barraAltura = 20.0f;

    float x0 = screenWidth - uiLargura - padding;
    float y0 = screenHeight - uiAltura - padding;

    m_barraR.x1 = x0; m_barraR.y1 = y0 + barraAltura * 2; m_barraR.x2 = x0 + uiLargura; m_barraR.y2 = y0 + barraAltura * 3;
    m_barraG.x1 = x0; m_barraG.y1 = y0 + barraAltura * 1; m_barraG.x2 = x0 + uiLargura; m_barraG.y2 = y0 + barraAltura * 2;
    m_barraB.x1 = x0; m_barraB.y1 = y0;                  m_barraB.x2 = x0 + uiLargura; m_barraB.y2 = y0 + barraAltura * 1;
    m_boxResultado.x1 = x0; m_boxResultado.y1 = y0 + barraAltura * 3 + 5; m_boxResultado.x2 = x0 + uiLargura; m_boxResultado.y2 = y0 + barraAltura * 4 + 5;
}

void ColorPicker::desenhar(Shader& uiShader, int screenWidth, int screenHeight) {
    if (!m_isVisible) return;

    calcularGeometriaUI(screenWidth, screenHeight);
    
    uiShader.use();
    glBindVertexArray(m_quadVAO);

    glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
    uiShader.setMat4("u_projection", projection);

    auto drawQuad = [&](const AABB& box, int mode, const glm::vec3& color) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(box.x1, box.y1, 0.0f));
        model = glm::scale(model, glm::vec3(box.x2 - box.x1, box.y2 - box.y1, 1.0f));
        uiShader.setMat4("u_model", model);
        uiShader.setInt("u_Mode", mode);
        uiShader.setVec3("u_Color", color);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

    drawQuad(m_barraR, 1, glm::vec3(0));
    drawQuad(m_barraG, 2, glm::vec3(0));
    drawQuad(m_barraB, 3, glm::vec3(0));
    drawQuad(m_boxResultado, 0, m_corResultado);

    float pinoLargura = 4.0f;
    float barraAlturaConstante = 20.0f; 
    
    float pinoR_x = m_barraR.x1 + (m_barraR.x2 - m_barraR.x1) * m_pinoR - (pinoLargura / 2.0f);
    float pinoG_x = m_barraG.x1 + (m_barraG.x2 - m_barraG.x1) * m_pinoG - (pinoLargura / 2.0f);
    float pinoB_x = m_barraB.x1 + (m_barraB.x2 - m_barraB.x1) * m_pinoB - (pinoLargura / 2.0f);

    drawQuad({pinoR_x, m_barraR.y1 - 2.0f, pinoR_x + pinoLargura, m_barraR.y2 + 2.0f}, 0, glm::vec3(1.0f));
    drawQuad({pinoG_x, m_barraG.y1 - 2.0f, pinoG_x + pinoLargura, m_barraG.y2 + 2.0f}, 0, glm::vec3(1.0f));
    drawQuad({pinoB_x, m_barraB.y1 - 2.0f, pinoB_x + pinoLargura, m_barraB.y2 + 2.0f}, 0, glm::vec3(1.0f));
    
    glBindVertexArray(0);
}

float ColorPicker::getPorcentagemDaPos(float xpos, const AABB& barra) {
    float percent = (xpos - barra.x1) / (barra.x2 - barra.x1);
    return std::max(0.0f, std::min(1.0f, percent));
}

bool ColorPicker::onMouseClick(double xpos, double ypos_inv, int action) {
    if (!m_isVisible) return false;

    int screenHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), nullptr, &screenHeight);
    float ypos = screenHeight - ypos_inv;

    auto isInside = [&](const AABB& box) {
        return xpos >= box.x1 && xpos <= box.x2 && ypos >= box.y1 && ypos <= box.y2;
    };

    if (action == GLFW_PRESS) {
        if (isInside(m_barraR))      { m_dragState = DRAG_R; onMouseMove(xpos, ypos_inv); return true; }
        else if (isInside(m_barraG)) { m_dragState = DRAG_G; onMouseMove(xpos, ypos_inv); return true; }
        else if (isInside(m_barraB)) { m_dragState = DRAG_B; onMouseMove(xpos, ypos_inv); return true; }
        else if (isInside(m_boxResultado)) return true;
    }

    if (action == GLFW_RELEASE) {
        m_dragState = NONE;
    }
    
    return isInside(m_barraR) || isInside(m_barraG) || isInside(m_barraB) || isInside(m_boxResultado);
}

void ColorPicker::onMouseMove(double xpos, double ypos_inv) {
    if (m_dragState == NONE) return;

    int screenHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), nullptr, &screenHeight);
    float ypos = screenHeight - ypos_inv;

    if (m_dragState == DRAG_R) {
        m_pinoR = getPorcentagemDaPos(xpos, m_barraR);
        m_corResultado.r = m_pinoR;
        m_corFoiAtualizada = true;
    } else if (m_dragState == DRAG_G) {
        m_pinoG = getPorcentagemDaPos(xpos, m_barraG);
        m_corResultado.g = m_pinoG;
        m_corFoiAtualizada = true;
    } else if (m_dragState == DRAG_B) {
        m_pinoB = getPorcentagemDaPos(xpos, m_barraB);
        m_corResultado.b = m_pinoB;
        m_corFoiAtualizada = true;
    }
}

void ColorPicker::setCor(const glm::vec3& cor) {
    m_corResultado = cor;
    m_pinoR = cor.r;
    m_pinoG = cor.g;
    m_pinoB = cor.b;
}

glm::vec3 ColorPicker::getCor() const {
    return m_corResultado;
}

void ColorPicker::setVisible(bool visible) {
    m_isVisible = visible;
}

bool ColorPicker::isVisible() const {
    return m_isVisible;
}

bool ColorPicker::foiAtualizada() {
    if (m_corFoiAtualizada) {
        m_corFoiAtualizada = false;
        return true;
    }
    return false;
}