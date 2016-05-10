#include "main.h"

#include <iostream>
#include <vector>
#include <memory>

#include <GL/glut.h>
#include <math.h>

#include "constantes.h"
#include "point4d.h"
#include "mundo.h"
#include "pnpoly.h"

#define EDICAO 0
#define MANIPULACAO 1

GLint estado_atual = MANIPULACAO;
std::vector<std::shared_ptr<VART::Point4D>> pontos_edicao;
Mundo mundo;
ObjetoGrafico *objeto_selecionado = nullptr;

// manipula o vertice selecionado
VART::Point4D *vertice_selecionado = nullptr;
ObjetoGrafico *vertice_selecionado_pai = nullptr;
int vertice_selecionado_posicao = 0;

GLint gJanelaPrincipal = 0;
GLint janelaLargura = 400, janelaAltura = 400;
GLfloat ortho2D_minX = -400.0f, ortho2D_maxX = 400.0f, ortho2D_minY = -400.0f, ortho2D_maxY = 400.0f;
GLfloat orthoLargura = ortho2D_maxX - ortho2D_minX;
GLfloat orthoAltura  = ortho2D_maxY - ortho2D_minY;

void inicializacao(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
}

void exibicaoPrincipal(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(ortho2D_minX, ortho2D_maxX, ortho2D_minY, ortho2D_maxY);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SRU();
	////////
	if (estado_atual == EDICAO) {
		desenhaPontos_pontosEdicao();
		desenhaVertices_pontosEdicao();
		desenhaVerticeSelecionado();
		desenhaObjetosGraficosEFilhos_sem_transformacao();
		desenhaSelecaoObjetoSelecionado_sem_transformacao();
	} else {
		desenhaObjetosGraficosEFilhos();
		desenhaSelecaoObjetoSelecionado();
	}
	
	///////
	glutSwapBuffers();
}

void desenhaVertices_pontosEdicao() {
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (auto x : pontos_edicao) {
		glVertex2f(x->GetX(), x->GetY());
	}
	glEnd();
}

void desenhaPontos_pontosEdicao() {
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (auto x : pontos_edicao) {
			glVertex2f(x->GetX(), x->GetY());
		}
	glEnd();
}

void desenhaObjetosGraficosEFilhos() {
	for (auto x : mundo.objetosGraficos) {
		
		// desenha os filhos
		for (auto y : x->objetosGraficos) {
			glPushMatrix();
		        glMultMatrixd(y->transform.GetData());
			glColor3f(y->cor.GetR1f(), y->cor.GetG1f(), y->cor.GetB1f());
			glBegin(GL_LINE_STRIP);
				for (auto z : y->pontos) {
					glVertex2f(z->GetX(), z->GetY());
				}
			glEnd();
			glPopMatrix();
		}
		
		// desenha o pai
		glPushMatrix();
		glMultMatrixd(x->transform.GetData());
		glColor3f(x->cor.GetR1f(), x->cor.GetG1f(), x->cor.GetB1f());
		glBegin(GL_LINE_STRIP);
			for (auto z : x->pontos) {
				glVertex2f(z->GetX(), z->GetY());
			}
		glEnd();
		glPopMatrix();
	}
}

void desenhaObjetosGraficosEFilhos_sem_transformacao() {
	for (auto x : mundo.objetosGraficos) {
		
		// desenha os filhos
		for (auto y : x->objetosGraficos) {
			glColor3f(y->cor.GetR1f(), y->cor.GetG1f(), y->cor.GetB1f());
			glBegin(GL_LINE_STRIP);
				for (auto z : y->pontos) {
					glVertex2f(z->GetX(), z->GetY());
				}
			glEnd();
		}
		
		// desenha o pai
		glColor3f(x->cor.GetR1f(), x->cor.GetG1f(), x->cor.GetB1f());
		glBegin(GL_LINE_STRIP);
			for (auto z : x->pontos) {
				glVertex2f(z->GetX(), z->GetY());
			}
		glEnd();
	}
}

void selecionaPoligonoClick(GLint x, GLint y) {
	for (auto o : mundo.objetosGraficos) {
		ObjetoGrafico* objeto = o->procuraObjetoXY(x, y);
		if (objeto != nullptr && pnpoly(objeto->pontos, objeto->transform, (float)x, (float)y)) {
			objeto_selecionado = objeto;
			return;
		}
	}
	objeto_selecionado = nullptr;
}

void selecionaVerticeClick(GLint x, GLint y) {
	double distancia = 9999999;
	int count = 0;
	for (auto o : mundo.objetosGraficos) {
		for (int i=0; i<o->pontos.size(); i++) {
			auto p = o->pontos[i].get();
			double point_dist = sqrt(pow((p->GetX()-x),2) + pow((p->GetY()-y),2));
			std::cout << "distancia = " << point_dist << std::endl;
			if (point_dist < distancia && point_dist < 10) {
				distancia = point_dist;
				vertice_selecionado = p;
				vertice_selecionado_pai = o.get();
				vertice_selecionado_posicao = i;
				std::cout << "vertice foi selecionado em x=" << p->GetX() << " y= " << p->GetY() << std::endl;
				count += 1;
			}
		}

	}

	if (count == 0) {
		vertice_selecionado = nullptr;
		vertice_selecionado_pai = nullptr;
		vertice_selecionado_posicao = 0;
	}
}

void desenhaVerticeSelecionado() {
	if (vertice_selecionado != nullptr && vertice_selecionado_pai != nullptr) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			glVertex2f(vertice_selecionado->GetX(), vertice_selecionado->GetY());
		glEnd();
	}
}

void desenhaSelecaoObjetoSelecionado() {
	if (objeto_selecionado != nullptr) {
		
		Limite l = objeto_selecionado->obterLimite();
		
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(l.minX, l.minY);
			glVertex2f(l.minX, l.maxY);
			glVertex2f(l.maxX, l.maxY);
			glVertex2f(l.maxX, l.minY);
		glEnd();
	}
}

void desenhaSelecaoObjetoSelecionado_sem_transformacao() {
	if (objeto_selecionado != nullptr) {
		
		Limite l = objeto_selecionado->obterLimite(false);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(l.minX, l.minY);
			glVertex2f(l.minX, l.maxY);
			glVertex2f(l.maxX, l.maxY);
			glVertex2f(l.maxX, l.minY);
		glEnd();
	}
}

void deletaSelecionado() {
	if (objeto_selecionado != nullptr) {
		ObjetoGrafico *pai = objeto_selecionado->pai;
		if (pai == nullptr) {
		for (int i=0; i < mundo.objetosGraficos.size(); i++) {
			if (mundo.objetosGraficos[i].get() == objeto_selecionado) {
				mundo.objetosGraficos.erase(mundo.objetosGraficos.begin()+i);
				objeto_selecionado = nullptr;
				break;
			}
		}
			
		} else {
		for (int i=0; i < pai->objetosGraficos.size(); i++) {
			if (pai->objetosGraficos[i].get() == objeto_selecionado) {
				pai->objetosGraficos.erase(pai->objetosGraficos.begin()+i);
				objeto_selecionado = nullptr;
				break;
			}
		}
		}
	}
}

void teclaPressionada(unsigned char tecla, int x, int y) {

	switch (tecla) {
	case 'e':
		estado_atual = EDICAO;
		std::cout << "Modo Edição" << std::endl;
		break;
	case 'm':
		estado_atual = MANIPULACAO;
		std::cout << "Manipulacao" << std::endl;
		
		if (pontos_edicao.size() > 0) {
			if (objeto_selecionado == nullptr) {
				mundo.adicionarNovoObjetoGrafico(pontos_edicao);
			} else {
				objeto_selecionado->adicionarNovoObjetoGrafico(pontos_edicao);
			}
			pontos_edicao.clear();
		}
		
		std::cout << "Objetos graficos no mundo = " << mundo.contaObjetosGraficos() << std::endl;
		break;
	case 'w': // move cima
		if (objeto_selecionado != nullptr) {objeto_selecionado->moverCima(5);}
	break;
	case 's': // move baixo
		if (objeto_selecionado != nullptr) {objeto_selecionado->moverBaixo(5);}
	break;
	case 'a': //move esquerda
		if (objeto_selecionado != nullptr) {objeto_selecionado->moverEsquerda(5);}
	break;
	case 'd': // move direita
		if (objeto_selecionado != nullptr) {objeto_selecionado->moverDireita(5);}
	break;
	case 'z': // amplia
		if (objeto_selecionado != nullptr) {objeto_selecionado->escalaAmplia(1.05);}
	break;
	case 'x': // reduz
		if (objeto_selecionado != nullptr) {objeto_selecionado->escalaReduz(1.05);}
	break;
	case 'c': // rotaciona esquerda
		if (objeto_selecionado != nullptr) {objeto_selecionado->rotaciona(5);}
	break;
	case 'v': // roraciona direita
		if (objeto_selecionado != nullptr) {objeto_selecionado->rotaciona(-5);}
	break;
	case 'q': // muda cor 
		if (objeto_selecionado != nullptr) {objeto_selecionado->cor = VART::Color::RANDOM();}
	break;
	case 'r': // deleta poligono selecionado 
		if (objeto_selecionado != nullptr) {deletaSelecionado();}
	break;
	case 'i': // move vertice selecionado cima 
		if (vertice_selecionado != nullptr && estado_atual == EDICAO) {moveVerticeCima(5);}
	break;
	case 'k': // move vertice selecionado baixo
		if (vertice_selecionado != nullptr && estado_atual == EDICAO) {moveVerticeBaixo(5);}
	break;
	case 'j': // move vertice selecionado esquerda
		if (vertice_selecionado != nullptr && estado_atual == EDICAO) {moveVerticeEsquerda(5);}
	break;
	case 'l': // move vertice selecionado direita
		if (vertice_selecionado != nullptr && estado_atual == EDICAO) {moveVerticeDireita(5);}
	break;
	case 'p': // deleta vertice selecionado
		if (vertice_selecionado != nullptr && estado_atual == EDICAO) {deletaVerticeSelecionado();}
	break;
	}

	glutPostRedisplay();
}

void moveVerticeCima(double n) {
	vertice_selecionado->SetY(vertice_selecionado->GetY()+n);
}
void moveVerticeBaixo(double n) {
	vertice_selecionado->SetY(vertice_selecionado->GetY()-n);
}
void moveVerticeEsquerda(double n) {
	vertice_selecionado->SetX(vertice_selecionado->GetX()-n);
}
void moveVerticeDireita(double n) {
	vertice_selecionado->SetX(vertice_selecionado->GetX()+n);
}

void deletaVerticeSelecionado() {
	if (vertice_selecionado != nullptr) {
		vertice_selecionado_pai->pontos.erase(vertice_selecionado_pai->pontos.begin()+vertice_selecionado_posicao);
		vertice_selecionado = nullptr;
		vertice_selecionado_pai = nullptr;
		vertice_selecionado_posicao = 0;
	}
}

void mouseEvento(GLint botao, GLint estado, GLint x, GLint y) {
	
	// calcula a posição como se o ponto 0,0 fosse no centro
	 float nx =  (float)(x - (janelaLargura/2));
	 float ny =  (float)(y - (janelaAltura/2));
	 
	 // inverte o Y para igualar as cordenadas do Opengl
	 ny *= -1;
	 
	 // calcula a porcentagem da posição com relação aos quadrantes da janela
	 nx = nx / (janelaLargura/2);
	 ny = ny / (janelaAltura/2);
	 
	 // calcula a posição correta com relação ao opengl
	 nx *= orthoLargura/2;
	 ny *= orthoAltura/2;
	 
	 std::cout << "x = " << nx << " y = " << ny << std::endl;
	 
	 // a cada click devemos armazenar o ponto para criar um poligono
	 if (botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		 if (estado_atual == EDICAO) {
			pontos_edicao.push_back(std::shared_ptr<VART::Point4D>(new VART::Point4D(nx, ny, 0, 1)));
		 } else if (estado_atual == MANIPULACAO) {
			 std::cout << "procurando objeto em x= " << nx << " y= " << ny << std::endl;
			 selecionaPoligonoClick(nx, ny);
			 if (objeto_selecionado != nullptr) {
				 std::cout << "Objeto selecionado = " << objeto_selecionado << std::endl;
			 }
		 }
    	} else if (botao == GLUT_RIGHT_BUTTON && estado == GLUT_DOWN) {
		if (estado_atual == EDICAO) {
			selecionaVerticeClick(nx, ny);
		}
	}
	
    glutPostRedisplay();
}
void mouseMovimento(GLint x, GLint y) {
}

void redimensionaJanela(GLint w, GLint h){
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
	
	// Update ortho variables
	ortho2D_minX  = -w;
	ortho2D_maxX  =  w;
	ortho2D_minY  = -h;
	ortho2D_maxY  =  h;
	janelaLargura =  w;
	janelaAltura  =  h;
	orthoLargura = ortho2D_maxX - ortho2D_minX;
	orthoAltura  = ortho2D_maxY - ortho2D_minY;
}

int main(int argc, const char * argv[]) {
	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(300, 250);
	glutInitWindowSize(janelaLargura, janelaAltura);
	gJanelaPrincipal = glutCreateWindow("OpenGL app");
	inicializacao();
	glutDisplayFunc(exibicaoPrincipal);
	glutKeyboardFunc(teclaPressionada);
	glutMouseFunc(mouseEvento);
	glutMotionFunc(mouseMovimento);
	glutReshapeFunc(redimensionaJanela);
	glutMainLoop();
	return 0;
}
#ifdef WIN32 && DEFINED WINDOWS_APP
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	return main(NULL, 0);
}
#endif
