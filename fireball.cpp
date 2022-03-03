#include "fireball.h"
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

void Fireball::DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);
    glPointSize(3.0);
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i+=1) 
            glVertex3f(radius * cos(i), radius * sin(i), 0.0);
    glEnd();
}
//Funcao auxiliar para normalizar um vetor a/|a|
void normFire(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

//Funcao auxiliar para fazer o produto vetorial entre dois vetores a x b = out
void crossFire(float a[3], float b[3], float out[3])
{
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}
double ToRad(int degree)
{
	return (degree * M_PI)/180;
}
//Aplica a transformacao que coloca o sistema de coordendas local em uma posicao 
//do mundo definida por dois pontos a e b e um vetor up. O ponto b sera a origem,
//o vetor BA = A - B sera determinara o novo y, o vetor up definira a orientacao
//do novo z em torno de BA.
//A matriz de transformacao no opengl eh armazanada transposta: 
//m[4][4] = {Xx, Xy, Xz, 0.0, Yx, Yy, Yz, 0.0, Zx, Zy, Zz, 0.0, Tx, Ty, Tz, 1.0}
void ChangeCoord(
        GLdouble ax, GLdouble ay, GLdouble az, 
        GLdouble bx, GLdouble by, GLdouble bz, 
        GLdouble upx, GLdouble upy, GLdouble upz)
{

    float x[3], y[3], z[3];
	
	x[0] = ax - bx;
	x[1] = ay - by;
	x[2] = az - bz;
	normFire(x);	

	y[0] = upx - ax;
	y[1] = upy - ay;
	y[2] = upz - az;	
	crossFire(x, y, z);
	normFire(z);


	crossFire(z, x, y);

	GLfloat m[4][4] = { x[0],x[1],x[2],0,
                        y[0],y[1],y[2],0,
                        z[0],z[1],z[2],0,
						(GLfloat)bx,(GLfloat)by,(GLfloat)bz,1};
    
    glMultMatrixf(&m[0][0]);
}
void Fireball::DrawFireball(GLfloat p0x, GLfloat p0y, GLfloat p0z,
                            GLfloat p1x, GLfloat p1y, GLfloat p1z,
                            GLfloat p2x, GLfloat p2y, GLfloat p2z)
{
    // GetPos(x,y,z);
    // glPushMatrix();
    glTranslatef(p1x, p1y, p1z);
    // ChangeCoord(p0x, p0y, p0z,
    //             p1x, p1y, p1z,
    //             p2x, p2y, p2z);
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
	GLfloat materialColor[] = { 1.0, 0.0, 0.0, 1.0};
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glColor3f(255, 0, 0);
    glutSolidSphere(radiusFireball, 50,50);
    // glPopMatrix();
}

bool Fireball::DetectBackground(Platform *platforms, int len, GLfloat gXpos, GLfloat pos)
{
    GLfloat y = - (platforms[0].GetY() - pos);
    if( sqrt(pow((gXpos - gX[0]) - platforms[0].GetX() - (platforms[0].GetW()/2), 2)) > platforms[0].GetW()/2 ||
        sqrt(pow((y - platforms[0].GetH()/2) - (gY[0]), 2)) > platforms[0].GetH()/2)   
    {
        return true;
    }
    return false;
}

bool Fireball::DetectBulletCollision(Platform *platforms, int len, GLfloat gXpos, GLfloat posY)
{
        
    for (int i = 0; i < len; i++)
    {   
        GLfloat y = - (platforms[i].GetY() - posY);
        if( sqrt(pow((gXpos - gX[0]) - platforms[i].GetX() - (platforms[i].GetW()/2), 2)) <= platforms[i].GetW()/2 &&
            sqrt(pow((y - platforms[i].GetH()/2) - (gY[0]), 2)) <= platforms[i].GetH()/2) 
        {
            return true;
        }
    }
    return false;
}

void Fireball::Move(GLfloat deltaTime)
{
    if (gDirectionAng < -0.1)
    {
        gX[0] = gX[0] - abs(sin((abs(gDirectionAng)*M_PI)/180) * 0.1 * deltaTime);
        gY[0] = gY[0] - (- cos((gDirectionAng*M_PI)/180) * 0.1 * deltaTime);
    }
    if (gDirectionAng > 0.1)
    {
        gX[0] = gX[0] + abs(sin((abs(gDirectionAng)*M_PI)/180) * 0.1 * deltaTime);
        gY[0] = gY[0] - (- cos((abs(gDirectionAng)*M_PI)/180) * 0.1 * deltaTime);
    }
}

