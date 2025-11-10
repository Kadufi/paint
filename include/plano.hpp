#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include "forma.hpp"
#include "shader.hpp"

class Plano {
private:
    glm::vec2 m_cameraPos;
    float m_zoomLevel;
    std::vector<std::unique_ptr<Forma>> m_objetos;
    
    int m_larguraJanela;
    int m_alturaJanela;
    
    std::vector<Forma*> m_objetosSelecionados;

public:
    Plano(int largura, int altura);
    ~Plano();

    void zoomIn(float fator = 1.1f);
    void zoomOut(float fator = 1.1f);
    void pan(float deltaX, float deltaY);

    glm::mat4 getMatrizView() const;
    glm::mat4 getMatrizProjection() const;
    
    glm::vec2 converterMouseParaMundo(double mouseX, double mouseY) const;
    float getZoomLevel() const;

    void adicionarForma(std::unique_ptr<Forma> forma);
    void desenhar(Shader& shader);
    void limpar();
    
    void onTamanhoJanelaMudou(int novaLargura, int novaAltura);
    
    void limparSelecao();
    void selecionarObjeto(const glm::vec2& posMundo, bool addSelecao);
    void selecionarArea(const glm::vec2& p1, const glm::vec2& p2);
    
    void escalarObjetosSelecionados(float fator);
    void transladarObjetosSelecionados(double deltaX_tela, double deltaY_tela);
    void rotacionarObjetosSelecionados(float anguloRad);
    void setCorObjetosSelecionados(const glm::vec3& cor);
    void removerObjetosSelecionados();
    
    const std::vector<Forma*>& getObjetosSelecionados() const;
};