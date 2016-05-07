#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <math.h>

#include "constantes.h"
#include "point4d.h"
#include "mundo.h"

#define EDICAO 0
#define MANIPULACAO 1

// funções
void desenhaVertices_pontosEdicao();
void desenhaPontos_pontosEdicao();
void criaNovoObjetoGrafico();
void desenhaObjetosGraficosEFilhos();
// fim funções

GLint estado_atual = MANIPULACAO;
std::vector<VART::Point4D> pontos_edicao;
Mundo mundo;

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
		desenhaVertices_pontosEdicao();
		desenhaPontos_pontosEdicao();
	} else {
		desenhaObjetosGraficosEFilhos();
	}
	///////
	glutSwapBuffers();
}

void desenhaVertices_pontosEdicao() {
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (auto x : pontos_edicao) {
		glVertex2f(x.GetX(), x.GetY());
	}
	glEnd();
}

void desenhaPontos_pontosEdicao() {
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (auto x : pontos_edicao) {
			glVertex2f(x.GetX(), x.GetY());
		}
	glEnd();
}

void desenhaObjetosGraficosEFilhos() {
	for (auto x : mundo.listaObjetosGraficos) {
		
		// desenha os filhos
		for (auto y : x.objetosGraficos) {
			glColor3f(y.cor.GetR1f(), y.cor.GetG1f(), y.cor.GetB1f());
			glBegin(GL_LINE_STRIP);
				for (auto z :y.pontos) {
					glVertex2f(z.GetX(), z.GetY());
				}
			glEnd();
		}
		
		// desenha o pai
		glColor3f(x.cor.GetR1f(), x.cor.GetG1f(), x.cor.GetB1f());
		glBegin(GL_LINE_STRIP);
			for (auto z :x.pontos) {
				glVertex2f(z.GetX(), z.GetY());
			}
		glEnd();
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
			mundo.adicionarNovoObjetoGrafico(pontos_edicao);
			pontos_edicao.clear();
		}
		
		std::cout << "Objetos graficos no mundo = " << mundo.contaObjetosGraficos() << std::endl;
		
		break;
	}
	glutPostRedisplay();
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
	 if (estado == GLUT_DOWN && estado_atual == EDICAO) {
        pontos_edicao.push_back(VART::Point4D(nx, ny, 0, 1));
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