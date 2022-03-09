#ifndef PLAYER_H
#define	PLAYER_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "fireball.h"
#include "platform.h"
#include <string>
#include <vector>
#include "enemy.h"
#include <math.h>
using namespace std;

typedef struct POS{
    float x;
    float y;
    float z;
} pos;

typedef struct NORM{
    float x;
    float y;
    float z;
} norm;

typedef struct TEX{
    float u;
    float v;
} tex;

typedef struct VERTICE_STRIP{
    pos vPos;
    norm vNorm;
    tex vTex;
} verticeStrip;

class Player {
    GLint hDirection;
    GLfloat gX, gY, gZ, radius, gTheta0, gTheta1, gTheta01, gTheta11, gThetaWheel, armAngle, time, posArmX, posArmY, posArmZ; 
    GLfloat direction, inc0, inc1, inc01, inc11, gThetaJump0, gThetaJump1, gThetaJump01, radiusWheel, ground;
    GLfloat gThetaJump11, percentual, legHeight, memberWidth, paddleHeight, paddleWidth, baseHeight, baseWidth;
    Fireball* t;
    bool on_move, on_jump, aiming, fireballOn, defeat, colliderState;

    public:
        vector<pos> vertsPos; //Lista de posicao dos vertices (original do OBJ)   
        vector<norm> vertsNorm; //Lista de normais dos vertices (original do OBJ)   
        vector<tex> vertsTex; //Lista de coor de textura dos vertices (original do OBJ)   
        vector<verticeStrip> vertsS;//Lista ordenada de vertices das faces para o desenho do modelo via trangle strip
        GLuint texID;
        char* texPixels;
        int texWidth;
        int texHeight;
        //Variaveis das meshes dos movimentos
        vector<vector<Player>> vecMeshes;
        int currentMovID;
        int currentFrame;
        Player(){
            gX = -1; 
            gY = -50;
            gZ = 0;
            posArmX = 0;
            posArmY = 0;
            posArmZ = 0; 
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
            texID = -1;
            currentMovID = -1;
            currentFrame = -1;
            vecMeshes.clear();
        };
        ~Player(){
        };
        
        int loadMeshAnim(string path, int qtd);//Path é o caminho para o arquivo .obj e deve conter o nome do arquivo.obj
        bool loadTexture(string path);//Path é o caminho para o arquivo .png e deve conter o nome do arquivo.png
        void draw(int movID, int frameId);
        void drawInit(int movID);
        void drawCurrent();
        bool drawNext();
        void Move(GLfloat delta, GLdouble deltaTime);
        void Rotate(GLfloat angle, GLdouble deltaTime);
        void Jump(GLfloat dx, GLfloat dy, GLdouble deltaTime);
        void OnMove(bool is_on_move);
        void OnJump(bool is_on_jump);
        void Aiming(bool is_aiming);
        void RotatePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat angle, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut);
        bool GravityEffect(GLfloat dx, GLdouble deltaTime);
        GLint DetectGround(Platform *platforms, Enemy *enemies, int lenPlatforms, int lenEnemies, GLfloat pos);
        GLint DetectCollision(Platform *element, Enemy *enemies, int lenPlatforms, int lenEnemies, GLfloat pos);
        GLint DetectBackground(Platform *element, int len, GLfloat pos);
        GLint DetectDistance(Enemy *enemies, int lenEnemies);
        bool Atingido(Fireball *fireball, GLfloat pos);
        void Atira();
        void DeleteFireball();
        bool loadMesh(string path);//Path é o caminho para o arquivo .obj e deve conter o nome do arquivo.obj
        void draw();
        GLfloat GetX(){
            return gX;
        };
        GLfloat GetY(){
            return gY;
        };
        GLfloat GetZ(){
            return gZ;
        };
        GLfloat GetRadius(){
            return radius;
        };
        Fireball* GetFireball(){
            return t;
        };
        // void SetFireball(*fireball){
        //     t = fireball[0];
        // };
        GLfloat GetLegHeight(){
            return legHeight;
        };
        GLfloat GetBaseWidth(){
            return baseWidth;
        };
        GLfloat GetBaseHeight(){
            return baseHeight;
        };
        GLfloat GetArmAngle(){
            return armAngle;
        }
        GLfloat GetPlayerCamHeight(){
            return legHeight*2 + baseHeight;
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
        void SetZ(GLfloat z){
            gZ = z;
        };
        void SetPosArm(GLfloat x, GLfloat y, GLfloat z){
            posArmX = x;
            posArmY = y;
            posArmZ = z;
        };
        GLfloat GetPosArmX(){
            return posArmX;
        };
        GLfloat GetPosArmY(){
            return posArmY;
        };
        GLfloat GetPosArmZ(){
            return posArmZ;
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

