#pragma once

#include <vector>
#include <memory>

#include "boundingbox.h"
#include "point4d.h"
#include "color.h"
#include "transform.h"
#include "mathdef.h"

/// \class Limite objetografico.h
/// \brief representa os limites de um polígono
struct Limite {
	double minX=999999, minY=999999, maxX=-999999, maxY=-999999;
};

/// \class ObjetoGrafico objetografico.h
/// \brief representa um objeto gráfico
/// Utilizada para armazenar as propriedades de um objeto gráfico
struct ObjetoGrafico {
	/// Vetor de pontos do polígono
	std::vector<std::shared_ptr<VART::Point4D>> pontos;
	/// Boundingbox do polígono
	VART::BoundingBox bbox;
	/// Cor atribuida
	VART::Color cor;
	/// Transformações realizada na renderização
	VART::Transform transform;
	/// Vetor com os objetos gráficos filhos
	std::vector<std::shared_ptr<ObjetoGrafico>> objetosGraficos;
	/// Pai do objeto gráfico
	ObjetoGrafico *pai;

	/// Cria um objeto vaziu
	ObjetoGrafico();

	/// Cria um objeto gráfico utilizando uma lista de pontos
	ObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPontos, ObjetoGrafico *pai);
	
	/// \brief Obtem o limite do polígono para a construção da bbox
	/// \param trasnform_points aplica ou não a transformação ao limite retornado 
	Limite obterLimite(bool transform_points=true);

	/// \brief Realiza a atualização da bbox
	void updateBBox();
	
	/// \brief Conta a quantidade de objetos gráficos
	int contaObjetosGraficos();
	
	/// \brief Realiza a varredura dos objetos onde a bbox está contida na coordenada
	/// \param x coordenada do eixo X
	/// \param y coordenada do eixo Y
	ObjetoGrafico* procuraObjetoXY(int x, int y);

	/// \brief realiza a translação para cima
	/// \param n quantidade		
	void moverCima(double n);

	/// \brief realiza a translação para baixo
	/// \param n quantidade	
	void moverBaixo(double n);

	/// \brief realiza a translação para esquerda
	/// \param n quantidade	
	void moverEsquerda(double n);

	/// \brief realiza a translação para direita
	/// \param n quantidade	
	void moverDireita(double n);

	/// \brief realiza a rotação
	/// \param n quantidade
	/// \param p centro	
	void rotaciona(double n, VART::Point4D *p=nullptr);

	/// \brief realiza a ampliação
	/// \param n quantidade
	/// \param p centro		
	void escalaAmplia(double n, VART::Point4D *p=nullptr);

	/// \brief realiza a redução
	/// \param n quantidade
	/// \param p centro		
	void escalaReduz(double n, VART::Point4D *p=nullptr);

	/// \brief Adiciona um novo objeto gráfico filho
	/// \param pPoints pontos		
	void adicionarNovoObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPoints);
};
