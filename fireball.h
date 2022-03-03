#ifndef FIREBALL_H
#define FIREBALL_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "platform.h"
#include <math.h>

#define radiusFireball 1
class Fireball
{
    GLfloat gX[3], gY[3], gZ[3], gDirectionAng;

private:
    void DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B);

public:
    // Fireball(GLfloat x, GLfloat y, GLfloat z, GLfloat directionAng)
    Fireball(GLfloat x0, GLfloat x1, GLfloat x2, 
             GLfloat y0, GLfloat y1, GLfloat y2, 
             GLfloat z0, GLfloat z1, GLfloat z2, 
             GLfloat directionAng)
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
        gDirectionAng = directionAng;
    };
    ~Fireball(){};
    void Draw()
    {
        DrawFireball(gX[0], gX[1], gX[2], 
                     gY[0], gY[1], gY[2], 
                     gZ[0], gZ[1], gZ[2]);
    };
    void Move(GLfloat deltaTime);
    void DrawFireball(GLfloat x0, GLfloat x1, GLfloat x2, 
                      GLfloat y0, GLfloat y1, GLfloat y2, 
                      GLfloat z0, GLfloat z1, GLfloat z2);
    bool DetectBulletCollision(Platform *element, int len, GLfloat gXpos, GLfloat pos);
    bool DetectBackground(Platform *element, int len, GLfloat gXpos, GLfloat pos);
    void GetPos(GLfloat &xOut, GLfloat &yOut, GLfloat &zOut)
    {
        xOut = gX[0];
        yOut = gY[0];
        zOut = gZ[0];
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
        return sqrt(pow((gX[0] - gX[1]), 2) + pow((gY[0] - gY[1]), 2));
    };
};

#endif /* FIREBALL_H */
