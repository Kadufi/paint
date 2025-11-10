#include "plano.hpp" 
#include <glm/gtc/matrix_transform.hpp>

// Inicializa os membros
Plano::Plano(int largura, int altura) : 
    m_cameraPos(0.0f),
    m_zoomLevel(1.0f),
    m_larguraJanela(largura),
    m_alturaJanela(altura)
{}

// Destrutor
Plano::~Plano() {
}

// func aux 
void Plano::zoomIn(float fator) {
    m_zoomLevel *= fator;
}

void Plano::zoomOut(float fator) {
    m_zoomLevel /= fator;
}

// "move a camera" em função do mause
void Plano::pan(float deltaX_tela, float deltaY_tela) {
    m_cameraPos.x -= deltaX_tela / m_zoomLevel;
    m_cameraPos.y += deltaY_tela / m_zoomLevel; 
}

/*
- Cria matriz identidade, realiza a translação da matriz em funçao da posição da camera
translação na direção oposta, o mundo se move não a camera
*/
glm::mat4 Plano::getMatrizView() const {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-m_cameraPos.x, -m_cameraPos.y, 0.0f));
    return view;
}

/*
- Define o campo de visão da camera, m_zoomLevel afeta o campo de visao no qual é atualizado
pelo glm:ortho
*/
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

/*
- Mouse em coordenadas de tela (0,0)sup esq, OpenGL NDC (0,0)centro, utiliza a matriz de zoom e pan,
calcula a inversa, porm fim calcula a posição do mause no mundo
*/
glm::vec2 Plano::converterMouseParaMundo(double mouseX, double mouseY) const {
    float ndcX = (2.0f * (float)mouseX) / (float)m_larguraJanela - 1.0f; //[0, 800] -> [-1.0, 1.0]
    float ndcY = 1.0f - (2.0f * (float)mouseY) / (float)m_alturaJanela;

    glm::mat4 proj = getMatrizProjection();
    glm::mat4 view = getMatrizView();

    glm::mat4 invVP = glm::inverse(proj * view);

    glm::vec4 worldPos = invVP * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);//X = M⁻¹*Y 

    return glm::vec2(worldPos.x, worldPos.y);
}

float Plano::getZoomLevel() const {
    return m_zoomLevel;
}

void Plano::adicionarForma(std::unique_ptr<Forma> forma) {
    m_objetos.push_back(std::move(forma));
}

/*
- Para cada elemento do vetor de objetos se atualiza a model a cor o shader
- Chama a função de desenho
*/
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

// Redmensionar a janela
void Plano::onTamanhoJanelaMudou(int novaLargura, int novaAltura) {
    m_larguraJanela = novaLargura;
    m_alturaJanela = novaAltura;
}

void Plano::limparSelecao() {
    m_objetosSelecionados.clear();
}

/*
- Para cada forma verifica se foi selecionado, addSelecao serve para mecanica do shift
*/
void Plano::selecionarObjeto(const glm::vec2& posMundo, bool addSelecao) {
    if (!addSelecao) {
        limparSelecao();
    }
    for (int i = m_objetos.size() - 1; i >= 0; --i) {
        if (m_objetos[i]->foiSelecionado(posMundo)) {
            bool jaSelecionado = false;
            if (addSelecao) {
                for (auto* ptrExistente : m_objetosSelecionados) {
                    if (m_objetos[i].get() == ptrExistente) {
                        jaSelecionado = true;
                        break;
                    }
                }
            }
            // Adc o objeto se ele ainda não estiver na lista
            if (!jaSelecionado) {
                m_objetosSelecionados.push_back(m_objetos[i].get());
            }
            // Se nao for um Shift+Clique para no primeiro objeto encontrado
            if (!addSelecao) {
                break;
            }
        }
    }
}
/*
- Selcionar multiplos, verifica se o centro do objeto pertence ao retangulo
*/
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

// Afeta a matriz escala da forma
void Plano::escalarObjetosSelecionados(float fator) {
    for (auto* forma : m_objetosSelecionados) {
        forma->escalar(glm::vec2(fator, fator));
    }
}

// Afeta a matrz posiçao da forma
void Plano::transladarObjetosSelecionados(double deltaX_tela, double deltaY_tela) {
    float deltaMundoX = deltaX_tela / m_zoomLevel;
    float deltaMundoY = -deltaY_tela / m_zoomLevel;
    glm::vec2 deltaMundo(deltaMundoX, deltaMundoY);

    for (auto* forma : m_objetosSelecionados) {
        forma->transladar(deltaMundo);
    }
}

// Afeta a matriz rotação da forma
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


/*
- Remove os objetos selecionados
*/
void Plano::removerObjetosSelecionados() {
    if (m_objetosSelecionados.empty()) return;
    for (int i = m_objetos.size() - 1; i >= 0; --i) {
        for (auto* ptrSelecionado : m_objetosSelecionados) {
            if (m_objetos[i].get() == ptrSelecionado) {
                m_objetos.erase(m_objetos.begin() + i);
            }
        }
    }
    limparSelecao();
}

const std::vector<Forma*>& Plano::getObjetosSelecionados() const {
    return m_objetosSelecionados;
}