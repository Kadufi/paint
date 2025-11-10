#include "plano.hpp" 
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <algorithm>
#include <set>

Plano::Plano(int largura, int altura) : 
    m_cameraPos(0.0f),
    m_zoomLevel(1.0f),
    m_larguraJanela(largura),
    m_alturaJanela(altura)
{
}

Plano::~Plano() {
}

void Plano::zoomIn(float fator) {
    m_zoomLevel *= fator;
}

void Plano::zoomOut(float fator) {
    m_zoomLevel /= fator;
}

void Plano::pan(float deltaX_tela, float deltaY_tela) {
    m_cameraPos.x -= deltaX_tela / m_zoomLevel;
    m_cameraPos.y += deltaY_tela / m_zoomLevel; 
}

glm::mat4 Plano::getMatrizView() const {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-m_cameraPos.x, -m_cameraPos.y, 0.0f));
    return view;
}

glm::mat4 Plano::getMatrizProjection() const {
    float viewWidth = (float)m_larguraJanela / m_zoomLevel;
    float viewHeight = (float)m_alturaJanela / m_zoomLevel;

    return glm::ortho(
        -viewWidth / 2.0f,
         viewWidth / 2.0f,
        -viewHeight / 2.0f,
         viewHeight / 2.0f,
        -1.0f,
         1.0f
    );
}

glm::vec2 Plano::converterMouseParaMundo(double mouseX, double mouseY) const {
    float ndcX = (2.0f * (float)mouseX) / (float)m_larguraJanela - 1.0f;
    float ndcY = 1.0f - (2.0f * (float)mouseY) / (float)m_alturaJanela;

    glm::mat4 proj = getMatrizProjection();
    glm::mat4 view = getMatrizView();

    glm::mat4 invVP = glm::inverse(proj * view);

    glm::vec4 worldPos = invVP * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

    return glm::vec2(worldPos.x, worldPos.y);
}

float Plano::getZoomLevel() const {
    return m_zoomLevel;
}

void Plano::adicionarForma(std::unique_ptr<Forma> forma) {
    m_objetos.push_back(std::move(forma));
}

void Plano::desenhar(Shader& shader) {

    for (const auto& forma : m_objetos) {
        
        glm::mat4 model = forma->getMatrizModelo();
        glm::vec3 cor = forma->getCor();

        shader.setMat4("u_model", model);
        shader.setVec3("u_Color", cor);

        forma->desenhar();
    }
}

void Plano::limpar() {
    m_objetos.clear();
    m_objetosSelecionados.clear();
}

void Plano::onTamanhoJanelaMudou(int novaLargura, int novaAltura) {
    m_larguraJanela = novaLargura;
    m_alturaJanela = novaAltura;
}

void Plano::limparSelecao() {
    m_objetosSelecionados.clear();
}

void Plano::selecionarObjeto(const glm::vec2& posMundo, bool addSelecao) {
    if (!addSelecao) {
        limparSelecao();
    }

    for (int i = m_objetos.size() - 1; i >= 0; --i) {
        if (m_objetos[i]->foiSelecionado(posMundo)) {
            m_objetosSelecionados.push_back(m_objetos[i].get());
            if (!addSelecao) {
                break;
            }
        }
    }
}

void Plano::selecionarArea(const glm::vec2& p1, const glm::vec2& p2) {
    limparSelecao();

    float minX = std::min(p1.x, p2.x);
    float maxX = std::max(p1.x, p2.x);
    float minY = std::min(p1.y, p2.y);
    float maxY = std::max(p1.y, p2.y);

    for (const auto& forma : m_objetos) {
        glm::vec2 centro = forma->getPosicao();
        if (centro.x >= minX && centro.x <= maxX && centro.y >= minY && centro.y <= maxY) {
            m_objetosSelecionados.push_back(forma.get());
        }
    }
}

void Plano::escalarObjetosSelecionados(float fator) {
    for (auto* forma : m_objetosSelecionados) {
        forma->escalar(glm::vec2(fator, fator));
    }
}

void Plano::transladarObjetosSelecionados(double deltaX_tela, double deltaY_tela) {
    float deltaMundoX = deltaX_tela / m_zoomLevel;
    float deltaMundoY = -deltaY_tela / m_zoomLevel;
    glm::vec2 deltaMundo(deltaMundoX, deltaMundoY);

    for (auto* forma : m_objetosSelecionados) {
        forma->transladar(deltaMundo);
    }
}

void Plano::rotacionarObjetosSelecionados(float anguloRad) {
    for (auto* forma : m_objetosSelecionados) {
        forma->rotacionar(anguloRad);
    }
}

void Plano::setCorObjetosSelecionados(const glm::vec3& cor) {
    for (auto* forma : m_objetosSelecionados) {
        forma->setCor(cor);
    }
}

void Plano::removerObjetosSelecionados() {
    if (m_objetosSelecionados.empty()) return;
    
    std::set<Forma*> setSelecionados(m_objetosSelecionados.begin(), m_objetosSelecionados.end());

    m_objetos.erase(
        std::remove_if(
            m_objetos.begin(), 
            m_objetos.end(),
            [&](const std::unique_ptr<Forma>& forma) {
                return setSelecionados.count(forma.get());
            }
        ),
        m_objetos.end()
    );

    limparSelecao();
}

const std::vector<Forma*>& Plano::getObjetosSelecionados() const {
    return m_objetosSelecionados;
}