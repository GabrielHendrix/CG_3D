// #include "fireball.h"
// #include <math.h>
// #include <stdio.h>
// #include <GL/gl.h>
// #include <GL/glu.h>
// #include <GL/glut.h>
// #include <stdlib.h>

// void Fireball::DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B)
// {
//     glColor3f (R, G, B);
//     glPointSize(3.0);
//     glBegin(GL_POLYGON);
//         for (int i = 0; i < 360; i+=1) 
//             glVertex3f(radius * cos(i), radius * sin(i), 0.0);
//     glEnd();
// }
// //Funcao auxiliar para normalizar um vetor a/|a|
// void normFire(float a[3])
// {
//     double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
//     a[0] /= norm;
//     a[1] /= norm;
//     a[2] /= norm;
// }

// //Funcao auxiliar para fazer o produto vetorial entre dois vetores a x b = out
// void crossFire(float a[3], float b[3], float out[3])
// {
//     out[0] = a[1]*b[2] - a[2]*b[1];
//     out[1] = a[2]*b[0] - a[0]*b[2];
//     out[2] = a[0]*b[1] - a[1]*b[0];
// }
// double ToRad(int degree)
// {
// 	return (degree * M_PI)/180;
// }


// void Fireball::DrawFireball()
// // (GLfloat posForearmX, GLfloat posForearmY, GLfloat posForearmZ,
// //                             GLfloat posArmX, GLfloat posArmY, GLfloat posArmZ,
//                             // GLfloat directionAng, GLfloat angleYZ)
// {
    
//     GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
// 	GLfloat materialColor[] = { 1.0, 0.0, 0.0, 1.0};
// 	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
// 	GLfloat mat_shininess[] = { 128 };
// 	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
// 	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
// 	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
// 	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
// 	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
//     glColor3f(255, 0, 0);

//     GLfloat mT[4][4] = { 1,0,0,0,
//                         0,1,0,0,
//                         0,0,1,0,
//                         0,0,playerPosZ,1};
//     glMultMatrixf(&mT[0][0]);
//     GLfloat m[4][4] = { (GLfloat)cos(ToRad(gDirectionAng)),0,(GLfloat)sin(ToRad(gDirectionAng)),0,
//                          0,1,0,0,
//                          (GLfloat)-sin(ToRad(gDirectionAng)),0,(GLfloat)cos(ToRad(gDirectionAng)),0,
//                          0,0,0,1};
//     glMultMatrixf(&m[0][0]);
    
//     glRotatef(90, 0, 1, 0);
//     glTranslatef(gX, playerPosY+gY, gZTranslate);
//     glRotatef(-90, 0, 1, 0);
//     glRotatef(gAngleYZ+90, 0, 0, 1);
//     glTranslatef(forearmX, forearmY, forearmZ);
//     glutSolidSphere(radiusFireball, 50,50);
// }

// bool Fireball::DetectBackground(Platform *platforms, int len, GLfloat gXpos, GLfloat pos)
// {
//     GLfloat y = - (platforms[0].GetY() - pos);
//     GLfloat fireballPosX = 0;
//     if (cos(ToRad(gDirectionAng))/abs(cos(ToRad(gDirectionAng))) >= 0){
//         fireballPosX = playerPosX + gX;
//     }
//     else{
//         fireballPosX = playerPosX - gX;
//     }
//     if( sqrt(pow(fireballPosX - platforms[0].GetX() - (platforms[0].GetW()/2), 2)) > platforms[0].GetW()/2 ||
//         sqrt(pow((y - platforms[0].GetH()/2) - (gY), 2)) > platforms[0].GetH()/2 ||
//         sqrt(pow((gZ + playerPosZ) - (platforms[0].GetZ() - (platforms[0].GetL()/2)), 2)) > platforms[0].GetL()/2)   
//     {
//         return true;
//     }
//     return false;
// }

// bool Fireball::DetectBulletCollision(Platform *platforms, int len, GLfloat gXpos, GLfloat posY)
// {
        
//     for (int i = 0; i < len; i++)
//     {   
//         GLfloat y = - (platforms[i].GetY() - posY);
//         GLfloat fireballPosX = 0;
//         if (cos(ToRad(gDirectionAng))/abs(cos(ToRad(gDirectionAng))) >= 0){
//             fireballPosX = playerPosX + gX;
//         }
//         else{
//             fireballPosX = playerPosX - gX;
//         }
//         if( sqrt(pow(fireballPosX - (platforms[i].GetX() + (platforms[i].GetW()/2)), 2)) <= platforms[i].GetW()/2 &&
//             sqrt(pow((y - platforms[i].GetH()/2) - (gY + playerPosY), 2)) <= platforms[i].GetH()/2)// &&
//             // sqrt(pow(gZ[0] - (platforms[i].GetZ() - (platforms[i].GetL()/2)), 2)) <= platforms[i].GetL() ) 
//         {
//             printf("ACERTOU!!!\n");
//             return true;
//         }
//     }
//     return false;
// }

// void Fireball::Move(GLfloat deltaTime)
// {
//     gZ = gZ + abs(sin(ToRad(gAngleYZ)) * 0.1 * deltaTime); 
//     gXmove = gXmove + (cos(ToRad(gDirectionAng)) * 0.1 * deltaTime);
//     gY = gY + (cos(ToRad(gAngleYZ)) * 0.1 * deltaTime);

//     // printf("Pos gX: %f\n", gY[2]+playerPosX);
//     // printf("Pos gY: %f\n", gY[1]+playerPosY);
//     // printf("Pos gZ: %f\n", gZ[0]+playerPosZ);

// }




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
	
	z[0] = bx - ax;
	z[1] = by - ay;
	z[2] = bz - az;
	normFire(z);	
	
    x[0] = bx - upx;
	x[1] = by - upy;
	x[2] = bz - upz;
    crossFire(x, z, y);
    normFire(y);
    
	crossFire(y, z, x);

	GLfloat m[4][4] = { x[0],x[1],x[2],0,
                        y[0],y[1],y[2],0,
                        z[0],z[1],z[2],0,
						(GLfloat)-bx,(GLfloat)by,(GLfloat)bz,1};
    
    glMultMatrixf(&m[0][0]);
}
void Fireball::DrawFireball(GLfloat p0x, GLfloat p0y, GLfloat p0z,
                            GLfloat p1x, GLfloat p1y, GLfloat p1z,
                            GLfloat p2x, GLfloat p2y, GLfloat p2z,
                            GLfloat directionAng, GLfloat angleYZ)
{
    
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
    glPushMatrix();
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
        glTranslatef(p1x, playerPosY+p1y, p1z);
        glRotatef(-90, 0, 1, 0);
        glRotatef(angleYZ+90, 0, 0, 1);
        glTranslatef(p0x, p0y, p0z);
        glRotatef(90, 0, 1, 0);
        glRotatef(angleYZ+90, 1, 0, 0);
        glRotatef(directionAng, 0, 1, 0);
        glutSolidSphere(radiusFireball, 50,50);
    glPopMatrix();

    // glPushMatrix();
    //     glTranslatef(p2x, p2y, p2z);
    //     glutSolidSphere(radiusFireball, 50,50);
    // glPopMatrix();
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
            sqrt(pow((y - platforms[i].GetH()/2) - (gY[2] + playerPosY), 2)) <= platforms[i].GetH()/2)// &&
            // sqrt(pow(gZ[0] - (platforms[i].GetZ() - (platforms[i].GetL()/2)), 2)) <= platforms[i].GetL() ) 
        {
            printf("ACERTOU!!!\n");
            return true;
        }
    }
    return false;
}

void Fireball::Move(GLfloat deltaTime)
{

    gX[0] = gX[0] + abs(sin(ToRad(gAngleYZ)) * 0.1 * deltaTime);
    gX[2] = gX[2] + abs(sin(ToRad(gAngleYZ)) * 0.1 * deltaTime);
    gY[2] = gY[2] + (cos(ToRad(gAngleYZ)) * 0.1 * deltaTime);
    gZ[2] = gZ[2] + (sin(ToRad(gDirectionAng)) * 0.1 * deltaTime);
}

