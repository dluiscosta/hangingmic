#pragma once
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <SOIL.h>

class Holofote {
private:
	GLdouble alcance;
	GLuint idTexturaLampada;
	bool ligado;

public:
	Holofote(bool ligado = false, GLdouble alcance = 3500);
	void desenha();
	void liga();
	void desliga();
	bool getLigado();
};


Holofote::Holofote(bool ligado, GLdouble alcance) : ligado(ligado), alcance(alcance) {
	idTexturaLampada = SOIL_load_OGL_texture("C:\\Users\\dluis\\OneDrive\\Documentos\\Visual Studio 2015\\Projects\\hangingmic.git\\Debug\\textura2.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
		);
	if (idTexturaLampada == NULL) {
		printf("Falha ao abrir a textura!\n");
	}

	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0);
	GLfloat p6[] = { 0.2, 0.2, 0.2, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, p6);
	GLfloat p7[] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, p7);
	GLfloat p8[] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, p8);

	if(ligado)
		glEnable(GL_LIGHT0);
}

void Holofote::desenha() {
	GLfloat v[4]; //vetor comum para passar parâmetros de iluminação

	glPushMatrix();

		//configura material plástico preto opaco
		v[0] = 0.2; v[1] = 0.2; v[2] = 0.2; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v);
		v[0] = 0.6; v[1] = 0.6; v[2] = 0.6; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_SPECULAR, v);
		glMateriali(GL_FRONT, GL_SHININESS, 70);
		v[0] = 0.1; v[1] = 0.1; v[2] = 0.1; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_AMBIENT, v);
		v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_EMISSION, v);

		glTranslatef(0, 0, 100);

		glPushMatrix();
			glScaled(1, 1, (GLdouble)200/(GLdouble)140);
			glutSolidCube(140); //caixa do holofote
		glPopMatrix();

		glTranslatef(0, 0, 100);
		GLUquadricObj *q1 = gluNewQuadric();
		gluQuadricDrawStyle(q1, GLU_FILL); //gera o cilindro com as faces poligonais preenchidas
		gluCylinder(q1, 70, 70, 35, 10, 2); //anel escuro que envolve a lampada

		glTranslatef(0, 0, 35);
		GLUquadricObj *q2 = gluNewQuadric();
		gluQuadricDrawStyle(q2, GLU_FILL);
		gluDisk(q2, 50, 70, 10, 2); //topo do anel escuro

		//configura material lampada
		v[0] = 0.2; v[1] = 0.2; v[2] = 0.2; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v);
		v[0] = 0.6; v[1] = 0.6; v[2] = 0.6; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_SPECULAR, v);
		glMateriali(GL_FRONT, GL_SHININESS, 70);
		v[0] = 0.1; v[1] = 0.1; v[2] = 0.1; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_AMBIENT, v);
		if (ligado) {
			v[0] = 0.8; v[1] = 0.8; v[2] = 0.8; v[3] = 1;
		}
		else {
			v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 1;
		}
		glMaterialfv(GL_FRONT, GL_EMISSION, v);

		glTranslatef(0, 0, 7);
		GLUquadricObj *q3 = gluNewQuadric();
		gluQuadricDrawStyle(q3, GLU_FILL); //gera o cilindro com as faces poligonais preenchidas
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idTexturaLampada); //escolhe a textura a ser usada pelo índice
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //um pixel na renderização recebe a cor do pixel mais próximo no mapeamento da imagem
		gluQuadricTexture(q3, 1);
		gluDisk(q3, 0, 50, 10, 3); // lampada

		if (ligado) {
			//configura material luz
			v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 1;
			glMaterialfv(GL_FRONT, GL_DIFFUSE, v);
			v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 1;
			glMaterialfv(GL_FRONT, GL_SPECULAR, v);
			glMateriali(GL_FRONT, GL_SHININESS, 0);
			v[0] = 0; v[1] = 0; v[2] = 0; v[3] = 1;
			glMaterialfv(GL_FRONT, GL_AMBIENT, v);
			v[0] = 0.04; v[1] = 0.04; v[2] = 0.04; v[3] = 0.05;
			glMaterialfv(GL_FRONT, GL_EMISSION, v);

			glTranslatef(0, 0, -7);


			glDepthMask(GL_FALSE); //bloqueia gravação no z-buffer, desta maneira a renderização da luz não impede a renderização de objetos atrás (ou dentro) dela
			glEnable(GL_BLEND); //ativa blend de fragmentos no mesmo pixel (não é mais só o mais próximo)
			glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA); //o pixel final é aditivo sobre os fragmentos (valido só apenas para a luz emulada aqui)
			GLUquadricObj *q4 = gluNewQuadric();
			gluQuadricOrientation(q4, GLU_OUTSIDE); //normais geradas apontam para fora do cilindro
			gluQuadricDrawStyle(q4, GLU_FILL); //gera o cilindro com as faces poligonais preenchidas
			gluCylinder(q4, 50, 50 + 0.20791169*(alcance - 235), alcance - 235, 10, (alcance - 235) / 300); //cabo do microfone
			glDisable(GL_BLEND);
			glDepthMask(GL_TRUE);

			//posiciona a luz (luz mesmo)
			GLfloat p1[] = { 0, 0, 0, 1 };
			glLightfv(GL_LIGHT0, GL_POSITION, p1);
			GLfloat p2[] = { 0, 0, 1, 0 };
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, p2);
		}

	glPopMatrix();
}

void Holofote::liga() {
	ligado = true;
	glEnable(GL_LIGHT0);

	glutPostRedisplay();
}

void Holofote::desliga() {
	ligado = false;
	glDisable(GL_LIGHT0);

	glutPostRedisplay();
}

bool Holofote::getLigado() {
	return ligado;
}
