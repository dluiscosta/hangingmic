#pragma once
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include "microfone.hpp"
#include <time.h>

class MicrofonePendurado : Microfone {
private:
	time_t getTempo();
	void normaliza(GLdouble[3]);
	void atualizaOrientacao(time_t deltaT);
	GLdouble comprimentoCabo;
	GLdouble tamanhoMicrofone;
	GLdouble orientacao[3];
	time_t ultimaAtualizacao;

public:
	void desenha();
	MicrofonePendurado(GLdouble comprimentoCabo = 800, GLdouble tamanhoMicrofone = 1);
};

time_t MicrofonePendurado::getTempo() {
	time_t t;
	time(&t);
	return t;
}

void MicrofonePendurado::normaliza(GLdouble v[3]) {
	GLdouble m = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	v[0] /= m;
	v[1] /= m;
	v[2] /= m;
}

MicrofonePendurado::MicrofonePendurado(GLdouble comprimentoCabo, GLdouble tamanhoMicrofone) :
	comprimentoCabo(comprimentoCabo), tamanhoMicrofone(tamanhoMicrofone) {

	orientacao[0] = 0; orientacao[1] = 0; orientacao[2] = -1; //orientacao inicial pendendo perpendicular ao chao

	ultimaAtualizacao = getTempo();
}

void MicrofonePendurado::atualizaOrientacao(time_t deltaT) {
	//atualizaOrientacao e momentos

	normaliza(orientacao);
}

void MicrofonePendurado::desenha() {
	time_t t = getTempo();

	atualizaOrientacao(t - ultimaAtualizacao);

	glPushMatrix();
		//reorienta com base no vetor armazenado
		GLdouble nX[3], nY[3], nZ[3] = {orientacao[0], orientacao[1], orientacao[2]};

		if (orientacao[0] == 0 && orientacao[1] == 0) {
			nY[0] = 0;
			nY[1] = 1;
			nY[2] = 0;
		}
		else {
			//novo eixo Y = - nZ x (0, 0, 1)
			nY[0] = -nZ[1];
			nY[1] = -nZ[0];
			nY[2] = 0;
			normaliza(nY);
		}

		//novo eixo X = nY x nZ
		nX[0] = nY[1] * nZ[2] + nY[2] * nZ[1];
		nX[1] = nY[0] * nZ[2] + nY[2] * nZ[0];
		nX[2] = nY[1] * nZ[0] + nY[0] * nZ[1];
		normaliza(nX);

		GLdouble m[16] = { nX[0], nX[1], nX[2], 0,
							nY[0], nY[1], nY[2], 0,
							nZ[0], nZ[1], nZ[2], 0,
							0, 0, 0, 1 };
		glMultMatrixd(m); //aplica a matriz de rotacao

		//configura material plastico brilhante
		GLfloat v[4];
		v[0] = 0.2; v[1] = 0.2; v[2] = 0.2; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, v);
		v[0] = 0.5; v[1] = 0.5; v[2] = 0.5; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_SPECULAR, v);
		glMateriali(GL_FRONT, GL_SHININESS, 170);
		v[0] = 0.1; v[1] = 0.1; v[2] = 0.1; v[3] = 1;
		glMaterialfv(GL_FRONT, GL_AMBIENT, v);

		GLUquadricObj *q1 = gluNewQuadric();
		gluQuadricDrawStyle(q1, GLU_FILL); //gera o cilindro com as faces poligonais preenchidas
		gluCylinder(q1, 3, 3, comprimentoCabo, 4, 20); //cabo do microfone

		glTranslatef(0, 0, comprimentoCabo + 260);
		Microfone::desenha();
	glPopMatrix();

	ultimaAtualizacao = t;
}