#pragma once

#include <GL/glut.h>
#include <math.h>

/// \brief Desenha os vertices em modo edição para poder visualizar melhor
void desenhaVertices_pontosEdicao();

/// \brief Desenha os pontos em modo edição para poder visualizar melhor
void desenhaPontos_pontosEdicao();

/// \brief Desenha os objetos gráficos do mundo e seus filhos
void desenhaObjetosGraficosEFilhos();

/// \brief Seleciona um polígono clicando que esteja contino nas coordenadas
/// \param x coordenada eixo X
/// \param y coordenada eixo Y
void selecionaPoligonoClick(GLint x, GLint y);

/// \brief Desenha a seleção do objeto, ou seja os limites da bbox
void desenhaSelecaoObjetoSelecionado();

/// \brief deleta um objeto gráfico que esteja selecionado do mundo
void deletaSelecionado();

/// \brief seleciona um vértice de um objeto gráfico que esteja proximo do click
/// \param x coordenada eixo X
/// \param y coordenada eixo Y
void selecionaVerticeClick(GLint x, GLint y);

/// \brief desenha um ponto sobre o vértice selecionado
void desenhaVerticeSelecionado();

/// \brief Desenha os objetos gráficos do mundo e seus filhos sem transformação para realizar a edição
void desenhaObjetosGraficosEFilhos_sem_transformacao();

/// \brief Desenha a seleção do objeto, ou seja os limites da bbox sem transformação para realizar a edição
void desenhaSelecaoObjetoSelecionado_sem_transformacao();

/// \brief move o vertice selecionado para cima
/// \param n quantidade
void moveVerticeCima(double n);

/// \brief move o vertice selecionado para baixo
/// \param n quantidade
void moveVerticeBaixo(double n);

/// \brief move o vertice selecionado para esquerda
/// \param n quantidade
void moveVerticeEsquerda(double n);

/// \brief move o vertice selecionado para direita
/// \param n quantidade
void moveVerticeDireita(double n);

/// \brief deleta o vértice selecionado
void deletaVerticeSelecionado();
