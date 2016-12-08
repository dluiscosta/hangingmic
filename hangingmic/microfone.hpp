#pragma once
#include <glut.h>
#include <SOIL.h>

class Microfone {
private:
	GLuint idTexturaCabeca;

public:
	void desenha();
	Microfone();
};

Microfone::Microfone() {
	 idTexturaCabeca = SOIL_load_OGL_texture("C:\\Users\\unifdcosta\\Documents\\Visual Studio 2013\\Projects\\hangingmic3\\Debug\\textura.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
		);
	if (idTexturaCabeca == NULL) {
		printf("Falha ao abrir a textura!\n");
	}
}

void Microfone::desenha() {
	GLfloat v[4]; //vetor comum para passar parâmetros de iluminação

	glPushMatrix();

		GLUquadricObj *q3 = gluNewQuadric();
		gluQuadricDrawStyle(q3, GLU_FILL); //gera a esfera com as faces poligonais preenchidas
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, idTexturaCabeca); //escolhe a textura a ser usada pelo índice
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //um pixel na renderização recebe a cor do pixel mais próximo no mapeamento da imagem
		gluQuadricTexture(q3, 1);
		gluSphere(q3, 40, 10, 10); //cabeça do microfone

		//configura material metálico
		v[0] = 0.7; v[1] = 0.7; v[2] = 0.7; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v);
		v[0] = 0.9; v[1] = 0.9; v[2] = 0.9; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_SPECULAR, v);
		glMateriali(GL_FRONT, GL_SHININESS, 170);
		v[0] = 0.5; v[1] = 0.5; v[2] = 0.5; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_AMBIENT, v);

		glPushMatrix();
			glScaled(1, 1, 2);
			glutSolidTorus(2, 40, 10, 20); //anel metálico em torno da cabeça do microfone
		glPopMatrix();

		glTranslatef(0, 0, -26);

		glPushMatrix();
			glScaled(1, 1, 2.5);
			glutSolidTorus(2, 28, 10, 20); //anel superior no pescoço do microfone
		glPopMatrix();

		glTranslatef(0, 0, -8);

		GLUquadricObj *q1 = gluNewQuadric();
		gluQuadricDrawStyle(q1, GLU_FILL); //gera o cilindro com as faces poligonais preenchidas
		gluCylinder(q1, 28, 29, 4, 10, 20); //anel inferior no pescoço do microfone

		glTranslatef(0, 0, -220);

		//configura material plástico preto opaco
		v[0] = 0.2; v[1] = 0.2; v[2] = 0.2; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v);
		v[0] = 0.6; v[1] = 0.6; v[2] = 0.6; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_SPECULAR, v);
		glMateriali(GL_FRONT, GL_SHININESS, 70);
		v[0] = 0.1; v[1] = 0.1; v[2] = 0.1; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_AMBIENT, v);

		GLUquadricObj *q2 = gluNewQuadric();
		gluQuadricOrientation(q2, GLU_OUTSIDE); //normais geradas apontam para fora do cilindro
		gluQuadricDrawStyle(q2, GLU_FILL); //gera o cilindro com as faces poligonais preenchidas
		gluCylinder(q2, 20, 28, 220, 20, 28); //cabo do microfone

	glPopMatrix();
}