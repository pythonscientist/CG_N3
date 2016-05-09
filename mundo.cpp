#include "mundo.h"

#include <list>
#include <vector>
#include <memory>
#include "objetografico.h"
#include "point4d.h"

void Mundo::adicionarNovoObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPoints) {
	objetosGraficos.push_back(std::shared_ptr<ObjetoGrafico>(new ObjetoGrafico(pPoints, nullptr)));
}

int Mundo::contaObjetosGraficos() {
	int contagem = 0;
	for (auto x : objetosGraficos) {
		contagem += x->contaObjetosGraficos();			
	}
	return contagem + objetosGraficos.size();
}
