// #ifndef FIREBALL_H
// #define FIREBALL_H
// #include <GL/gl.h>
// #include <GL/glu.h>
// #include "platform.h"
// #include <math.h>
// #include <stdio.h>

// #define radiusFireball 1
// class Fireball
// {
//     GLfloat gX, gY, gZ, gXmove, gDirectionAng, forearmX, forearmY, forearmZ, gAngleYZ, posForearmX, posForearmY, posForearmZ, gXInit, gYInit, gZTranslate,playerPosX, playerPosY, playerPosZ;

// private:
//     void DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B);

// public:
//     // Fireball(GLfloat x, GLfloat y, GLfloat z, GLfloat directionAng)
//     Fireball(GLfloat posForearmX, GLfloat posForearmY, GLfloat posForearmZ,
//              GLfloat posArmX, GLfloat posArmY, GLfloat posArmZ, 
//              GLfloat z0, GLfloat z1, GLfloat z2, 
//              GLfloat directionAng, GLfloat angleYZ,
//              GLfloat playerX, GLfloat playerY, GLfloat playerZ)
//     {
//         forearmX = posForearmX;
//         forearmY = posForearmY;
//         forearmZ = posForearmZ;
//         gX = posArmX;
//         gY = posArmY;
//         gZ = posArmZ;
//         gXmove = z0;
//         gXInit = posArmX;
//         gYInit = posArmY;
//         gZTranslate = posArmZ;
//         gDirectionAng = directionAng;
//         gAngleYZ = angleYZ;
//         playerPosX = playerX;
//         playerPosY = playerY;
//         playerPosZ = playerZ;
//     };
//     ~Fireball(){};
//     void Draw()
//     {
//         DrawFireball();
//     };
//     void Move(GLfloat deltaTime);
//     void DrawFireball();
//     // (GLfloat posForearmX, GLfloat posForearmY, GLfloat posForearmZ,
//     //                   GLfloat posArmX, GLfloat posArmY, GLfloat posArmZ,
//     //                   GLfloat directionAng, GLfloat angleYZ);
//     bool DetectBulletCollision(Platform *element, int len, GLfloat gXpos, GLfloat pos);
//     bool DetectBackground(Platform *element, int len, GLfloat gXpos, GLfloat pos);
//     void GetPos(GLfloat &xOut0, GLfloat &xOut1, GLfloat &xOut2, 
//                 GLfloat &yOut0, GLfloat &yOut1, GLfloat &yOut2, 
//                 GLfloat &zOut0, GLfloat &zOut1, GLfloat &zOut2)
//     {
//         xOut0 = forearmX;
//         xOut1 = forearmY;
//         xOut2 = forearmZ;
//         yOut0 = gX;
//         yOut1 = gY;
//         yOut2 = gZ;
//         zOut0 = gXInit;
//         zOut1 = gYInit;
//         zOut2 = gZTranslate;
//     };

//     // GLfloat GetX(){
//     //     return gX;
//     // };
//     // GLfloat GetY(){
//     //     return gY;
//     // };
//     // void SetX(GLfloat *x, int i){
//     //     gX[i] = x[0];
//     // };
//     // void SetY(GLfloat *y, int i){
//     //     gY[i] = y[0];
//     // };
//     // void SetZ(GLfloat *z, int i){
//     //     gZ[i] = z[0];
//     // };
//     GLfloat GetDeltaDist()
//     {
//         // glPushMatrix();
//         // glTranslatef(0,0,0);
//         // printf("Dist: %f\n", sqrt(pow((gY[0] - gXInit), 2) + pow((gY[1] - gYInit), 2)));
//         GLfloat result = sqrt(pow((gX - gXInit), 2) + pow((gY - gYInit), 2));
//         // glPopMatrix();
//         return result;   
//     };
// };

// #endif /* FIREBALL_H */

#ifndef FIREBALL_H
#define FIREBALL_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "platform.h"
#include <math.h>
#include <stdio.h>

#define radiusFireball 1
class Fireball
{
    GLfloat gX[3], gY[3], gZ[3], gDirectionAng, gAngleYZ, gXInit, gYInit, gZInit, playerPosX, playerPosY, playerPosZ, posCorrection;
    bool isEnemy;
private:
    void DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B);

public:
    // Fireball(GLfloat x, GLfloat y, GLfloat z, GLfloat directionAng)
    Fireball(GLfloat x0, GLfloat y0, GLfloat z0, 
             GLfloat x1, GLfloat y1, GLfloat z1, 
             GLfloat x2, GLfloat y2, GLfloat z2, 
             GLfloat directionAng, GLfloat angleYZ,
             GLfloat playerX, GLfloat playerY, GLfloat playerZ, bool enemy)
    {
        gX[0] = x0;
        gX[1] = x1;
        gX[2] = x2;
        gY[0] = y0;
        gY[1] = y1;
        gY[2] = y2;
        gZ[0] = z0;
        gZ[1] = z1;
        gZ[2] = z2;
        gXInit = x1;
        gYInit = y1;
        gZInit = z1;
        isEnemy = enemy;
        gDirectionAng = directionAng;
        gAngleYZ = angleYZ;
        playerPosX = playerX;   
        playerPosY = playerY;
        playerPosZ = playerZ;
    };
    ~Fireball(){};
    void Draw(GLfloat pos)
    {
        posCorrection = pos; 
        DrawFireball(gX[0], gY[0], gZ[0],  
                     gX[1], gY[1], gZ[1],
                     gX[2], gY[2], gZ[2],
                     gDirectionAng, gAngleYZ, isEnemy);
    };
    void Move(GLfloat deltaTime);
    void DrawFireball(GLfloat x0, GLfloat y0, GLfloat z0, 
                      GLfloat x1, GLfloat y1, GLfloat z1, 
                      GLfloat x2, GLfloat y2, GLfloat z2, 
                      GLfloat directionAng, GLfloat angleYZ, bool enemy);
    bool DetectBulletCollision(Platform *element, int len, GLfloat gXpos, GLfloat pos);
    bool DetectBackground(Platform *element, int len, GLfloat gXpos, GLfloat pos);
    void GetPos(GLfloat &xOut, GLfloat &yOut, GLfloat &zOut)
    {
        if (cos((gDirectionAng * M_PI)/180)/abs(cos((gDirectionAng * M_PI)/180)) >= 0){
            xOut = playerPosX + gX[2];
            zOut = gZ[2] + playerPosZ;
        }
        else{
            xOut = playerPosX - gX[2];
            zOut = playerPosZ - gZ[2];
        }
        // xOut = playerPosX + gX[2];

        yOut = gY[2] + playerPosY;        
    };

    // GLfloat GetX(){
    //     return gX;
    // };
    // GLfloat GetY(){
    //     return gY;
    // };
    // void SetX(GLfloat *x, int i){
    //     gX[i] = x[0];
    // };
    // void SetY(GLfloat *y, int i){
    //     gY[i] = y[0];
    // };
    // void SetZ(GLfloat *z, int i){
    //     gZ[i] = z[0];
    // };
    GLfloat GetDeltaDist()
    {
        // glPushMatrix();
        // glTranslatef(0,0,0);
        // printf("Dist: %f\n", sqrt(pow((gY[0] - gXInit), 2) + pow((gY[1] - gYInit), 2)));
        GLfloat result = sqrt(pow((gX[2] - gXInit), 2) + pow((gY[2] - gYInit), 2) + pow((gZ[2] - gZInit), 2));
        // glPopMatrix();
        return result;   
    };
};

#endif /* FIREBALL_H */
