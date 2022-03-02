#ifndef FIREBALL_H
#define	FIREBALL_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "platform.h"
#include <math.h>

#define radiusFireball 1
class Fireball {
    GLfloat gXInit, gYInit , gX, gY , gDirectionAng, gZ;
    private:
        void DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B);
        void DrawFireball(GLfloat x, GLfloat y, GLfloat z);
    public:
        Fireball(GLfloat x, GLfloat y, GLfloat z, GLfloat directionAng){
            gXInit = x; 
            gYInit = y; 
            gX = x; 
            gY = y;
            gDirectionAng = directionAng; 
            gZ = z;
            // radiusFireball = 1;
        };
        ~Fireball(){
        };
        void Draw(){ 
            DrawFireball(gX, gY, gZ);
        };
        void Move(GLfloat deltaTime);
        bool DetectBulletCollision(Platform *element, int len, GLfloat gXpos, GLfloat pos);
        bool DetectBackground(Platform *element, int len, GLfloat gXpos, GLfloat pos);
        void GetPos(GLfloat &xOut, GLfloat &yOut, GLfloat &zOut){
            xOut = gX;
            yOut = gY;
            zOut = gZ;
        };
        // GLfloat GetX(){
        //     return gX;
        // };
        // GLfloat GetY(){
        //     return gY;
        // };
        // GLfloat SetY(){
        //     return gY;
        // };
        GLfloat GetDeltaDist(){
            return sqrt(pow((gX - gXInit),2) + pow((gY - gYInit),2));
        };
};

#endif	/* FIREBALL_H */

