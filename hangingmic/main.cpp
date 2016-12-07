#include <glut.h>
#include <math.h>


#include "camera.hpp"
#include "microfone.hpp"
#include "microfonePendurado.hpp"

using namespace std;

#define PI 3.1415926535897932384626433832795

Camera *camera;
MicrofonePendurado *microfoneP;

void processaTecla(unsigned char key, int x, int y) { //processa input do teclado, se movendo para frente, trás ou para os lados
	if (key == 'a') {
		camera->move(0, -10);
	}
	else if (key == 'd') {
		camera->move(0, 10);
	}
	if (key == 'w') {
		camera->move(30, 0);
	}
	else if (key == 's') {
		camera->move(-30, 0);
	}
}

bool acabouDeEntrar = true;

void processaMovimentoPassivo(int x, int y) { //processa movimento do mouse sem botão apertado, olhando para os lados, cima ou baixo
	static int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	static int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

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

void desenha() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat v[4];

	//esfera na origem do spotlight
	glPushMatrix();
		glTranslatef(2000, 2000, 3000);
		glColor3f(0.3, 0.2, 0.1);
		glutWireSphere(10, 5, 5);
	glPopMatrix();


	//plota o piso
	glPushMatrix();
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vertices);

		v[0] = 0.5; v[1] = 0.5; v[2] = 0.5; v[3] = 1;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, v);
		v[0] = 0.5; v[1] = 0.5; v[2] = 0.5; v[3] = 1;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, v);

		glDrawElements(GL_TRIANGLE_STRIP, (width*height) + (width - 1)*(height - 2), GL_UNSIGNED_INT, indices);
		glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	// posicao do spotlight
	GLfloat p1[] = { 2000.0, 2000.0, 3000.0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, p1);
	GLfloat p2[] = { -1.0, -1.0, -6.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, p2);

	glPushMatrix();
	glTranslatef(1500, 1500, 2850);
	microfoneP->desenha();
	glPopMatrix();


	glutSwapBuffers();
}


void alteraTamanhoJanela(GLsizei w, GLsizei h) {
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Especifica a projeção perspectiva
	gluPerspective(45, (GLfloat)GLUT_WINDOW_HEIGHT / (GLfloat)GLUT_WINDOW_WIDTH, 0.1, 10000);
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
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 18.0);
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
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Tres objetos");
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0); //apenas 1 face considerada por polígono

	//iluminacao ambiente padrão desabilitada
	GLfloat p4[] = { 0, 0, 0, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, p4);

	camera = new Camera(1500, 0, 1500, 0, 1, 0, 0, 0, 1);

	microfoneP = new MicrofonePendurado(1240);

	geraPiso();

	configuraSpotlight();

	glutDisplayFunc(desenha);
	glutReshapeFunc(alteraTamanhoJanela);
	glutKeyboardFunc(processaTecla);
	glutPassiveMotionFunc(processaMovimentoPassivo);
	glutEntryFunc(processaEntradaSaida);

	glutMainLoop();
	return 0;
}