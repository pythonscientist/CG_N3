#pragma once

#include <vector>
#include <memory>

#include "boundingbox.h"
#include "point4d.h"
#include "color.h"
#include "transform.h"
#include "mathdef.h"

struct Limite {
	double minX=999999, minY=999999, maxX=-999999, maxY=-999999;
};

struct ObjetoGrafico {
	std::vector<std::shared_ptr<VART::Point4D>> pontos;
	VART::BoundingBox bbox;
	VART::Color cor;
	VART::Transform transform;
	std::vector<std::shared_ptr<ObjetoGrafico>> objetosGraficos;
	ObjetoGrafico *pai;
	
	ObjetoGrafico();
	
	ObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPontos, ObjetoGrafico *pai);
	
	Limite obterLimite(bool transform_points=true);

	void updateBBox();
	
	int contaObjetosGraficos();
	
	ObjetoGrafico* procuraObjetoXY(int x, int y);
		
	void moverCima(double n);

	void moverBaixo(double n);

	void moverEsquerda(double n);

	void moverDireita(double n);

	void rotaciona(double n, VART::Point4D *p=nullptr);
	
	void escalaAmplia(double n, VART::Point4D *p=nullptr);
	
	void escalaReduz(double n, VART::Point4D *p=nullptr);
	
	void adicionarNovoObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPoints);
};
