#ifndef PLAYER_H
#define	PLAYER_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "fireball.h"
#include "platform.h"
#include "enemy.h"
#include <math.h>

class Player {
    GLint hDirection;
    GLfloat gX, gY, radius, gTheta0, gTheta1, gTheta01, gTheta11, gThetaWheel, armAngle, time; 
    GLfloat direction, inc0, inc1, inc01, inc11, gThetaJump0, gThetaJump1, gThetaJump01, radiusWheel, ground;
    GLfloat gThetaJump11, percentual, legHeight, memberWidth, paddleHeight, paddleWidth, baseHeight, baseWidth;
    Fireball* t;
    bool on_move, on_jump, aiming, fireballOn, defeat, colliderState;
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
    Player(){
        gX = -1; 
        gY = -50; 
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
        hDirection = 0;
        direction = 0;
        on_move = false;
        on_jump = false;
        aiming = false;
        defeat = false;
        colliderState = false;
        percentual = 1;
        ground = 0;
        armAngle = 0;
        time = 0.0;
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
    GLint DetectGround(Platform *platforms, Enemy *enemies, int lenPlatforms, int lenEnemies, GLfloat pos);
    GLint DetectCollision(Platform *element, Enemy *enemies, int lenPlatforms, int lenEnemies, GLfloat pos);
    GLint DetectBackground(Platform *element, int len, GLfloat pos);
    GLint DetectDistance(Enemy *enemies, int lenEnemies);
    bool Atingido(Fireball *fireball, GLfloat pos);
    void Atira();
    void DeleteFireball();
    GLfloat GetX(){
        return gX;
    };
    GLfloat GetY(){
        return gY;
    };
    GLfloat GetRadius(){
        return radius;
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
    void SetTime(GLfloat t){
        time = t;
    };
    void SetX(GLfloat x){
        gX = x;
    };
    void SetY(GLfloat y){
        gY = y;
    };
    void SetRadius(GLfloat r){
        radius = r;
    };
    void SetArmAngle(GLfloat angle){
        armAngle = angle;
    };
    void SetGround(GLfloat gnd){
        ground = gnd;
    };
    void SetFireballState(bool state){
        fireballOn = state;
    };
    bool GetDefeatState(){
        return defeat;
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

#endif	/* ROBO_H */

