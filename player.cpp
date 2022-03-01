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
    glScalef(baseHeight, baseHeight, baseHeight);
    glRotatef(90, 0, 1, 0);
    glRotatef(hDirection, 0, 1, 0);
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


GLfloat degreeToRad(int degree)
{
	return (degree * M_PI)/180;
}

bool Player::GravityEffect(GLfloat delta, GLdouble deltaTime)
{
    GLfloat dx, dz;

    if(delta > 0)
    {
        hDirection = direction;
    }
    if (delta < 0)
    {
        hDirection = direction + 180;    
    }
    dx = cos(degreeToRad(direction)) * delta;
    dz = sin(degreeToRad(direction)) * delta;


    if(ground < gY)
    {
        gY -= percentual*deltaTime;
                
        if(!colliderState)
        {
            gX += (dx/9)*percentual*deltaTime;
            gZ -= (dz/9)*percentual*deltaTime;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void Player::Jump(GLfloat delta, GLfloat dy, GLdouble deltaTime)
{
    GLfloat dx, dz;

    if(delta > 0)
    {
        hDirection = direction;
    }
    if (delta < 0)
    {
        hDirection = direction + 180;    
    }
    dx = cos(degreeToRad(direction)) * delta;
    dz = sin(degreeToRad(direction)) * delta;

    
    if(!colliderState)
    {
        gX += (dx/9)*percentual*deltaTime;
        gZ -= (dz/9)*percentual*deltaTime;
    }
    gY += percentual*deltaTime;

    gThetaJump0  = -175;
    gThetaJump1  = -175;
    gThetaJump01 = 0;
    gThetaJump11 = 0;
}
void Player::Rotate(GLfloat angle, GLdouble deltaTime)
{
    direction += angle*deltaTime/15;
}


void Player::Move(GLfloat delta, GLdouble deltaTime)
{
    GLfloat dx, dz;

    if(delta > 0)
    {
        hDirection = direction;
    }
    if (delta < 0)
    {
        hDirection = direction + 180;    
    }
    dx = cos(degreeToRad(direction)) * delta;
    dz = sin(degreeToRad(direction)) * delta;

    gX += dx*percentual*deltaTime/15;
    gZ -= dz*percentual*deltaTime/15;    
    
    if(ground < gY-legHeight)
        gY = gY-legHeight;
    else
    {
        gY = ground;
    }
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
    if(gZ-baseWidth >= platforms[0].GetZ())  
    {
        gZ= platforms[0].GetZ() + baseWidth;
        return -1;
    }
    else if(gZ-baseWidth <= (platforms[0].GetZ() - platforms[0].GetL()))
    {
        gZ= platforms[0].GetZ() - (platforms[0].GetL() - baseWidth);
        return -1;
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
                if(sqrt(pow(gZ + baseWidth - enemies[i].GetZ(), 2)) < baseWidth)
                {
                    gX= enemies[i].GetX() - baseWidth*2;
                    gZ= enemies[i].GetZ() - baseWidth*2;
                    return 0;
                }
                if(sqrt(pow(gZ - baseWidth - enemies[i].GetZ(), 2)) < baseWidth)
                {
                    gX= enemies[i].GetX() - baseWidth*2;
                    gZ= enemies[i].GetZ() + baseWidth*2;
                    return 0;
                }
            }
            else if(sqrt(pow(gX - baseWidth - enemies[i].GetX(), 2)) < baseWidth)
            {
                if(sqrt(pow(gZ + baseWidth - enemies[i].GetZ(), 2)) < baseWidth)
                {
                    gX= enemies[i].GetX() + baseWidth*2;
                    gZ= enemies[i].GetZ() - baseWidth*2;
                    return 0;
                }
                if(sqrt(pow(gZ - baseWidth - enemies[i].GetZ(), 2)) < baseWidth)
                {
                    gX= enemies[i].GetX() + baseWidth*2;
                    gZ= enemies[i].GetZ() + baseWidth*2;
                    return 0;
                }
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
            else if((gX+baseWidth >= platforms[i].GetX()) && gX+baseWidth < (platforms[i].GetX() + platforms[i].GetW()) &&
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
        // if (( gY <= y) &&  gY >= (y - platforms[i].GetH()))
        // {
        //     if((gX+baseWidth >= platforms[i].GetX()) && gX+baseWidth < (platforms[i].GetX() + platforms[i].GetW()) &&
        //         gX+baseWidth <= platforms[i].GetX() + (platforms[i].GetW()/4))
        //     {
        //         colliderState = true;
        //         gX= platforms[i].GetX() - baseWidth;
        //         return 1;
        //     }
        //     else if((gX-baseWidth > platforms[i].GetX()) && gX-baseWidth <= (platforms[i].GetX() + platforms[i].GetW()) &&
        //         gX-baseWidth > platforms[i].GetX() + (platforms[i].GetW()/2) + (platforms[i].GetW()/4))
        //     {
        //         colliderState = true;
        //         gX= platforms[i].GetX() + (platforms[i].GetW() +baseWidth);
        //         return 1;
        //     }
        // }
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
    
    if (hDirection >= 180)
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