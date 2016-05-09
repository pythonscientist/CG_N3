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
	
	ObjetoGrafico() {};
	
	ObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPontos) {
		
		for (auto p : pPontos) {
			pontos.push_back(p);
		}
		
		transform.MakeIdentity();
		updateBBox();
		
		std::cout << "novo objeto grafico em " << this << std::endl;
		
	}
	
	Limite obterLimite() {
		Limite limite;
		for (auto o : pontos) {

			VART::Point4D r(o.get());
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

	void updateBBox() {
		Limite limite = obterLimite();
		bbox.SetBoundingBox(limite.minX, limite.minY, 0, limite.maxX, limite.maxY, 0);
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

	void moverCima(double n) {
		VART::Transform t;
		t.MakeIdentity();
		t.MakeTranslation(0.0f, n, 0.0f);
		transform = transform * t;

		for (auto o : objetosGraficos) {
			o->moverCima(n);
		}
		
	}

	void moverBaixo(double n) {
		VART::Transform t;
		t.MakeIdentity();
		t.MakeTranslation(0.0f, -n, 0.0f);
		transform = transform * t;
		
		for (auto o : objetosGraficos) {
			o->moverBaixo(n);
		}
	}

	void moverEsquerda(double n) {
		VART::Transform t;
		t.MakeIdentity();
		t.MakeTranslation(-n, 0.0f, 0.0f);
		transform = transform * t;
		
		for (auto o : objetosGraficos) {
			o->moverEsquerda(n);
		}
	}

	void moverDireita(double n) {
		VART::Transform t;
		t.MakeIdentity();
		t.MakeTranslation(n, 0.0f, 0.0f);
		transform = transform * t;
		
		for (auto o : objetosGraficos) {
			o->moverDireita(n);
		}
	}

	void rotaciona(double n, VART::Point4D *p=nullptr) {
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
	
	void escalaAmplia(double n, VART::Point4D *p=nullptr) {
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
	
	void escalaReduz(double n, VART::Point4D *p=nullptr) {
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
	
	void adicionarNovoObjetoGrafico(std::vector<std::shared_ptr<VART::Point4D>> pPoints) {
		objetosGraficos.push_back(std::shared_ptr<ObjetoGrafico>(new ObjetoGrafico(pPoints)));
	}
};
