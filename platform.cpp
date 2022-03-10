#include "platform.h"
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>


void Platform::DrawRect(GLint height, GLint width, GLint length, GLfloat R, GLfloat G, GLfloat B)
{
	/* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
	GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
	GLfloat materialColor[] = { 1.0, 1.0, 0.0, 1.0};
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	/* Desenhar um polígono branco (retângulo) */
	if(B == 255)
	{         
		glBegin(GL_QUADS);
 			glNormal3f(0,1,0);
			glVertex3f (gX+width, -gY-height, gZ); // Lado Direito Baixo
			glVertex3f (gX, -gY-height, gZ);  // Lado Esquerdo Baixo 
			glVertex3f (gX, -gY-height, gZ-length);  // Lado Esquerdo Cima
			glVertex3f (gX+width, -gY-height, gZ-length); // Lado Direito Cima
		glEnd();
	}	
	else if (B == 0)
	{
		// GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
		// GLfloat materialColor[] = { 1.0, 0.0, 1.0, 1.0};
		// GLfloat mat_specular[] = { 1.0, 0.0, 1.0, 1.0};
		// GLfloat mat_shininess[] = { 128 };
		// glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
		// glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
		// glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
		// glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		// glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		// // glTranslatef(0,height/2,0);
		// glScalef(width, height, length);
		// glutSolidCube(1);


		// glColor3f(1,0,0);
		glPushMatrix();
		glTranslatef(gX+(width/2),-gY-(height/2),gZ - length/2);
		// glRotatef(180, 1, 0, 0);
		glScalef(width, height, length);
		glutSolidCube(1);
		glPopMatrix(); 
		// GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
		// GLfloat materialColor[] = { 1.0, 0.0, 1.0, 1.0};
		// GLfloat mat_specular[] = { 1.0, 0.0, 1.0, 1.0};
		// GLfloat mat_shininess[] = { 128 };		
		// GLfloat norm[3], a[3], b[3];

		// glBegin(GL_QUADS);
		// 	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
		// 	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
		// 	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
		// 	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		// 	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			
		// 	// down face
		// 	a[0] = gX+width;
		// 	a[1] = -gY-height;
		// 	a[2] = gZ-length;

		// 	b[0] = gX;
		// 	b[1] = -gY-height;
		// 	b[2] = gZ;

		// 	cross(a,b,norm);
		// 	normalize(norm);
		// 	glNormal3f(norm[0], norm[1], norm[2]);
		// 	glVertex3f (gX, -gY-height, gZ);  // Lado Esquerdo Baixo 
		// 	glVertex3f (gX+width, -gY-height, gZ); // Lado Direito Baixo
		// 	glVertex3f (gX+width, -gY-height, gZ-length); // Lado Direito Cima
		// 	glVertex3f (gX, -gY-height, gZ-length);  // Lado Esquerdo Cima
			
		// 	//left face
		// 	a[0] = gX;
		// 	a[1] = -gY;
		// 	a[2] = gZ-length;

		// 	b[0] = gX;
		// 	b[1] = -gY-height;
		// 	b[2] = gZ;

		// 	cross(a,b,norm);
		// 	normalize(norm);
		// 	glNormal3f(norm[0], norm[1], norm[2]);
		// 	glVertex3f (gX, -gY, gZ-length); // Lado Direito Baixo
		// 	glVertex3f (gX, -gY, gZ);  // Lado Esquerdo Baixo
		// 	glVertex3f (gX, -gY-height, gZ);  // Lado Esquerdo Cima
		// 	glVertex3f (gX, -gY-height, gZ-length); // Lado Direito Cima
			
		// 	//right face
		// 	a[0] = gX+width;
		// 	a[1] = -gY-height;
		// 	a[2] = gZ;

		// 	b[0] = gX+width;
		// 	b[1] = -gY;
		// 	b[2] = gZ-length;

		// 	cross(a,b,norm);
		// 	normalize(norm);
		// 	glNormal3f(norm[0], norm[1], norm[2]);
		// 	glVertex3f (gX+width, -gY, gZ);  // Lado Esquerdo Baixo
		// 	glVertex3f (gX+width, -gY, gZ-length); // Lado Direito Baixo
		// 	glVertex3f (gX+width, -gY-height, gZ-length); // Lado Direito Cima
		// 	glVertex3f (gX+width, -gY-height, gZ);  // Lado Esquerdo Cima
			
		// 	// front face
		// 	a[0] = gX+width;
		// 	a[1] = -gY;
		// 	a[2] = gZ-length;

		// 	b[0] = gX;
		// 	b[1] = -gY-height;
		// 	b[2] = gZ-length;

		// 	cross(a,b,norm);
		// 	normalize(norm);
		// 	glNormal3f(norm[0], norm[1], norm[2]);
		// 	glVertex3f (gX, -gY-height, gZ-length);  // Lado Esquerdo Cima
		// 	glVertex3f (gX+width, -gY-height, gZ-length); // Lado Direito Cima
		// 	glVertex3f (gX+width, -gY, gZ-length); // Lado Direito Baixo
		// 	glVertex3f (gX, -gY, gZ-length);  // Lado Esquerdo Baixo 

		// 	// up face
		// 	a[0] = gX;
		// 	a[1] = -gY;
		// 	a[2] = gZ-length;

		// 	b[0] = gX+width;
		// 	b[1] = -gY;
		// 	b[2] = 0;

		// 	cross(a,b,norm);
		// 	normalize(norm);
		// 	glNormal3f(norm[0], norm[1], norm[2]);
		// 	glVertex3f (gX+width, -gY, gZ); // Lado Direito Baixo
		// 	glVertex3f (gX, -gY, gZ);  // Lado Esquerdo Baixo 
		// 	glVertex3f (gX, -gY, gZ-length);  // Lado Esquerdo Cima
		// 	glVertex3f (gX+width, -gY, gZ-length); // Lado Direito Cima

		// 	// back face
		// 	a[0] = gX;
		// 	a[1] = -gY-height;
		// 	a[2] = gZ;

		// 	b[0] = gX+width;
		// 	b[1] = -gY;
		// 	b[2] = gZ;

		// 	cross(a,b,norm);
		// 	normalize(norm);
		// 	glNormal3f(norm[0], norm[1], norm[2]);
		// 	glVertex3f (gX, -gY, gZ);  // Lado Esquerdo Baixo 
		// 	glVertex3f (gX+width, -gY, gZ); // Lado Direito Baixo
		// 	glVertex3f (gX+width, -gY-height, gZ); // Lado Direito Cima
		// 	glVertex3f (gX, -gY-height, gZ);  // Lado Esquerdo Cima
		// glEnd();
	}
}

void Platform::DrawPlatform(GLfloat x, GLfloat y, GLfloat z, GLint heigth, GLint width, GLint length, GLfloat R, GLfloat G, GLfloat B)
{
    gX = x;
    gY = y;
    DrawRect(height, width, length, R, G, B);
}
