#include "player.h"
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "imageloader.h"

//Aponta para uma das Player de movimento e coloca na primeira frame
void Player::drawInit(int movID){
    currentMovID = movID;
    currentFrame = 0;
}

//Desenha a proxima frame do movimento atual e retorna se ja chegou no final
bool Player::drawNext(){
    this->draw(this->currentMovID, this->currentFrame);
    this->currentFrame++;
    
    return (this->currentFrame >= this->vecMeshes[this->currentMovID].size());
}


//Carrega as Player de um movimento do caminho path e assumindo qtd arquivos
int Player::loadMeshAnim(string path, int qtd){
    vector<mesh> vec;
    mesh m;
    this->vecMeshes.push_back(vec);
    int movID = this->vecMeshes.size()-1;
    char str[7];
    size_t index = path.find("#");
    for(int i = 1; i<=qtd; i++){
        snprintf (str, 7, "%06d", i);
        path.replace(index, 6, str);
        std::cout << path << std::endl; 
        this->vecMeshes[movID].push_back(m);
        this->vecMeshes[movID][this->vecMeshes[movID].size()-1].loadMesh(path);
    }
    return movID;
}

//Desenha uma mesh com a respectiva textura
void Player::draw(int movID, int frameId){
    glPushMatrix();
    glTranslatef(0, gY, gZ);
    GLfloat m[4][4] = { 3,0,0,0,
                        0,3,0,0,
                        0,0,3,0,
                        1,1,1,1};

    // glMultMatrixf(&m[0][0]);
    glScalef(3, 3, 3);
    glRotatef(90, 0, 1, 0);
    glRotatef(direction, 0, 1, 0);
    if (this->texID != -1){
        glEnable(GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D, this->texID);
    }
    this->vecMeshes[movID][frameId].draw();
    if (this->texID != -1){
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
}

//Le a textura
bool Player::loadTexture(string path){
    FILE* file= fopen(path.data(), "r");
    
    Image* image = loadBMP(path.c_str());
    this->texWidth = image->width;
    this->texHeight = image->height;

    glGenTextures( 1, &(this->texID) );
    glBindTexture( GL_TEXTURE_2D, this->texID );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                             0,                            //0 for now
                             GL_RGB,                       //Format OpenGL uses for image
                             this->texWidth, this->texHeight,  //Width and height
                             0,                            //The border of the image
                             GL_RGB, //GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                               //as unsigned numbers
                             image->pixels);               //The actual pixel data
    delete image;
}

//função para carregar o OBJ
bool mesh::loadMesh(string path){
    vector<int> vertIndex, uvIndex, normIndex;//indexadores para vertices, mapeamento de textura e normais
    vertsPos.clear();
    vertsNorm.clear();
    vertsTex.clear();
    vertsS.clear();
    int i =0;
    FILE* file= fopen(path.data(), "r");
    if(file== NULL){
        cout<< "falha ao carregar o arquivo"<< endl;
        return false;
    }
    else{ 
        while(1){
            char lineHeader[128];
            int res= fscanf(file, "%s", lineHeader);
            if(res==EOF){
                    break;
            }
            if(strcmp(lineHeader, "v") == 0){
                pos vert;
                fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
                vertsPos.push_back(vert);
            }
            else if(strcmp(lineHeader,"vt")==0){
                tex vert;
                fscanf(file, "%f %f\n", &vert.u, &vert.v);
                vertsTex.push_back(vert);
            }
            else if(strcmp(lineHeader, "vn")==0){
                norm vert;
                fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
                vertsNorm.push_back(vert);
            }
            else if(strcmp(lineHeader, "f")==0){
                string v1, v2, v3;
                unsigned int vertInd[3], uvInd[3], normInd[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertInd[0], &uvInd[0], &normInd[0], &vertInd[1], &uvInd[1], &normInd[1], &vertInd[2], &uvInd[2], &normInd[2]);
                if(matches !=9){
                        printf("QUantidade de valores lidos diferente doesperado. Falha ao ler uma linha de face");
                        return false;			
                }
                //Cria uma lista com os índices na ordem apropriada para o desenho das faces
                //Esta é a lista de índices de vértices
                vertIndex.push_back(vertInd[0]);
                vertIndex.push_back(vertInd[1]);
                vertIndex.push_back(vertInd[2]);
                //Esta é a lista de índices de mapeamento de textura
                uvIndex.push_back(uvInd[0]);
                uvIndex.push_back(uvInd[1]);
                uvIndex.push_back(uvInd[2]);
                // Esta é a lista de índices de normais
                normIndex.push_back(normInd[0]);
                normIndex.push_back(normInd[1]);
                normIndex.push_back(normInd[2]);
            }
        }
        for(unsigned int i=0; i<vertIndex.size(); i++){
            verticeStrip vert;
            vert.vPos.x = vertsPos[vertIndex[i]-1].x;
            vert.vPos.y = vertsPos[vertIndex[i]-1].y;
            vert.vPos.z = vertsPos[vertIndex[i]-1].z;
            vert.vTex.u = vertsTex[uvIndex[i]-1].u;
            vert.vTex.v = vertsTex[uvIndex[i]-1].v;
            vert.vNorm.x = vertsNorm[normIndex[i]-1].x;
            vert.vNorm.y = vertsNorm[normIndex[i]-1].y;
            vert.vNorm.z = vertsNorm[normIndex[i]-1].z;
            vertsS.push_back(vert);
        }
    }
    return true;
}

//desenha a malha
void mesh::draw(){
    int cont=0;
    GLfloat materialEmission[] = { 0.10, 0.10, 0.10, 1};
    GLfloat materialColorA[] = { 0.1, 0.1, 0.1, 0.1};
    GLfloat materialColorD[] = { .90, .90, .90, 1};
    glColor3f(1,1,1);

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);

    for(unsigned int i=0; i<vertsS.size(); i=i+3){
        glBegin(GL_TRIANGLE_STRIP);
            for(int j=i; j<i+3; j++){
                glTexCoord2f (vertsS[j].vTex.u, vertsS[j].vTex.v);
                glNormal3f(vertsS[j].vNorm.x,vertsS[j].vNorm.y,vertsS[j].vNorm.z);
                glVertex3f(vertsS[j].vPos.x,vertsS[j].vPos.y,vertsS[j].vPos.z);
            }
        glEnd();
    }
}

void normalizePlayer(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}
void crossPlayer(GLfloat a[3], GLfloat b[3], GLfloat out[3])
{
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}
void Player::DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
	/* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
	glColor3f (R, G, B);
	/* Desenhar um polígono branco (retângulo) */
    GLint length = 2;
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
            GLfloat materialColor[] = { 1.0, 0.0, 1.0, 1.0};
            GLfloat mat_specular[] = { 1.0, 0.0, 1.0, 1.0};
            GLfloat mat_shininess[] = { 128 };
            GLfloat norm[3], a[3], b[3];

    glBegin(GL_QUADS);
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        
        // // down face
        // a[0] = -width/2;
        // a[1] = 0;
        // a[2] = length;

        // b[0] = width/2;
        // b[1] = 0;
        // b[2] = 0.0;

        // crossPlayer(a,b,norm);
        // normalizePlayer(norm);
        // glNormal3f(norm[0], norm[1], norm[2]);
        // glVertex3f (gX, -gY-height, 0.0);  // Lado Esquerdo Baixo 
        // glVertex3f (gX+width, -gY-height, 0.0); // Lado Direito Baixo
        // glVertex3f (gX+width, -gY-height, -length); // Lado Direito Cima
        // glVertex3f (gX, -gY-height, -length);  // Lado Esquerdo Cima

        // glVertex3f (-width/2, 0, 0.0);  // Lado Esquerdo Baixo 
        // glVertex3f (width/2, 0, 0.0); // Lado Direito Baixo        
        // glVertex3f (-width/2, 0, -length);  // Lado Esquerdo Cima
        // glVertex3f (width/2, 0, -length); // Lado Direito Cima
        //right face
        a[0] = -width/2;
        a[1] = height;
        a[2] = -length;

        b[0] = -width/2;
        b[1] = 0;
        b[2] = 0;

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
        glNormal3f(norm[0], norm[1], norm[2]);
        glVertex3f (-width/2, 0, 0.0);  // Lado Esquerdo Cima
        glVertex3f (-width/2, 0, -length); // Lado Direito Cima
        glVertex3f (-width/2, height, -length); // Lado Direito Baixo
        glVertex3f (-width/2, height, 0.0);  // Lado Esquerdo Baixo
        
        //left face
        a[0] = width/2;
        a[1] = 0;
        a[2] = 0;

        b[0] = width/2;
        b[1] = height;
        b[2] = -length;

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
        glNormal3f(norm[0], norm[1], norm[2]);

        glVertex3f (width/2, 0, 0.0);  // Lado Esquerdo Cima
        glVertex3f (width/2, 0, -length); // Lado Direito Cima
        glVertex3f (width/2, height, -length); // Lado Direito Baixo
        glVertex3f (width/2, height, 0.0);  // Lado Esquerdo Baixo
        
        // back face
        a[0] = -width/2;
        a[1] = height;
        a[2] = -length;

        b[0] = width/2;
        b[1] = 0;
        b[2] = -length;

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
        glNormal3f(norm[0], norm[1], norm[2]);
        glVertex3f (-width/2, 0, -length);  // Lado Esquerdo Cima
        glVertex3f (width/2, 0, -length); // Lado Direito Cima
        glVertex3f (width/2, height, -length); // Lado Direito Baixo
        glVertex3f (-width/2, height, -length);  // Lado Esquerdo Baixo 


    //     // up face
    //     a[0] = -width/2;
    //     a[1] = height;
    //     a[2] = -length;

    //     b[0] = width/2;
    //     b[1] = height;
    //     b[2] = 0;

    //     crossPlayer(a,b,norm);
    //     normalizePlayer(norm);
    //     glNormal3f(norm[0], norm[1], norm[2]);
    //     glVertex3f (width/2, height, 0.0); // Lado Direito Baixo        
    //     glVertex3f (-width/2, height, 0.0);  // Lado Esquerdo Baixo
    //     glVertex3f (-width/2, height, -length);  // Lado Esquerdo Cima
    //     glVertex3f (width/2, height, -length); // Lado Direito Cima
         
    //     // front face
    //     a[0] = -width/2;
    //     a[1] = 0;
    //     a[2] = 0.0;

    //     b[0] =width/2;
    //     b[1] = height;
    //     b[2] = 0.0;

    //     crossPlayer(a,b,norm);
    //     normalizePlayer(norm);
    //     glNormal3f(norm[0], norm[1], norm[2]);
    //     glVertex3f (-width/2, 0, 0.0);  // Lado Esquerdo Cima
	// 	glVertex3f (width/2, 0, 0.0); // Lado Direito Cima
	// 	glVertex3f (width/2, height, 0.0); // Lado Direito Baixo
	// 	glVertex3f (-width/2, height, 0.0);  // Lado Esquerdo Baixo
    glEnd();
}

void Player::DrawHead(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B)
{
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(thetaWheel, 0, 0, 1);
    DrawCircle(radiusWheel, R, G, B);
    glPopMatrix();
}

void Player::DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    // glColor3f (R, G, B);
    glPointSize(1.0);
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i+=1) 
            for (int j = 0; j < 200; j++)
                glVertex3f(radius * cos(i), radius * sin(i), (float)(-j/75));
    glEnd();
}

void Player::DrawArm(GLfloat x, GLfloat y, GLfloat theta, GLfloat theta2, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(theta, 0, 0, 1);   
    DrawRect(paddleHeight/2 - paddleHeight/10, memberWidth, R, G, B);
    glTranslatef(0, paddleHeight/2 - paddleHeight/10, 0);
    glRotatef(theta2, 0, 0, 1);   
    DrawRect(paddleHeight/2 - paddleHeight/10, memberWidth, R, G, B);
    glPopMatrix();
    
}

void Player::DrawLeg(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(theta1, 0, 0, 1);   
    DrawRect(legHeight/2, memberWidth, R, G, B);
    glTranslatef(0, legHeight/2, 0);
    glRotatef(theta2, 0, 0, 1);   
    DrawRect(legHeight/2, memberWidth, R, G, B);
    glPopMatrix();
    
}

void Player::DrawBody(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta0, GLfloat theta1, GLfloat theta01, GLfloat theta11)
{
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
    GLfloat materialColor[] = { 1.0, 1.0, 0.0, 1.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = { 128 };
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    int R, G, B;
    R = 0;
    G = 255;
    B = 0; 
    if(on_move == false && !on_jump)
    {
        glPushMatrix();
        if (direction == 0)
            glTranslatef(0,y,0);
        else
            glTranslatef(0,y,0);
        glRotatef(direction,0,1,0);
        if(aiming)
        {
            DrawArm(baseWidth, baseHeight, armAngle, 0,R,G,B);
        }
        else
        {
            DrawArm(baseWidth, baseHeight, -170,0,R,G,B);
        }
        DrawLeg(baseWidth/2, 0, 180, 0,R,G,B);
        DrawHead(0, baseHeight+radiusWheel, thetaWheel,R,G,B);
        // DrawHead(0, baseHeight+baseHeight/2, thetaWheel,R,G,B);
        DrawRect(baseHeight, baseWidth*1.5,R,G,B);
        // if(aiming && hDirection == -1)
        // {
        //     DrawArm(-baseWidth, baseHeight, armAngle, 0,R,G,B);
        // }
        // else 
        // {
        DrawArm(-baseWidth, baseHeight, 170,0,R,G,B);
        // }
        
        DrawLeg(-baseWidth/2, 0, 180, 0,R,G,B);
        glPopMatrix();
    }
    else if (on_move && !on_jump)
    {
        glPushMatrix();

        if (direction == 0)
            glTranslatef(0,y,0);
        else
            glTranslatef(0,y,0);
        glRotatef(direction,0,1,0);
        if(aiming)
        {
            DrawArm(0, baseHeight, armAngle, 0,R,G,B);
        }
        else
        {
            DrawArm(0, baseHeight, theta0, 0,R,G,B);
        }
        DrawLeg(0, 0, theta0, theta01,R,G,B);
        DrawHead(0, baseHeight+radiusWheel, thetaWheel,R,G,B);
        // DrawHead(0, baseHeight+baseHeight/2, thetaWheel,R,G,B);
        DrawRect(baseHeight, baseWidth,R,G,B);
        DrawArm(0, baseHeight, theta1, 0,R,G,B);
        DrawLeg(0, 0, theta1, theta11,R,G,B);
        glPopMatrix();
        // printf("gX on move: %f\n", gX);
        // printf("gy on move: %f\n", gY);
    }
    else if (on_jump)
    {
        glPushMatrix();
        if (direction == 0)
            glTranslatef(0,y,-7);
        else
            glTranslatef(0,y,-9);
        glRotatef(direction,0,1,0);
        if(aiming)
        {
            DrawArm(baseWidth, baseHeight, armAngle, 0,R,G,B);
        }
        else
        {
            DrawArm(baseWidth, baseHeight, -30,gThetaJump01/2,R,G,B);
        }
        DrawLeg(baseWidth/2, 0, -gThetaJump0, -gThetaJump01,R,G,B);
        DrawHead(0, baseHeight+radiusWheel, thetaWheel,R,G,B);
        // DrawHead(0, baseHeight+baseHeight/2, gThetaWheel,R,G,B);
        DrawRect(baseHeight, baseWidth*1.5,R,G,B);
        DrawArm(0, baseHeight, -30,gThetaJump11/2,R,G,B);
        DrawLeg(-baseWidth/2, 0, -gThetaJump1, -gThetaJump11,R,G,B);
        glPopMatrix();
    }
    // printf("gX: %f\n", gX);
    // printf("gy: %f\n", gY);
    // printf("ground: %f\n", ground);
    // printf("armAngle: %f\n", armAngle);
}

bool Player::GravityEffect(GLfloat dx, GLdouble deltaTime)
{
    if(ground < gY)
    {
        gY -= percentual*deltaTime;
        if(!colliderState)
            gX += (dx/9)*percentual*deltaTime;
        return true;
    }
    else
    {
        return false;
    }
}

void Player::Jump(GLfloat dx, GLfloat dy, GLdouble deltaTime)
{
    if(!colliderState)
        gX += (dx/9)*percentual*deltaTime;
    
    gY += percentual*deltaTime;
    if(dx > 0)
    {
        direction = 0;
        hDirection = 1;
    }
    if(dx < 0)
    {
        direction = 180;
        hDirection = -1;
    }
    gThetaJump0  = -175;
    gThetaJump1  = -175;
    gThetaJump01 = 0;
    gThetaJump11 = 0;
}
void Player::Rotate(GLfloat angle, GLdouble deltaTime)
{
    direction += angle*deltaTime/15;
}

GLfloat degreeToRad(int degree)
{
	return (degree * M_PI)/180;
}

void Player::Move(GLfloat delta, GLdouble deltaTime)
{
    GLfloat dx, dz;

    dx = cos(degreeToRad(direction)) * delta;
    dz = sin(degreeToRad(direction)) * delta;

    gX += dx*percentual*deltaTime/15;
    gZ -= dz*percentual*deltaTime/15;

    // if(dx > 0)
    // {
    //     direction = 0;
    //     hDirection = 1;
    // }
    // if(dx < 0)
    // {
    //     direction = 180;
    //     hDirection = -1;    
    // }
    if(ground < gY)
        gY = gY;
    else
        gY = ground; // * deltaTime/15;
    // gTheta0 += inc0 * abs(dx) ;
    // gTheta1 += inc1 * abs(dx);
    // gTheta01 += inc01 * abs(dx);
    // gTheta11 += inc11 * abs(dx);
    // if(gTheta01 == -45 || gTheta01 == 0)dddd
    //     inc01 *= -1;
    // if(gTheta11 == -45 || gTheta11 == 0)
    //     inc11 *= -1;
    // if(gTheta1 == 225 || gTheta1 == 135)
    //     inc1 *= -1;
    // if(gTheta0 == -225 || gTheta0 == -135)
    //     inc0 *= -1;
    // if(gTheta1 == 225) 
    // {
    //     inc11 = 0;
    //     gTheta11 = 0;
    // }
    // if(gTheta1 == 135)
    // {
    //     inc11 = -1; 
    //     gTheta11 = 0;
    // }
    // if(gTheta0 == -225) 
    // {
    //     inc01 = -1;
    //     gTheta01 = 0;
    // }
    // if(gTheta0 == -135)
    // {
    //     inc01 = 0; 
    //     gTheta01 = 0;
    // }
}

void Player::Aiming(bool is_aiming)
{
    aiming = is_aiming;
}

void Player::OnMove(bool is_on_move)
{
    on_move = is_on_move;
}

void Player::OnJump(bool is_on_jump)
{
    on_jump = is_on_jump;
}

GLint Player::DetectBackground(Platform *platforms, int len, GLfloat pos)
{
    GLfloat y = - (platforms[0].GetY() - pos);

    if(gX-baseWidth <= platforms[0].GetX())  
    {
        gX= platforms[0].GetX() + baseWidth;
        return -1;
    }
    else if(gX+baseWidth >= (platforms[0].GetX() + platforms[0].GetW()))
    {
        gX= platforms[0].GetX() + (platforms[0].GetW() - baseWidth);
        return 1;
    }
    return 0;
}
GLint Player::DetectGround(Platform *platforms, Enemy *enemies, int lenPlatforms, int lenEnemies, GLfloat pos)
{
    // printf("(-gY + legHeight + pos) %f\n", (-gY + legHeight + pos) );
    for (int i = 0; i < lenEnemies; i++)
    {
        GLfloat y = (enemies[i].GetY());
        if(sqrt(pow(gX - enemies[i].GetX(), 2)) < baseWidth &&
           sqrt(pow((gY - legHeight) - (y+baseHeight+gThetaWheel),2)) <= legHeight &&
           !enemies[i].GetDefeatState())
        {
            if (ground <= enemies[i].GetY())
            {
                ground = enemies[i].GetY();
                // printf("enemies[%d].GetY(): %f\n", i, enemies[i].GetY() );

                return enemies[i].GetY();
            }
        }
    }
    for (int i = 0; i < lenPlatforms; i++)
    {
        GLfloat y = - (platforms[i].GetY() - pos);
        if(((gX+(baseWidth/2)+memberWidth > platforms[i].GetX()) && (gX-(baseWidth/2)-memberWidth < (platforms[i].GetX() + platforms[i].GetW()))) &&
            sqrt(pow((gY - legHeight) - y,2)) <= baseHeight+legHeight)
        {
            if (ground <= platforms[i].GetY())
            {
                ground = platforms[i].GetY();
                // printf("gY Player: %f\n", y );

                return platforms[i].GetY();
            }
        }
    }

    return -8888;
}

GLint Player::DetectDistance(Enemy *enemies, int lenEnemies)
{
    GLfloat min = 9999;
    GLint index = 0;
    for (int i = 0; i < lenEnemies; i++)
    {   
        if(!enemies[i].GetDefeatState())
        {
            GLfloat x = enemies[i].GetX();
            GLfloat y = enemies[i].GetY();
            
            GLfloat distance = abs(sqrt(pow(gX - x, 2) + pow(gY - y, 2)));
            if(distance < min)
            {
                min = distance;
                index = i;
            }
        }
        
    }
    return index;
}

GLint Player::DetectCollision(Platform *platforms, Enemy *enemies, int lenPlatforms, int lenEnemies, GLfloat pos)
{
    for (int i = 0; i < lenEnemies; i++)
    {   
        GLfloat y = enemies[i].GetY();
        
        if (((gY - legHeight/4 <= y) &&  
            (gY + legHeight/4 >= (y - (baseHeight+gThetaWheel)))) &&
            !enemies[i].GetDefeatState())
        {
            if(sqrt(pow(gX + baseWidth - enemies[i].GetX(), 2)) < baseWidth)
            {
                gX= enemies[i].GetX() - baseWidth*2;
                return 0;
            }
            else if(sqrt(pow(gX - baseWidth - enemies[i].GetX(), 2)) < baseWidth)
            {
                gX= enemies[i].GetX() + baseWidth*2;
                return 0;
            }
        }

    }
    for (int i = 0; i < lenPlatforms; i++)
    {   
        GLfloat y = - (platforms[i].GetY() - pos);
        

        // if(gY + legHeight/4 == y)
        
        if ((( gY + baseHeight*2 + radiusWheel <= y) &&  (gY + baseHeight*2 + legHeight >= (y - platforms[i].GetH()))))
        {   
            if((gX-baseWidth/2 - memberWidth >= platforms[i].GetX()) && gX+baseWidth < (platforms[i].GetX() + platforms[i].GetW()) &&
                gX+baseWidth/2 + memberWidth <= platforms[i].GetX() + (platforms[i].GetW()))
            {
                colliderState = true;
                // gX= platforms[i].GetX() - baseWidth;
                return 1;
            }
        }
        if (( gY <= y) &&  gY >= (y - platforms[i].GetH()))
        {
            if((gX+baseWidth >= platforms[i].GetX()) && gX+baseWidth < (platforms[i].GetX() + platforms[i].GetW()) &&
                gX+baseWidth <= platforms[i].GetX() + (platforms[i].GetW()/4))
            {
                colliderState = true;
                gX= platforms[i].GetX() - baseWidth;
                return 1;
            }
            else if((gX-baseWidth > platforms[i].GetX()) && gX-baseWidth <= (platforms[i].GetX() + platforms[i].GetW()) &&
                gX-baseWidth > platforms[i].GetX() + (platforms[i].GetW()/2) + (platforms[i].GetW()/4))
            {
                colliderState = true;
                gX= platforms[i].GetX() + (platforms[i].GetW() +baseWidth);
                return 1;
            }
        }
        else
        {
            colliderState = false;
        }
    }
    return 2;
}

void Player::RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut)
{
    if (angle < -0.1)
    {
        x = x - (sin((angle*M_PI)/180) * paddleHeight) * 1.8;
        y = y - (- cos((angle*M_PI)/180) * paddleHeight) * 1.8;
    }
    else if (angle > 0.1)
    {
        x = x - sin((angle*M_PI)/180) * paddleHeight * 1.8;
        y = y - (- cos((angle*M_PI)/180) * paddleHeight) * 1.8;
    }
    xOut = x;
    yOut = y;
}

void Player::DeleteFireball()
{
    if(t)
    {
        t = NULL;
        delete t;
    }
}

void Player::Atira()
{      
    GLfloat xa, ya, x, y;
    Fireball *fireball;
    GLfloat th = armAngle;
   
    t = fireball;
    fireballOn = true;
    
    if (hDirection == -1)
    {
        if(on_move == false && !on_jump)
        {
            x = baseWidth;
            y = GetY() + baseHeight;
        }
        else if (on_move && !on_jump)
        {
            x = 0;
            y = GetY() + baseHeight;
        }
        else if (on_jump)
        {
            x = baseWidth;
            y = GetY() + baseHeight;
        }
        RotatePoint(x, y, armAngle, xa, ya);
        *fireball = Fireball(xa, ya, armAngle);
    }
    else
    {
        if(on_move == false && !on_jump)
        {
            x = baseWidth;
            y = GetY() + baseHeight;
        }
        else if (on_move && !on_jump)
        {
            x = baseHeight;
            y = GetY() + baseHeight;
        }
        else if (on_jump)
        {
            x = 0;
            y = GetY() + baseHeight;
        }
        RotatePoint(x, y, -armAngle, xa, ya);
        *fireball = Fireball(xa, ya, -armAngle);
    } 
}

bool Player::Atingido(Fireball *fireball, GLfloat pos)
{
    GLfloat x, y;
    if (fireball)
    {
        fireball->GetPos(x,y);

        if(abs(sqrt(pow(gX - (-x + pos), 2))) < baseWidth &&
           abs(sqrt(pow(gY - (y), 2))) < (baseHeight*1.5))
        {            
            defeat = true;
            return true;
        }
        return false;
    }  
    return false;     
}