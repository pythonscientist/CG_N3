#pragma once

#include <vector>
#include <memory>
#include "objetografico.h"
#include "point4d.h"

struct Mundo {
	std::vector<std::shared_ptr<ObjetoGrafico>> objetosGraficos;
	
	void adicionarNovoObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPoints);
	
	int contaObjetosGraficos();
};
