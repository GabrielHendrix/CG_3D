#include "fireball.h"
#include <math.h>
#include <stdio.h>


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
    glPushMatrix();
    glTranslatef(-x, y, 0);
    DrawCircle(radiusFireball, 255, 0, 0);
    glPopMatrix();
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

