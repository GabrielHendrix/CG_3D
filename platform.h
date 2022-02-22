#ifndef PLATFORM_H
#define	PLATFORM_H
#include <GL/gl.h>
#include <GL/glu.h>
// #include "fireball.h"

class Platform {
    GLfloat gX; 
    GLfloat gY;
    GLint height;
    GLint width;
    GLint red;
    GLint green;
    GLint blue;
private:
    void DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B);
    void DrawPlatform(GLfloat x, GLfloat y, GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B);
public:
    Platform(GLfloat x, GLfloat y, GLint h, GLint w, GLfloat R, GLfloat G, GLfloat B){
        gX = x; 
        gY = y;
        height = h;
        width = w;
        red = R;
        green = G;
        blue = B;
    };
    void Draw(){ 
        DrawPlatform(gX, gY, height, width, red, green, blue);
    };
    GLfloat GetX(){
        return gX;
    };
    GLfloat GetY(){
        return gY;
    };
    GLfloat GetH(){
        return height;
    };
    GLfloat GetW(){
        return width;
    };
    // void Recria(GLfloat x, GLfloat y);
    // bool Atingido(Fireball *fireball);
};

#endif	/* PLATFORM_H */

