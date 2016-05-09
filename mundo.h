#pragma once

#include <list>
#include <vector>
#include <memory>
#include "objetografico.h"
#include "point4d.h"

struct Mundo {
	std::list<std::shared_ptr<ObjetoGrafico>> listaObjetosGraficos;
	
	
	void adicionarNovoObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPoints) {
		listaObjetosGraficos.push_back(std::shared_ptr<ObjetoGrafico>(new ObjetoGrafico(pPoints)));
	}
	
	int contaObjetosGraficos() {
		int contagem = 0;
		for (auto x : listaObjetosGraficos) {
			contagem += x->contaObjetosGraficos();			
		}
		return contagem + listaObjetosGraficos.size();
	}
	
};
