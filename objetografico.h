#pragma once

#include <list>
#include <vector>

#include "boundingbox.h"
#include "point4d.h"
#include "color.h"
#include "transform.h"

struct Limite {
	double minX=999999, minY=999999, maxX=0, maxY=0;
};

struct ObjetoGrafico {
	std::list<VART::Point4D> pontos;
	VART::BoundingBox bbox;
	VART::Color cor;
	VART::Transform transform;
	std::list<ObjetoGrafico> objetosGraficos;
	
	ObjetoGrafico() {};
	
	ObjetoGrafico(std::vector<VART::Point4D> pPontos) {
		
		for (auto p : pPontos) {
			pontos.push_back(p);
		}
		
		Limite limite = obterLimite();
		bbox.SetBoundingBox(limite.minX, limite.minY, 0, limite.maxX, limite.maxY, 0);
		
	}
	
	Limite obterLimite() {
		Limite limite;
		for (auto o : pontos) {
			if (o.GetX() < limite.minX) {
				limite.minX = o.GetX();
			}
			
			if (o.GetY() < limite.minY) {
				limite.minY = o.GetY();
			}
			
			if (o.GetX() > limite.maxX) {
				limite.maxX = o.GetX();
			}
			
			if (o.GetY() > limite.maxY) {
				limite.maxY = o.GetY();
			}
		}
		return limite;
	}
	
	int contaObjetosGraficos() {
		int contagem = 0;
		for (auto x : objetosGraficos) {
			contagem += x.contaObjetosGraficos();			
		}
		return contagem + objetosGraficos.size();
	}
};