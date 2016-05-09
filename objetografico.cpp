#include "objetografico.h"

#include <vector>
#include <memory>

#include "boundingbox.h"
#include "point4d.h"
#include "color.h"
#include "transform.h"
#include "mathdef.h"


ObjetoGrafico::ObjetoGrafico() {};

ObjetoGrafico::ObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPontos, ObjetoGrafico *pai) {
	
	for (auto p : pPontos) {
		pontos.push_back(p);
	}
	
	transform.MakeIdentity();
	updateBBox();
	this->pai = pai;
	
	std::cout << "novo objeto grafico em " << this << std::endl;
	
}

Limite ObjetoGrafico::obterLimite(bool transform_points) {
	Limite limite;
	for (auto o : pontos) {

		VART::Point4D r(o.get());
		if (transform_points)
			transform.ApplyTo(&r);

		if (r.GetX() < limite.minX) {
			limite.minX = r.GetX();
		}
		
		if (r.GetY() < limite.minY) {
			limite.minY = r.GetY();
		}
		
		if (r.GetX() > limite.maxX) {
			limite.maxX = r.GetX();
		}
		
		if (r.GetY() > limite.maxY) {
			limite.maxY = r.GetY();
		}
	}
	return limite;
}

void ObjetoGrafico::updateBBox() {
	Limite limite = obterLimite();
	bbox.SetBoundingBox(limite.minX, limite.minY, 0, limite.maxX, limite.maxY, 0);
}

int ObjetoGrafico::contaObjetosGraficos() {
	int contagem = 0;
	for (auto x : objetosGraficos) {
		contagem += x->contaObjetosGraficos();			
	}
	return contagem + objetosGraficos.size();
}

ObjetoGrafico* ObjetoGrafico::procuraObjetoXY(int x, int y) {
	
	ObjetoGrafico *objetoXY = nullptr;
	updateBBox();
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

void ObjetoGrafico::moverCima(double n) {
	VART::Transform t;
	t.MakeIdentity();
	t.MakeTranslation(0.0f, n, 0.0f);
	transform = transform * t;

	for (auto o : objetosGraficos) {
		o->moverCima(n);
	}
	
}

void ObjetoGrafico::moverBaixo(double n) {
	VART::Transform t;
	t.MakeIdentity();
	t.MakeTranslation(0.0f, -n, 0.0f);
	transform = transform * t;
	
	for (auto o : objetosGraficos) {
		o->moverBaixo(n);
	}
}

void ObjetoGrafico::moverEsquerda(double n) {
	VART::Transform t;
	t.MakeIdentity();
	t.MakeTranslation(-n, 0.0f, 0.0f);
	transform = transform * t;
	
	for (auto o : objetosGraficos) {
		o->moverEsquerda(n);
	}
}

void ObjetoGrafico::moverDireita(double n) {
	VART::Transform t;
	t.MakeIdentity();
	t.MakeTranslation(n, 0.0f, 0.0f);
	transform = transform * t;
	
	for (auto o : objetosGraficos) {
		o->moverDireita(n);
	}
}

void ObjetoGrafico::rotaciona(double n, VART::Point4D *p) {
	VART::Transform t; //translação
	VART::Transform r; // rotacao
	VART::Transform i; // translação inversa
	VART::Point4D pto = bbox.GetCenter();
	if (p != nullptr && 0) {
		pto = VART::Point4D(p);
	}

	// rotacao em si
	r.MakeZRotation(RAS_DEG_TO_RAD * n);

	// tranlacao inversa, voltando a posicao original
	i.MakeTranslation(pto);

	// tranlacao para origem
	pto = -pto;
	t.MakeTranslation(pto);
	
	transform = transform * (i * (r * t));
	
	for (auto o : objetosGraficos) {
		o->rotaciona(n, &pto);
	}
}

void ObjetoGrafico::escalaAmplia(double n, VART::Point4D *p) {
	VART::Transform t; //translação
	VART::Transform e; // escala
	VART::Transform i; // translação inversa
	VART::Point4D pto = bbox.GetCenter();
	if (p != nullptr) {
		pto = VART::Point4D(p);
	}

	// escala em si
	e.MakeScale(n, n, 0);

	// tranlacao inversa, voltando a posicao original
	i.MakeTranslation(pto);

	// tranlacao para origem
	pto = -pto;
	t.MakeTranslation(pto);

	transform = transform * (i * (e * t));
	
	for (auto o : objetosGraficos) {
		o->escalaAmplia(n, &pto);
	}
}

void ObjetoGrafico::escalaReduz(double n, VART::Point4D *p) {
	VART::Transform t; //translação
	VART::Transform e; // escala
	VART::Transform i; // translação inversa
	VART::Point4D pto = bbox.GetCenter();
	if (p != nullptr) {
		pto = VART::Point4D(p);
	}

	// escala em si
	e.MakeScale(1.0f/n, 1.0f/n, 0);

	// tranlacao inversa, voltando a posicao original
	i.MakeTranslation(pto);

	// tranlacao para origem
	pto = -pto;
	t.MakeTranslation(pto);

	transform = transform * (i * (e * t));
	for (auto o : objetosGraficos) {
		o->escalaReduz(n, &pto);
	}
}

void ObjetoGrafico::adicionarNovoObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPoints) {
	objetosGraficos.push_back(std::shared_ptr<ObjetoGrafico>(new ObjetoGrafico(pPoints, this)));
}
