#ifndef ENEMY_H
#define	ENEMY_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "fireball.h"
#include "platform.h"
#include <math.h>
#include <string>
#include <vector>
using namespace std;

typedef struct POS_ENEMY{
    float x;
    float y;
    float z;
} posEnemy;

typedef struct NORM_ENEMY{
    float x;
    float y;
    float z;
} normEnemy;

typedef struct TEX_ENEMY{
    float u;
    float v;
} texEnemy;

typedef struct VERTICE_STRIP_ENEMY{
    posEnemy vPos;
    normEnemy vNorm;
    texEnemy vTex;
} verticeStripEnemy;

class meshEnemy{
	
    public:
        vector<posEnemy> vertsPos; //Lista de posicao dos vertices (original do OBJ)   
        vector<normEnemy> vertsNorm; //Lista de normais dos vertices (original do OBJ)   
        vector<texEnemy> vertsTex; //Lista de coor de textura dos vertices (original do OBJ)   
        vector<verticeStripEnemy> vertsS;//Lista ordenada de vertices das faces para o desenho do modelo via trangle strip
        meshEnemy(){
        };
        bool loadMesh(string path);//Path é o caminho para o arquivo .obj e deve conter o nome do arquivo.obj
        void draw();
};

class Enemy {

    GLint hDirection, detectedEnemyGnd;
    GLfloat gX, gY,gZ, radius, gTheta0, gTheta1, gTheta01, gTheta11, gThetaWheel, armAngle, time, posArmX, posArmY, posArmZ; 
    GLfloat direction, inc, inc0, inc1, inc01, inc11, gThetaJump0, gThetaJump1, gThetaJump01, radiusWheel, ground;
    GLfloat gThetaJump11, percentual, legHeight, memberWidth, paddleHeight, paddleWidth, baseHeight, baseWidth;
    Fireball* t;
    bool on_move, gndIsPlat, on_jump, aiming, fireballOn, groundDefined, defeat;
    
    public:
        GLuint texID;
        char* texPixels;
        int texWidth;
        int texHeight;
        //Variaveis das meshes dos movimentos
        vector<vector<meshEnemy>> vecMeshes;
        int currentMovID;
        int currentFrame;
        Enemy(GLfloat x, GLfloat y, GLfloat z, GLfloat percent, GLfloat rad)
        {
            gX = x; 
            gY = y; 
            gZ = z;
            posArmX = 0;
            posArmY = 0;
            posArmZ = 0;
            inc = 1;
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
            texID = -1;
            currentMovID = -1;
            currentFrame = -1;
            vecMeshes.clear();

        };
        ~Enemy(){
            };
        // void Draw(){ 
        //     DrawBody(gX, gY, gThetaWheel, gTheta0, 
        //             gTheta1, gTheta01, gTheta11);
        // };
        int loadMeshAnim(string path, int qtd);//Path é o caminho para o arquivo .obj e deve conter o nome do arquivo.obj
        bool loadTexture(string path);//Path é o caminho para o arquivo .png e deve conter o nome do arquivo.png
        void draw(int movID, int frameId);
        void drawInit(int movID);
        void drawCurrent();
        bool drawNext();
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
        GLfloat GetZ(){
            return gZ;
        };
        Fireball* GetFireball(){
            return t;
        };
        GLfloat GetLegHeight(){
            return legHeight;
        };
        GLint GetInc(){
            return inc;
        };
        GLint GetHDirection(){
            return hDirection;
        };
        GLfloat GetArmAngle(){
            return armAngle;
        }
        bool GetDefeatState(){
            return defeat;
        };
        void SetInc(GLfloat dir){
            inc*=dir;
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
        void SetHDirection(GLfloat angle){
            direction = angle;
        };
        void SetArmAngle(GLfloat angle){
            armAngle = angle;
        };
        void SetDefeatState(bool state){
            defeat = state;
        };
        void SetPosArm(GLfloat x, GLfloat y, GLfloat z){
            posArmX = x;
            posArmY = y;
            posArmZ = z;
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
        GLfloat GetPosArmX(){
            return posArmX;
        };
        GLfloat GetPosArmY(){
            return posArmY;
        };
        GLfloat GetPosArmZ(){
            return posArmZ;
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

