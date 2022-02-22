#ifndef ENEMY_H
#define	ENEMY_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "fireball.h"
#include "platform.h"
#include <math.h>


class Enemy {

    GLint hDirection, detectedEnemyGnd;
    GLfloat gX, gY, radius, gTheta0, gTheta1, gTheta01, gTheta11, gThetaWheel, armAngle, time; 
    GLfloat direction, inc0, inc1, inc01, inc11, gThetaJump0, gThetaJump1, gThetaJump01, radiusWheel, ground;
    GLfloat gThetaJump11, percentual, legHeight, memberWidth, paddleHeight, paddleWidth, baseHeight, baseWidth;
    Fireball* t;
    bool on_move, gndIsPlat, on_jump, aiming, fireballOn, groundDefined, defeat;
private:
    void DrawRect(  GLint height, GLint width, 
                    GLfloat R, GLfloat G, GLfloat B);
    void DrawCircle(  GLint radius, GLfloat R, 
                    GLfloat G, GLfloat B);
    void DrawHead(GLfloat x, GLfloat y, GLfloat thetaWheel, 
                    GLfloat R, GLfloat G, GLfloat B);
    void DrawArm(   GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat R, GLfloat G, GLfloat B);
    void DrawLeg(   GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat R, GLfloat G, GLfloat B);
    void DrawBody( GLfloat x, GLfloat y, GLfloat thetaWheel, 
                    GLfloat theta1, GLfloat theta2, GLfloat theta3, GLfloat theta4);

public:
    Enemy(GLfloat x, GLfloat y, GLfloat percent, GLfloat rad)
    {
        gX = x; 
        gY = y; 
        t = NULL;
        fireballOn = false;
        gTheta0 = -135; 
        gTheta1 = 135; 
        gTheta01 = 0;
        gTheta11 = 0;
        gThetaJump0  = 180; 
        gThetaJump1  = 180; 
        gThetaJump01 = 0;
        gThetaJump11 = 0;
        gThetaWheel = 0; 
        inc0 = -1;
        inc1 = 1;
        inc01 = 0;
        inc11 = -1;
        hDirection = 1;
        detectedEnemyGnd = 0;
        direction = 0;
        on_move = false;
        on_jump = false;
        aiming = false;
        groundDefined = false;
        gndIsPlat = false;
        percentual = percent;
        legHeight    = 80;
        memberWidth  = 10;
        paddleHeight = 80;
        paddleWidth  = 10;
        baseHeight   = 50;
        baseWidth    = 20;
        radiusWheel  = 20;
        radius = rad; 
        ground = 0;
        armAngle = 0;
        GLfloat time = 0.0;
        defeat = false;
        legHeight    = ceil((radius * 2) * 0.4);
        memberWidth  = ceil((radius) * 0.3);
        paddleHeight = ceil((radius * 2) * 0.6);
        paddleWidth  = ceil((radius) * 0.1);
        baseHeight   = ceil((radius * 2) * 0.3);
        baseWidth    = ceil((radius * 2) * 0.3);
        radiusWheel  = ceil((radius * 2) * 0.2);

    };
    void Draw(){ 
        DrawBody(gX, gY, gThetaWheel, gTheta0, 
                gTheta1, gTheta01, gTheta11);
    };
    void MoveInX(GLfloat dx, GLdouble deltaTime);
    void Jump(GLfloat dx, GLfloat dy, GLdouble deltaTime);
    void OnMove(bool is_on_move);
    void OnJump(bool is_on_jump);
    void Aiming(bool is_aiming);
    void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut);
    bool GravityEffect(GLfloat dx, GLdouble deltaTime);
    GLint DetectGround(Platform *platforms, int len, GLfloat pos);
    bool DetectCollision(Platform *element, int len, GLfloat pos);
    bool DetectBackground(Platform *element, int len, GLfloat pos);
    bool Atingido(Fireball *fireball, GLfloat pos);
    void Atira();
    void DeleteFireball();
    GLfloat GetX(){
        return gX;
    };
    GLfloat GetY(){
        return gY;
    };
    Fireball* GetFireball(){
        return t;
    };
    GLfloat GetLegHeight(){
        return legHeight;
    };
    GLint GetHDirection(){
        return hDirection;
    };
    bool GetDefeatState(){
        return defeat;
    };
    void SetHDirection(GLfloat dir){
        hDirection*=dir;
    };
    void SetTime(GLfloat t){
        time = t;
    };
    void SetX(GLfloat x){
        gX = x;
    };
    void SetY(GLfloat y){
        gY = y;
    };
    void SetArmAngle(GLfloat angle){
        armAngle = angle;
    };
    void SetDefeatState(bool state){
        defeat = state;
    };
    void SetGround(GLfloat gnd, bool isPlat, GLint detectedGnd){
        ground = gnd;
        groundDefined = true;
        gndIsPlat = isPlat;
        detectedEnemyGnd = detectedGnd;
    };
    bool GetGroundState(){
        return groundDefined;
    };
    GLfloat GetRadius(){
        return radius;
    };
    void SetFireballState(bool state){
        fireballOn = state;
    };
    void SetPercentual(GLfloat percent){
        percentual = percent;
        legHeight    = ceil((radius * 2) * 0.4);
        memberWidth  = ceil((radius) * 0.3);
        paddleHeight = ceil((radius * 2) * 0.6);
        paddleWidth  = ceil((radius) * 0.1);
        baseHeight   = ceil((radius * 2) * 0.3);
        baseWidth    = ceil((radius * 2) * 0.3);
        radiusWheel  = ceil((radius * 2) * 0.2);
    };
};

#endif	/* ENEMY_H */

