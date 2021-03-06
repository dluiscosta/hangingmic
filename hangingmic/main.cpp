#include <glut.h>
#include <math.h>
#include <gl.h>
#include <glu.h>

#include "camera.hpp"
#include "microfone.hpp"
#include "microfonePendurado.hpp"
#include "holofote.hpp"

using namespace std;


Camera *camera;
MicrofonePendurado *microfoneP;
Holofote *holofote;

void processaTecla(unsigned char key, int x, int y) { //processa input do teclado, se movendo para frente, tr�s ou para os lados
	if (key == 'a') {
		camera->move(0, -10);
	}
	else if (key == 'd') {
		camera->move(0, 10);
	}
	else if (key == 'w') {
		camera->move(30, 0);
	}
	else if (key == 's') {
		camera->move(-30, 0);
	}
	else if (key == 'l') {
		if (holofote->getLigado())
			holofote->desliga();
		else
			holofote->liga();
	}
}

bool acabouDeEntrar = true;
GLsizei widthJanela, heightJanela;

void processaMovimentoPassivo(GLint x, GLint y) { //processa movimento do mouse sem bot�o apertado, olhando para os lados, cima ou baixo
	int centerX = widthJanela / 2;
	int centerY = heightJanela / 2;

	if (x != centerX || y != centerY) {
		if (!acabouDeEntrar) {
			int movX = x - centerX; // ignora movimentos excedentes a 50 pixels
			if (movX > 50)
				movX = 50;
			if (movX < -50)
				movX = -50;

			int movY = y - centerY;
			if (movY > 50)
				movY = 50;
			if (movY < -50)
				movY = -50;

			camera->rotaciona((GLdouble)movX / 100, -(GLdouble)movY / 100);
		}
		acabouDeEntrar = false;
		glutWarpPointer(centerX, centerY);
	}
}

void processaEntradaSaida(int a) {
	acabouDeEntrar = true;
}

GLdouble *vertices;
int *indices;
int width = 60;
int height = 60;

void normaliza(GLdouble v[3]) {
	GLdouble m = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	v[0] /= m;
	v[1] /= m;
	v[2] /= m;
}

void desenha() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat v[4];

	//plota o piso
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, vertices);

	v[0] = 0.5; v[1] = 0.5; v[2] = 0.5; v[3] = 1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, v);
	v[0] = 0.5; v[1] = 0.5; v[2] = 0.5; v[3] = 1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, v);
	v[0] = 0.3; v[1] = 0.3; v[2] = 0.3; v[3] = 1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, v);
	v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, v);

	glDrawElements(GL_TRIANGLE_STRIP, (width*height) + (width - 1)*(height - 2), GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();


	//plota o microfone
	glPushMatrix();
		glTranslatef(1500, 1500, 2500);
	microfoneP->desenha();
	glPopMatrix();

	
	//plota o holofote
	glPushMatrix();
		glTranslatef(2000, 2000, 3000);

		GLdouble nX[3], nY[3], nZ[3] = { -1.5, -1.3, -6 };
		normaliza(nZ);

		//novo eixo Y = nZ x (0, 0, 1)
		nY[0] = nZ[1];
		nY[1] = -nZ[0];
		nY[2] = 0;
		normaliza(nY);

		//novo eixo X = nY x nZ
		nX[0] = nY[1] * nZ[2] - nY[2] * nZ[1];
		nX[1] = nY[2] * nZ[0] - nY[0] * nZ[2];
		nX[2] = nY[0] * nZ[1] - nY[1] * nZ[0];
		normaliza(nX);

		GLdouble m[16] = { nX[0], nX[1], nX[2], 0,
						   nY[0], nY[1], nY[2], 0,
						   nZ[0], nZ[1], nZ[2], 0,
							0, 0, 0, 1 };
	
		glMultMatrixd(m); //aplica a matriz de rotacao

		holofote->desenha();
	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}


void alteraTamanhoJanela(GLsizei w, GLsizei h) {
	// Para previnir uma divis�o por zero
	if (h == 0) h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Especifica a proje��o perspectiva
	gluPerspective(45, (GLdouble)w/(GLdouble)h, 0.1, 10000);

	widthJanela = w;
	heightJanela = h;

	camera->rotaciona(0, 0);
}

void geraPiso() {
	vertices = new GLdouble[width * height * 3];
	indices = new int[(width*height) + (width - 1)*(height - 2)];

	int i = 0;
	for (int row = 0; row<height; row++) {
		for (int col = 0; col<width; col++) {
			vertices[i++] = 50 * col;
			vertices[i++] = 50 * row;
			vertices[i++] = 0;
		}
	}

	i = 0;
	for (int row = 0; row<height - 1; row++) {
		if ((row & 1) == 0) { // even rows
			for (int col = 0; col<width; col++) {
				indices[i++] = col + row * width;
				indices[i++] = col + (row + 1) * width;
			}
		}
		else { // odd rows
			for (int col = width - 1; col>0; col--) {
				indices[i++] = col + (row + 1) * width;
				indices[i++] = col - 1 + +row * width;
			}
		}
	}
	if ((height & 1) && height>2) {
		indices[i++] = (height - 1) * width;
	}
}

void configuraSpotlight() {
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 12.0);
	GLfloat p6[] = { 0.3, 0.3, 0.3, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, p6);
	GLfloat p7[] = { 0.8, 0.8, 0.8, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, p7);
	GLfloat p8[] = { 0.8, 0.8, 0.8, 1 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, p8);
	glEnable(GL_LIGHT0);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(widthJanela = 800, heightJanela = 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Tres objetos");

	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0); //apenas 1 face considerada por pol�gono

	//iluminacao ambiente padr�o desabilitada
	GLfloat p4[] = { 0.05, 0.05, 0.05, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, p4);

	camera = new Camera(1500, 0, 1180, 0, 1, 0, 0, 0, 1);

	microfoneP = new MicrofonePendurado(1240);

	holofote = new Holofote(true);

	geraPiso();

	//configuraSpotlight();

	glutDisplayFunc(desenha);
	glutReshapeFunc(alteraTamanhoJanela);
	glutKeyboardFunc(processaTecla);
	glutPassiveMotionFunc(processaMovimentoPassivo);
	glutEntryFunc(processaEntradaSaida);

	glutMainLoop();
	return 0;
}