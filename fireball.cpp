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

double ToRad(int degree)
{
	return (degree * M_PI)/180;
}

void Fireball::DrawFireball(GLfloat p0x, GLfloat p0y, GLfloat p0z,
                            GLfloat p1x, GLfloat p1y, GLfloat p1z,
                            GLfloat p2x, GLfloat p2y, GLfloat p2z,
                            GLfloat directionAng, GLfloat angleYZ, 
                            bool enemy)
{
    
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
	GLfloat materialColor[] = { 1.0, 0.0, 0.0, 1.0};
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
	// GLfloat mat_shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	// glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    // glColor3f(255, 0, 0);
    // glPushMatrix();
        GLfloat mT[4][4] = { 1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,playerPosZ,1};
        glMultMatrixf(&mT[0][0]);
        GLfloat m[4][4] = { (GLfloat)cos(ToRad(directionAng)),0,(GLfloat)sin(ToRad(directionAng)),0,
                            0,1,0,0,
                            (GLfloat)-sin(ToRad(directionAng)),0,(GLfloat)cos(ToRad(directionAng)),0,
                            0,0,0,1};
        glMultMatrixf(&m[0][0]);
        
        glRotatef(90, 0, 1, 0);
        if(enemy)
        {
            glTranslatef(p1x, playerPosY+p1y, p1z);
            glRotatef(-90, 0, 1, 0);
            glRotatef(angleYZ+90, 0, 0, 1);
            glTranslatef(p0x, p0y, p0z);
            glRotatef(90, 0, 1, 0);
            glRotatef(angleYZ+90, 1, 0, 0);
            glRotatef(directionAng, 0, 1, 0);           
        }
        else
        {
            glTranslatef(p1x, playerPosY+p1y, p1z);
            glRotatef(-90, 0, 1, 0);
            glRotatef(angleYZ+90, 0, 0, 1);
            glTranslatef(p0x, p0y, p0z);
            glRotatef(90, 0, 1, 0);
            glRotatef(angleYZ+90, 1, 0, 0);
            glRotatef(directionAng, 0, 1, 0);
        }
           
            
        
        glutSolidSphere(radiusFireball, 50,50);
}

bool Fireball::DetectBackground(Platform *platforms, int len, GLfloat gXpos, GLfloat pos)
{
    GLfloat y = - (platforms[0].GetY() - pos);
    GLfloat fireballPosX = 0;
    if (cos(ToRad(gDirectionAng))/abs(cos(ToRad(gDirectionAng))) >= 0){
        fireballPosX = playerPosX + gY[2];
    }
    else{
        fireballPosX = playerPosX - gY[2];
    }
    if( sqrt(pow(fireballPosX - platforms[0].GetX() - (platforms[0].GetW()/2), 2)) > platforms[0].GetW()/2 ||
        sqrt(pow((y - platforms[0].GetH()/2) - (gY[1]), 2)) > platforms[0].GetH()/2 ||
        sqrt(pow((gZ[0] + playerPosZ) - (platforms[0].GetZ() - (platforms[0].GetL()/2)), 2)) > platforms[0].GetL()/2)   
    {
        // return true;
    }
    return false;
}

bool Fireball::DetectBulletCollision(Platform *platforms, int len, GLfloat gXpos, GLfloat posY)
{
        
    for (int i = 0; i < len; i++)
    {   
        GLfloat y = - (platforms[i].GetY() - posY);
        GLfloat fireballPosX = 0;
        if (cos(ToRad(gDirectionAng))/abs(cos(ToRad(gDirectionAng))) >= 0){
            fireballPosX = playerPosX + gX[2];
        }
        else{
            fireballPosX = playerPosX - gX[2];
        }
        if( sqrt(pow(fireballPosX - (platforms[i].GetX() + (platforms[i].GetW()/2)), 2)) <= platforms[i].GetW()/2 &&
            sqrt(pow((y - platforms[i].GetH()/2) - (gY[2] + playerPosY), 2)) <= platforms[i].GetH()/2)
        {
            return true;
        }
    }
    return false;
}

void Fireball::Move(GLfloat deltaTime)
{
    if(isEnemy)
    {    
        gY[0] = gY[0] - 0.1 * deltaTime;
        gX[2] = gX[2] + (cos(ToRad(gAngleYZ)) * 0.1 * deltaTime);
        gY[2] = gY[2] + (sin(ToRad(gAngleYZ)) * 0.1 * deltaTime);
        gZ[2] = gZ[2] + (tan(ToRad(gDirectionAng)) * 0.1 * deltaTime);
    }
    else
    {   
        gX[0] = gX[0] + 0.1 * deltaTime;
        gX[2] = gX[2] + 0.1 * deltaTime;
        gY[2] = gY[2] + (cos(ToRad(gAngleYZ)) * 0.1 * deltaTime);
        gZ[2] = gZ[2] + (tan(ToRad(gDirectionAng)) * 0.1 * deltaTime);   
    }
}

