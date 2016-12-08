#pragma once
#include <glut.h>
#include <math.h>
#include <stdio.h>

class Camera {
private:
	GLdouble posicao[3]; //posicao da camera
	GLdouble frente[3]; //o vetor que indica a direção olhada a partir da câmera
	GLdouble topo[3]; //o vetor que indica a direção superior da câmera
	GLdouble direita[3]; //o vetor que indica o lado direito da câmera
	void normaliza(GLdouble [3]);
	void updateLookAt();

public:
	Camera(GLdouble px = 0, GLdouble py = 0, GLdouble pz = 0, GLdouble fx = 1, GLdouble fy = 0, GLdouble fz = 0, GLdouble tx = 0, GLdouble ty = 1, GLdouble tz = 0);
	void rotaciona(GLdouble hAngle, GLdouble vAngle); //rotaciona a câmera em torno dos próprios eixos, horizontal e/ou verticalmente relativo à visão
	void move(int deFrente, int deLado); //se move para frente, para trás ou para os lados
};

Camera::Camera(GLdouble px, GLdouble py, GLdouble pz, GLdouble fx, GLdouble fy, GLdouble fz, GLdouble tx, GLdouble ty, GLdouble tz) {
	posicao[0] = px;
	posicao[1] = py;
	posicao[2] = pz;

	frente[0] = fx;
	frente[1] = fy;
	frente[2] = fz;

	topo[0] = tx;
	topo[1] = ty;
	topo[2] = tz;

	//produto vetorial frente x topo
	direita[0] = frente[1] * topo[2] - frente[2] * topo[1];
	direita[1] = frente[0] * topo[2] - frente[2] * topo[0];
	direita[2] = frente[0] * topo[1] - frente[1] * topo[0];

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posicao[0], posicao[1], posicao[2], posicao[0] + frente[0], posicao[1] + frente[1], posicao[2] + frente[2], topo[0], topo[1], topo[2]);
}

void Camera::updateLookAt() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posicao[0], posicao[1], posicao[2], posicao[0] + frente[0], posicao[1] + frente[1], posicao[2] + frente[2], topo[0], topo[1], topo[2]);
	glutPostRedisplay();
}

void Camera::normaliza(GLdouble v[3]) {
	GLdouble m = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	v[0] /= m;
	v[1] /= m;
	v[2] /= m;
}

void Camera::rotaciona(GLdouble hAngle, GLdouble vAngle) {
	if (vAngle != 0) {
		if (frente[2] >= -0.8 && vAngle <= 0 || frente[2] <= 0.8 && vAngle >= 0) {
			GLdouble frenteCopia[3] = { frente[0], frente[1], frente[2] };

			GLdouble  c = cos(vAngle), s = sin(vAngle);

			frente[0] = c*frenteCopia[0] + s*topo[0];
			frente[1] = c*frenteCopia[1] + s*topo[1];
			frente[2] = c*frenteCopia[2] + s*topo[2];
			normaliza(frente);

			topo[0] = -s*frenteCopia[0] + c*topo[0];
			topo[1] = -s*frenteCopia[1] + c*topo[1];
			topo[2] = -s*frenteCopia[2] + c*topo[2];
			normaliza(topo);
		}
	}

	if (hAngle != 0) {
		GLdouble  c = cos(-hAngle), s = sin(-hAngle);

		GLdouble frenteCopia[3] = { frente[0], frente[1], frente[2] };
		frente[0] = c*frenteCopia[0] - s*frenteCopia[1];
		frente[1] = s*frenteCopia[0] + c*frenteCopia[1];
		normaliza(frente);

		GLdouble direitaCopia[3] = { direita[0], direita[1], direita[2] };
		direita[0] = c*direitaCopia[0] - s*direitaCopia[1];
		direita[1] = s*direitaCopia[0] + c*direitaCopia[1];
		normaliza(direita);

		GLdouble topoCopia[3] = { topo[0], topo[1], topo[2] };
		topo[0] = c*topoCopia[0] - s*topoCopia[1];
		topo[1] = s*topoCopia[0] + c*topoCopia[1];
		normaliza(topo);
	}

	updateLookAt();
}

void Camera::move(int deFrente, int deLado) {
	if (deLado) {
		posicao[0] += deLado*direita[0];
		posicao[1] += deLado*direita[1];
		posicao[2] += deLado*direita[2];
	}
	if (deFrente) {
		GLdouble v[3] = { frente[0], frente[1], 0 };
		normaliza(v);

		posicao[0] += deFrente*v[0];
		posicao[1] += deFrente*v[1];
	}

	updateLookAt();
}