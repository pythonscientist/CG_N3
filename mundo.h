#pragma once

#include <list>
#include <vector>
#include "objetografico.h"
#include "point4d.h"

struct Mundo {
	std::list<ObjetoGrafico> listaObjetosGraficos;
	
	
	void adicionarNovoObjetoGrafico(std::vector<VART::Point4D> pPoints) {
			listaObjetosGraficos.push_back(ObjetoGrafico(pPoints));
	}
	
	int contaObjetosGraficos() {
		int contagem = 0;
		for (auto x : listaObjetosGraficos) {
			contagem += x.contaObjetosGraficos();			
		}
		return contagem + listaObjetosGraficos.size();
	}
	
};