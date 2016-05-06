#pragma once

#include <list>

#inlude "bbox.h"
#include "ponto4d.h"

class ObjetoGrafico {
	typedef std::list<unique_ptr<ObjetoGrafico>> ListaObjetoGraficos;
	BBox bbox;
	ListaPonto4D ponto;
	struct {double r, double g, double b, double a} cor;
	Trasformer transformer;
	ListaObjetoGraficos objetosGraficos;
};