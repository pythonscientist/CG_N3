#pragma once

#include <vector>
#include <memory>

#include "boundingbox.h"
#include "point4d.h"
#include "color.h"
#include "transform.h"
#include "mathdef.h"

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
		
	}

	void moverBaixo(double n) {
		VART::Transform t;
		t.MakeIdentity();
		t.MakeTranslation(0.0f, -n, 0.0f);
		transform = transform * t;
	}

	void moverEsquerda(double n) {
		VART::Transform t;
		t.MakeIdentity();
		t.MakeTranslation(-n, 0.0f, 0.0f);
		transform = transform * t;
	}

	void moverDireita(double n) {
		VART::Transform t;
		t.MakeIdentity();
		t.MakeTranslation(n, 0.0f, 0.0f);
		transform = transform * t;
	}

	void rotaciona(double n) {
		VART::Transform t; //translação
		VART::Transform r; // rotacao
		VART::Transform i; // translação inversa
		VART::Point4D pto = bbox.GetCenter();
    
		// rotacao em si
		r.MakeZRotation(RAS_DEG_TO_RAD * n);
    
		// tranlacao inversa, voltando a posicao original
		i.MakeTranslation(pto);
    
		// tranlacao para origem
		pto = -pto;
		t.MakeTranslation(pto);
		
		transform = transform * (i * (r * t));
	}
	
	void escalaAmplia(double n) {
		VART::Transform t; //translação
		VART::Transform e; // escala
		VART::Transform i; // translação inversa
		VART::Point4D pto = bbox.GetCenter();

		// escala em si
		e.MakeScale(n, n, 0);

		// tranlacao inversa, voltando a posicao original
		i.MakeTranslation(pto);
    
		// tranlacao para origem
		pto = -pto;
		t.MakeTranslation(pto);
    
		transform = transform * (i * (e * t));
	}
	
	void escalaReduz(double n) {
		VART::Transform t; //translação
		VART::Transform e; // escala
		VART::Transform i; // translação inversa
		VART::Point4D pto = bbox.GetCenter();

		// escala em si
		e.MakeScale(1.0f/n, 1.0f/n, 0);

		// tranlacao inversa, voltando a posicao original
		i.MakeTranslation(pto);
    
		// tranlacao para origem
		pto = -pto;
		t.MakeTranslation(pto);
    
		transform = transform * (i * (e * t));
	}
};
