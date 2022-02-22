#ifndef PLATFORM_H
#define	PLATFORM_H
#include <GL/gl.h>
#include <GL/glu.h>
// #include "fireball.h"

class Platform {
    GLfloat gX; 
    GLfloat gY;
    GLfloat gZ;
    GLint height;
    GLint width;
    GLint length;
    GLint red;
    GLint green;
    GLint blue;
private:
    void DrawRect(GLint height, GLint width, GLint length, GLfloat R, GLfloat G, GLfloat B);
    void DrawPlatform(GLfloat x, GLfloat y, GLfloat z, GLint height, GLint width, GLint length, GLfloat R, GLfloat G, GLfloat B);
public:
    Platform(GLfloat x, GLfloat y, GLfloat z, GLint h, GLint w, GLint l, GLfloat R, GLfloat G, GLfloat B){
        gX = x; 
        gY = y;
        gZ = z;
        height = h;
        width = w;
        length = l;
        red = R;
        green = G;
        blue = B;
    };
    void Draw(){ 
        DrawPlatform(gX, gY, gZ, height, width, length, red, green, blue);
    };
    GLfloat GetX(){
        return gX;
    };
    GLfloat GetY(){
        return gY;
    };
    GLfloat GetZ(){
        return gZ;
    };
    GLfloat GetH(){
        return height;
    };
    GLfloat GetW(){
        return width;
    };
    GLfloat GetL(){
        return length;
    };
    // void Recria(GLfloat x, GLfloat y);
    // bool Atingido(Fireball *fireball);
};

#endif	/* PLATFORM_H */

