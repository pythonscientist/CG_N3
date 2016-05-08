#pragma once

#include <vector>
#include <memory>

#include "boundingbox.h"
#include "point4d.h"
#include "color.h"
#include "transform.h"

struct Limite {
	double minX=999999, minY=999999, maxX=0, maxY=0;
};

struct ObjetoGrafico {
	std::vector<std::shared_ptr<VART::Point4D>> pontos;
	VART::BoundingBox bbox;
	VART::Color cor;
	VART::Transform transform;
	std::list<std::shared_ptr<ObjetoGrafico>> objetosGraficos;
	
	ObjetoGrafico() {};
	
	ObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPontos) {
		
		for (auto p : pPontos) {
			pontos.push_back(p);
		}
		
		Limite limite = obterLimite();
		bbox.SetBoundingBox(limite.minX, limite.minY, 0, limite.maxX, limite.maxY, 0);
		
		std::cout << "novo objeto grafico em " << this << std::endl;
		
	}
	
	Limite obterLimite() {
		Limite limite;
		for (auto o : pontos) {
			if (o->GetX() < limite.minX) {
				limite.minX = o->GetX();
			}
			
			if (o->GetY() < limite.minY) {
				limite.minY = o->GetY();
			}
			
			if (o->GetX() > limite.maxX) {
				limite.maxX = o->GetX();
			}
			
			if (o->GetY() > limite.maxY) {
				limite.maxY = o->GetY();
			}
		}
		return limite;
	}
	
	int contaObjetosGraficos() {
		int contagem = 0;
		for (auto x : objetosGraficos) {
			contagem += x->contaObjetosGraficos();			
		}
		return contagem + objetosGraficos.size();
	}
	
	ObjetoGrafico* procuraObjetoXY(int x, int y) {
		
		ObjetoGrafico *objetoXY = nullptr;
		if (bbox.testPoint(VART::Point4D(x, y, 0, 1))) {
				objetoXY = this;
		} else {	
			for (auto o : objetosGraficos) {
				objetoXY = o->procuraObjetoXY(x, y);
				if (objetoXY != nullptr) {
					return objetoXY;
				}
			}
		}			
		return objetoXY;
	}
};
