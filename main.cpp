#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <math.h>

#include "constantes.h"
#include "point4d.h"

#define EDICAO 0
#define MANIPULACAO 1

GLint estado_atual = MANIPULACAO;
std::vector<VART::Point4D> pontos_edicao;

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
		// draw edicao
	} else {
		// draw manipulacao
	}
	///////

	glutSwapBuffers();
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
		break;
	}
	glutPostRedisplay();
}
void mouseEvento(GLint botao, GLint estado, GLint x, GLint y) {
	 
	 float nx =  (float)(x - (janelaLargura/2));
	 float ny =  (float)(y - (janelaAltura/2));
	 ny *= -1;
	 
	 nx = nx / (janelaLargura/2);
	 ny = ny / (janelaAltura/2);
	 
	 nx *= orthoLargura/2;
	 ny *= orthoAltura/2;
	 
	 std::cout << "x = " << nx << "y = " << ny << std::endl;
	
    glutPostRedisplay();
}
void mouseMovimento(GLint x, GLint y) {
}

void redimensionaJanela(GLint width, GLint height){
	janelaAltura = width;
	janelaLargura = height;
	GLfloat mw = (width/2);
	GLfloat mh = (height/2);
	gluOrtho2D(-mw, mw, -mh, mh);
	glutPostRedisplay();
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
