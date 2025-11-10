#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "plano.hpp"
#include "shader.hpp"
#include "triangulo.hpp"
#include "retangulo.hpp"
#include "circulo.hpp"
#include "colorpicker.hpp"
#include "poligono.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Plano g_Plano(SCR_WIDTH, SCR_HEIGHT);
ColorPicker g_ColorPicker;

bool g_IsPanning = false;
bool g_IsDragging = false;
bool g_IsSelectingMarquee = false;
bool g_isRotationMode = false;
glm::dvec2 g_LastMousePos;
glm::vec2 g_MarqueeStartPosMundo;

enum class CreationState { NONE, CREATING_POLYGON };
CreationState g_creationState = CreationState::NONE;
std::vector<glm::vec3> g_tempVertices;
unsigned int g_previewVAO = 0;
unsigned int g_previewVBO = 0;
unsigned int g_marqueeVAO = 0;
unsigned int g_marqueeVBO = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Meu Paint 2D", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }
    glDisable(GL_DEPTH_TEST);

    Shader shader2D("../shader/simples.vert", "../shader/simples.frag");
    Shader uiShader("../shader/ui.vert", "../shader/ui.frag");

    Triangulo::setupMesh();
    Retangulo::setupMesh();
    Circulo::setupMesh();
    g_ColorPicker.setupMesh();

    glGenVertexArrays(1, &g_previewVAO);
    glGenBuffers(1, &g_previewVBO);
    glBindVertexArray(g_previewVAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_previewVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glGenVertexArrays(1, &g_marqueeVAO);
    glGenBuffers(1, &g_marqueeVBO);
    glBindVertexArray(g_marqueeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_marqueeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader2D.use();
        glm::mat4 projection = g_Plano.getMatrizProjection();
        glm::mat4 view = g_Plano.getMatrizView();
        shader2D.setMat4("u_projection", projection);
        shader2D.setMat4("u_view", view);   
        g_Plano.desenhar(shader2D); 

        if (g_creationState == CreationState::CREATING_POLYGON && !g_tempVertices.empty()) {
            shader2D.use();
            shader2D.setMat4("u_projection", projection);
            shader2D.setMat4("u_view", view);   
            shader2D.setMat4("u_model", glm::mat4(1.0f));
            shader2D.setVec3("u_Color", glm::vec3(1.0f, 1.0f, 1.0f));

            glBindVertexArray(g_previewVAO);
            glBindBuffer(GL_ARRAY_BUFFER, g_previewVBO);
            glBufferData(GL_ARRAY_BUFFER, g_tempVertices.size() * sizeof(glm::vec3), g_tempVertices.data(), GL_DYNAMIC_DRAW);
            
            glDrawArrays(GL_LINE_STRIP, 0, g_tempVertices.size());
            glBindVertexArray(0);
        }
        
        if (g_IsSelectingMarquee) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            glm::vec2 marqueeEndPosMundo = g_Plano.converterMouseParaMundo(xpos, ypos);

            float vertices[] = {
                g_MarqueeStartPosMundo.x, g_MarqueeStartPosMundo.y, 0.0f,
                marqueeEndPosMundo.x,   g_MarqueeStartPosMundo.y, 0.0f,
                marqueeEndPosMundo.x,   marqueeEndPosMundo.y,   0.0f,
                g_MarqueeStartPosMundo.x, marqueeEndPosMundo.y,   0.0f
            };

            shader2D.use();
            shader2D.setMat4("u_projection", projection);
            shader2D.setMat4("u_view", view);
            shader2D.setMat4("u_model", glm::mat4(1.0f));
            shader2D.setVec3("u_Color", glm::vec3(1.0f, 1.0f, 1.0f));

            glBindVertexArray(g_marqueeVAO);
            glBindBuffer(GL_ARRAY_BUFFER, g_marqueeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
            
            glDrawArrays(GL_LINE_LOOP, 0, 4);
            glBindVertexArray(0);
        }

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        g_ColorPicker.desenhar(uiShader, width, height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &g_marqueeVAO);
    glDeleteBuffers(1, &g_marqueeVBO);
    glDeleteVertexArrays(1, &g_previewVAO);
    glDeleteBuffers(1, &g_previewVBO);
    g_Plano.limpar();
    g_ColorPicker.deleteMesh();
    Circulo::deleteMesh();
    Retangulo::deleteMesh();
    Triangulo::deleteMesh();
    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (g_creationState == CreationState::CREATING_POLYGON) {
            g_creationState = CreationState::NONE;
            g_tempVertices.clear();
        } else if (g_isRotationMode) {
            g_isRotationMode = false;
        } else if (!g_Plano.getObjetosSelecionados().empty()){
            g_Plano.limparSelecao();
            g_ColorPicker.setVisible(false); 
        } else {
            glfwSetWindowShouldClose(window, true);
        }
        return;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (g_creationState == CreationState::NONE) {
            g_creationState = CreationState::CREATING_POLYGON;
            g_tempVertices.clear();
            g_Plano.limparSelecao();
            g_ColorPicker.setVisible(false);
        } else if (g_creationState == CreationState::CREATING_POLYGON) {
            g_creationState = CreationState::NONE;
            if (g_tempVertices.size() >= 3) {
                auto novoPoligono = std::make_unique<Poligono>(g_tempVertices, glm::vec3(1.0, 1.0, 1.0));
                g_Plano.adicionarForma(std::move(novoPoligono));
            }
            g_tempVertices.clear();
        }
        return;
    }
    
    if (g_creationState != CreationState::NONE) return;

    if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        g_Plano.zoomIn();
    }
    
    if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        g_Plano.zoomOut();
    } 
   
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        double xpos_pixels, ypos_pixels;
        glfwGetCursorPos(window, &xpos_pixels, &ypos_pixels);
        glm::vec2 posMundo = g_Plano.converterMouseParaMundo(xpos_pixels, ypos_pixels);
        auto novoTriangulo = std::make_unique<Triangulo>(posMundo, 50.0f, glm::vec3(1.0, 1.0, 1.0));
        g_Plano.adicionarForma(std::move(novoTriangulo));
    }
    
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        double xpos_pixels, ypos_pixels;
        glfwGetCursorPos(window, &xpos_pixels, &ypos_pixels);
        glm::vec2 posMundo = g_Plano.converterMouseParaMundo(xpos_pixels, ypos_pixels);
        auto novoRetangulo = std::make_unique<Retangulo>(posMundo, 50.0f, 50.0f, glm::vec3(1.0, 1.0, 1.0));
        g_Plano.adicionarForma(std::move(novoRetangulo));
    }
    
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        double xpos_pixels, ypos_pixels;
        glfwGetCursorPos(window, &xpos_pixels, &ypos_pixels);
        glm::vec2 posMundo = g_Plano.converterMouseParaMundo(xpos_pixels, ypos_pixels);
        auto novoCirculo = std::make_unique<Circulo>(posMundo, 25.0f, glm::vec3(1.0, 1.0, 1.0));
        g_Plano.adicionarForma(std::move(novoCirculo));
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        if (g_isRotationMode) {
            g_isRotationMode = false;
        } else {
            if (!g_Plano.getObjetosSelecionados().empty()) {
                g_isRotationMode = true;
                g_creationState = CreationState::NONE;
            }
        }
        return;
    }

    if (g_isRotationMode) {
        float angulo = glm::radians(5.0f);
        if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            g_Plano.rotacionarObjetosSelecionados(angulo);
        }
        if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            g_Plano.rotacionarObjetosSelecionados(-angulo);
        }
        return;
    }
    
    float scale_factor = 1.1f;
    if (key == GLFW_KEY_EQUAL && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        g_Plano.escalarObjetosSelecionados(scale_factor);
    }
    
    if (key == GLFW_KEY_MINUS && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        g_Plano.escalarObjetosSelecionados(1.0f / scale_factor);
    }

    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
        g_Plano.removerObjetosSelecionados();
        g_ColorPicker.setVisible(false);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if (g_creationState == CreationState::CREATING_POLYGON) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            glm::vec2 posMundo = g_Plano.converterMouseParaMundo(xpos, ypos);
            g_tempVertices.push_back(glm::vec3(posMundo, 0.0f));
        }
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if (g_ColorPicker.isVisible() && g_ColorPicker.onMouseClick(xpos, ypos, action)) {
                return;
            }
            
            g_LastMousePos = glm::dvec2(xpos, ypos);
            glm::vec2 posMundo = g_Plano.converterMouseParaMundo(xpos, ypos);
            bool addSelecao = (mods & GLFW_MOD_SHIFT);
            
            bool clicouEmSelecionado = false;
            for (auto* forma : g_Plano.getObjetosSelecionados()) {
                if (forma->foiSelecionado(posMundo)) {
                    clicouEmSelecionado = true;
                    break;
                }
            }

            if (clicouEmSelecionado && !addSelecao) {
                g_IsDragging = true;
            } else {
                g_Plano.selecionarObjeto(posMundo, addSelecao);
                
                if (!g_Plano.getObjetosSelecionados().empty()) {
                    g_IsDragging = true;
                    if (g_Plano.getObjetosSelecionados().size() == 1) {
                        g_ColorPicker.setCor(g_Plano.getObjetosSelecionados()[0]->getCor());
                    }
                    g_ColorPicker.setVisible(true);
                } else {
                    g_IsDragging = false;
                    g_IsSelectingMarquee = true;
                    g_MarqueeStartPosMundo = posMundo;
                    g_ColorPicker.setVisible(false);
                }
            }

        } else if (action == GLFW_RELEASE) {
            g_ColorPicker.onMouseClick(xpos, ypos, action);
            g_IsDragging = false;

            if (g_IsSelectingMarquee) {
                g_IsSelectingMarquee = false;
                glm::vec2 posMundo = g_Plano.converterMouseParaMundo(xpos, ypos);
                g_Plano.selecionarArea(g_MarqueeStartPosMundo, posMundo);
                if (!g_Plano.getObjetosSelecionados().empty()) {
                    g_ColorPicker.setVisible(true);
                }
            }
        }
    }
    
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            if (g_ColorPicker.isVisible() && g_ColorPicker.onMouseClick(xpos, ypos, action)) {
                return; 
            }
            g_IsPanning = true;
            glfwGetCursorPos(window, &g_LastMousePos.x, &g_LastMousePos.y);
        } else if (action == GLFW_RELEASE) {
            g_IsPanning = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    g_ColorPicker.onMouseMove(xpos, ypos);
    if(g_ColorPicker.foiAtualizada()) {
        g_Plano.setCorObjetosSelecionados(g_ColorPicker.getCor());
    }

    if (g_creationState == CreationState::CREATING_POLYGON) {
        return;
    }

    if (!g_IsPanning && !g_IsDragging && !g_IsSelectingMarquee) {
        return;
    } 

    double deltaX = xpos - g_LastMousePos.x;
    double deltaY = ypos - g_LastMousePos.y;

    if (g_IsPanning) {
        g_Plano.pan(deltaX, deltaY);
    } else if (g_IsDragging) {
        g_Plano.transladarObjetosSelecionados(deltaX, deltaY);
    }

    g_LastMousePos = glm::dvec2(xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (yoffset > 0) {
        g_Plano.zoomIn();
    } else if (yoffset < 0) {
        g_Plano.zoomOut();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    g_Plano.onTamanhoJanelaMudou(width, height);
}