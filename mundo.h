#pragma once

#include <vector>
#include <memory>
#include "objetografico.h"
#include "point4d.h"

/// \class Mundo mundo.h
/// \brief representa o mundo onde guarda todo os objetos do programa
struct Mundo {

	/// Contém a lista de objetos gráficos do programa
	std::vector<std::shared_ptr<ObjetoGrafico>> objetosGraficos;
	
	/// \brief Adiciona um novo objeto gráfico ao mundo
	void adicionarNovoObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPoints);
	
	/// \brief COnta a quantidade de objetos gráficos no mundo 
	int contaObjetosGraficos();
};
