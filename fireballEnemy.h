#ifndef FireballEnemy_H
#define	FireballEnemy_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "platform.h"
#include <math.h>

#define radiusFireballEnemy 1
class FireballEnemy {
    GLfloat gXInit, gYInit , gX, gY , gDirectionAng, gVel;
    // GLint radiusFireballEnemy = 1;
private:
    void DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawFireballEnemy(GLfloat x, GLfloat y);
public:
    FireballEnemy(GLfloat x, GLfloat y, GLfloat directionAng){
        gXInit = x; 
        gYInit = y; 
        gX = x; 
        gY = y; 
        gDirectionAng = directionAng; 
        gVel = 0.1;
        // radiusFireballEnemy = 1;
    };
    void Draw(){ 
        DrawFireballEnemy(gX, gY);
    };
    void Move(GLfloat deltaTime);
    bool DetectBulletCollision(Platform *element, int len, GLfloat gXpos, GLfloat pos);
    bool DetectBackground(Platform *element, int len, GLfloat gXpos, GLfloat pos);
    void GetPos(GLfloat &xOut, GLfloat &yOut){
        xOut = gX;
        yOut = gY;
    };
    GLfloat GetX(){
        return gX;
    };
    GLfloat GetY(){
        return gY;
    };
    GLfloat GetDeltaDist(){
        return sqrt(pow((gX - gXInit),2) + pow((gY - gYInit),2));
    };
};

#endif	/* FireballEnemy_H */

