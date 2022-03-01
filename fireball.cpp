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

void Fireball::DrawFireball(GLfloat x, GLfloat y)
{
    GetPos(x,y);
    // glPushMatrix();
    glTranslatef(-x, y, 0);
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
    if( sqrt(pow((gXpos - gX) - platforms[0].GetX() - (platforms[0].GetW()/2), 2)) > platforms[0].GetW()/2 ||
        sqrt(pow((y - platforms[0].GetH()/2) - (gY), 2)) > platforms[0].GetH()/2)   
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
        if( sqrt(pow((gXpos - gX) - platforms[i].GetX() - (platforms[i].GetW()/2), 2)) <= platforms[i].GetW()/2 &&
            sqrt(pow((y - platforms[i].GetH()/2) - (gY), 2)) <= platforms[i].GetH()/2) 
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
        gX = gX - abs(sin((abs(gDirectionAng)*M_PI)/180) * gVel * deltaTime);
        gY = gY - (- cos((gDirectionAng*M_PI)/180) * gVel * deltaTime);
    }
    if (gDirectionAng > 0.1)
    {
        gX = gX + abs(sin((abs(gDirectionAng)*M_PI)/180) * gVel * deltaTime);
        gY = gY - (- cos((abs(gDirectionAng)*M_PI)/180) * gVel * deltaTime);
    }
}

