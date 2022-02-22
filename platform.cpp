#include "platform.h"
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
void normalize(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}
void cross(GLfloat a[3], GLfloat b[3], GLfloat out[3])
{
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

void Platform::DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
	/* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
	// glColor3f (R, G, B);
	/* Desenhar um polígono branco (retângulo) */
	GLint lenght = 18;
	if(B == 255)
	{
		// glBegin(GL_QUADS);
		// 	glVertex3f (gX, -gY-height, -lenght);  // Lado Esquerdo Cima
		// 	glVertex3f (gX+width, -gY-height, -lenght); // Lado Direito Cima
		// 	glVertex3f (gX+width, -gY, -lenght); // Lado Direito Baixo
		// 	glVertex3f (gX, -gY, -lenght);  // 
		// glEnd();
		// glBegin(GL_QUADS);
		// 	// down face
		// 	// glColor3f (255, 255, 255);
		// 	glVertex3f (gX, -gY-height, -lenght);  // Lado Esquerdo Cima
		// 	glVertex3f (gX+width, -gY-height, -lenght); // Lado Direito Cima
		// 	glVertex3f (gX+width, -gY-height, 0.0); // Lado Direito Baixo
		// 	glVertex3f (gX, -gY-height, 0.0);  // Lado Esquerdo Baixo 
		// glEnd();
	}	
	else if (B == 0)
	{
		GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
		GLfloat materialColor[] = { 1.0, 0.0, 1.0, 1.0};
		GLfloat mat_specular[] = { 1.0, 0.0, 1.0, 1.0};
		GLfloat mat_shininess[] = { 128 };
		GLfloat norm[3], a[3], b[3];

		glBegin(GL_QUADS);
			glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
			glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			
			// down face
			a[0] = gX;
			a[1] = -gY-height;
			a[2] = lenght;

			b[0] = gX+width;
			b[1] = -gY-height;
			b[2] = 0.0;

			cross(a,b,norm);
			normalize(norm);
			glNormal3f(norm[0], norm[1], norm[2]);
			glVertex3f (gX, -gY-height, 0.0);  // Lado Esquerdo Baixo 
			glVertex3f (gX+width, -gY-height, 0.0); // Lado Direito Baixo
			glVertex3f (gX+width, -gY-height, -lenght); // Lado Direito Cima
			glVertex3f (gX, -gY-height, -lenght);  // Lado Esquerdo Cima
			
			//left face
			a[0] = gX;
			a[1] = -gY;
			a[2] = -lenght;

			b[0] = gX;
			b[1] = -gY-height;
			b[2] = 0.0;

			cross(a,b,norm);
			normalize(norm);
			glNormal3f(norm[0], norm[1], norm[2]);
			glVertex3f (gX, -gY, -lenght); // Lado Direito Baixo
			glVertex3f (gX, -gY, 0.0);  // Lado Esquerdo Baixo
			glVertex3f (gX, -gY-height, 0.0);  // Lado Esquerdo Cima
			glVertex3f (gX, -gY-height, -lenght); // Lado Direito Cima
			
			//right face
			a[0] = gX+width;
			a[1] = -gY-height;
			a[2] = 0.0;

			b[0] = gX+width;
			b[1] = -gY;
			b[2] = -lenght;

			cross(a,b,norm);
			normalize(norm);
			glNormal3f(norm[0], norm[1], norm[2]);
			glVertex3f (gX+width, -gY, 0.0);  // Lado Esquerdo Baixo
			glVertex3f (gX+width, -gY, -lenght); // Lado Direito Baixo
			glVertex3f (gX+width, -gY-height, -lenght); // Lado Direito Cima
			glVertex3f (gX+width, -gY-height, 0.0);  // Lado Esquerdo Cima
			
			// front face
			a[0] = gX+width;
			a[1] = -gY;
			a[2] = -lenght;

			b[0] = gX;
			b[1] = -gY-height;
			b[2] = -lenght;

			cross(a,b,norm);
			normalize(norm);
			glNormal3f(norm[0], norm[1], norm[2]);
			glVertex3f (gX, -gY-height, -lenght);  // Lado Esquerdo Cima
			glVertex3f (gX+width, -gY-height, -lenght); // Lado Direito Cima
			glVertex3f (gX+width, -gY, -lenght); // Lado Direito Baixo
			glVertex3f (gX, -gY, -lenght);  // Lado Esquerdo Baixo 

			// up face
			a[0] = gX;
			a[1] = -gY;
			a[2] = -lenght;

			b[0] = gX+width;
			b[1] = -gY;
			b[2] = 0;

			cross(a,b,norm);
			normalize(norm);
			glNormal3f(norm[0], norm[1], norm[2]);
			glVertex3f (gX+width, -gY, 0.0); // Lado Direito Baixo
			glVertex3f (gX, -gY, 0.0);  // Lado Esquerdo Baixo 
			glVertex3f (gX, -gY, -lenght);  // Lado Esquerdo Cima
			glVertex3f (gX+width, -gY, -lenght); // Lado Direito Cima

			// back face
			a[0] = gX;
			a[1] = -gY-height;
			a[2] = 0.0;

			b[0] = gX+width;
			b[1] = -gY;
			b[2] = 0.0;

			cross(a,b,norm);
			normalize(norm);
			glNormal3f(norm[0], norm[1], norm[2]);
			glVertex3f (gX, -gY, 0.0);  // Lado Esquerdo Baixo 
			glVertex3f (gX+width, -gY, 0.0); // Lado Direito Baixo
			glVertex3f (gX+width, -gY-height, 0.0); // Lado Direito Cima
			glVertex3f (gX, -gY-height, 0.0);  // Lado Esquerdo Cima
		glEnd();
	}
}

void Platform::DrawPlatform(GLfloat x, GLfloat y, GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    gX = x;
    gY = y;
    DrawRect(height, width, R, G, B);
}
