#include "enemy.h"
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "imageloader.h"


//Aponta para uma das Enemy de movimento e coloca na primeira frame
void Enemy::drawInit(int movID){
    currentMovID = movID;
    currentFrame = 0;
}

//Desenha a proxima frame do movimento atual e retorna se ja chegou no final
bool Enemy::drawNext(){
    this->draw(this->currentMovID, this->currentFrame);
    this->currentFrame++;
    
    return (this->currentFrame >= this->vecMeshes[this->currentMovID].size());
}


//Carrega as Enemy de um movimento do caminho path e assumindo qtd arquivos
int Enemy::loadMeshAnim(string path, int qtd){
    vector<meshEnemy> vec;
    meshEnemy m;
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
void Enemy::draw(int movID, int frameId){
    // glPushMatrix();
    // glTranslatef(0, gY, gZ);
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
    // glPopMatrix();
}

//Le a textura
bool Enemy::loadTexture(string path){
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
bool meshEnemy::loadMesh(string path){
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
                posEnemy vert;
                fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
                vertsPos.push_back(vert);
            }
            else if(strcmp(lineHeader,"vt")==0){
                texEnemy vert;
                fscanf(file, "%f %f\n", &vert.u, &vert.v);
                vertsTex.push_back(vert);
            }
            else if(strcmp(lineHeader, "vn")==0){
                normEnemy vert;
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
            verticeStripEnemy vert;
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
void meshEnemy::draw(){
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

GLfloat degreeToRadEnemy(int degree)
{
	return (degree * M_PI)/180;
}

bool Enemy::GravityEffect(GLfloat delta, GLdouble deltaTime)
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
    dx = cos(degreeToRadEnemy(direction)) * delta;
    dz = sin(degreeToRadEnemy(direction)) * delta;


    if(ground < gY)
    {
        gY -= percentual*deltaTime;
        gX += (dx/9)*percentual*deltaTime;
        gZ -= (dz/9)*percentual*deltaTime;
        return true;
    }
    else
    {
        return false;
    }
    // if(ground < gY)
    // {
    //     // gY -= percentual*deltaTime;
    //     gY = ground;
    //     // gX += (dx/9)*percentual*deltaTime;
    // }

    // return true; 
}
void Enemy::MoveInX(GLfloat delta, GLdouble deltaTime)
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
    dx = cos(degreeToRadEnemy(direction)) * delta;
    dz = sin(degreeToRadEnemy(direction)) * delta;

    gX += dx*percentual*deltaTime/15;
    gZ -= dz*percentual*deltaTime/15;    
    
    if(ground < gY-legHeight)
        gY = gY-legHeight;
    else
    {
        gY = ground;
    }
    // gX += dx*percentual*deltaTime/15;
    // if(dx > 0)
    // {
    //     direction = 0;
    // }
    // if(dx < 0)
    // {
    //     direction = 180;
    // }
    
    // if(ground < gY)
    //     gY = gY;
    // else
    // gY = ground - (cos((abs(gTheta0)*M_PI)/180) * legHeight ); // * deltaTime/15;
    // gTheta0 += inc0 * abs(dx) ;
    // gTheta1 += inc1 * abs(dx);
    // gTheta01 += inc01 * abs(dx);
    // gTheta11 += inc11 * abs(dx);
    // if(gTheta01 == -45 || gTheta01 == 0)
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

void Enemy::Aiming(bool is_aiming)
{
    aiming = is_aiming;
}

void Enemy::OnMove(bool is_on_move)
{
    on_move = is_on_move;
}

bool Enemy::DetectBackground(Platform *platforms, int len, GLfloat pos)
{
    GLfloat y = - (platforms[0].GetY() - pos);

    if(gX-baseWidth <= platforms[0].GetX())  
    {
        gX= platforms[0].GetX() + baseWidth;
        return true;
    }
    else if(gX+baseWidth >= (platforms[0].GetX() + platforms[0].GetW()))
    {
        gX= platforms[0].GetX() + (platforms[0].GetW() - baseWidth);
        return true;
    }
    return false;
}
GLint Enemy::DetectGround(Platform *platforms, int len, GLfloat pos)
{

    for (int i = 0; i < len; i++)
    {

        // printf("(-gY + legHeight + pos) %f\n", -(gY - pos + legHeight) );
        GLfloat y = - (platforms[i].GetY() - pos);
        if(((gX+(baseWidth/2)+memberWidth > platforms[i].GetX()) && (gX-(baseWidth/2)-memberWidth < (platforms[i].GetX() + platforms[i].GetW()))) &&
            sqrt(pow((gY - legHeight) - y,2)) <= baseHeight+legHeight)
        // if(((gX-baseWidth >= platforms[i].GetX()) && 
        //     (gX+baseWidth <= (platforms[i].GetX() + platforms[i].GetW()))) &&
        //     // gY - legHeight/4 >= y)
        //     sqrt(pow((gY + legHeight) - platforms[i].GetY(),2)) <= legHeight)
        {
            if (ground <= platforms[i].GetY())
            {
                // printf("gY enemy[%d]: %f\n", i, y);

                ground = platforms[i].GetY();
                return i;
            }
        }
    }
    return -8888;
}

bool Enemy::DetectCollision(Platform *platforms, int len, GLfloat pos)
{
    GLfloat y;
    if(!gndIsPlat)    
    {
        for (int i = 0; i < len; i++)
        {   
            y = - (platforms[i].GetY() - pos);
            
            if (((gY - legHeight/4 <= y) &&  (gY + legHeight/4 >= (y - platforms[i].GetH()))))
            {
                if((gX+baseWidth >= platforms[i].GetX()) && gX+baseWidth < (platforms[i].GetX() + platforms[i].GetW()) &&
                    gX+baseWidth <= platforms[i].GetX() + (platforms[i].GetW()/4))
                {
                    gX= platforms[i].GetX() - baseWidth;
                    return true;
                }
                else if((gX-baseWidth > platforms[i].GetX()) && gX-baseWidth <= (platforms[i].GetX() + platforms[i].GetW()) &&
                    gX-baseWidth > platforms[i].GetX() + (platforms[i].GetW()/2) + (platforms[i].GetW()/4))
                {
                    gX= platforms[i].GetX() + (platforms[i].GetW() +baseWidth);
                    return true;
                }
            }
        }
    }
    else 
    {
        // printf("detectedEnemyGnd %d\n", detectedEnemyGnd);
        y = - (platforms[detectedEnemyGnd].GetY() - pos);

        if(gX-baseWidth <= (platforms[detectedEnemyGnd].GetX()))
        {
            gX = platforms[detectedEnemyGnd].GetX() + baseWidth + 1;
            return true;
        }
        else if(gX+baseWidth >= (platforms[detectedEnemyGnd].GetX() + platforms[detectedEnemyGnd].GetW()))
        {
            gX = platforms[detectedEnemyGnd].GetX() + (platforms[detectedEnemyGnd].GetW() - baseWidth - 1);
            return true;
        }
    }
    return false;
}

void Enemy::RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut)
{
    if (angle < -0.1)
    {
        x = x - (sin((angle*M_PI)/180) * paddleHeight);
        y = y - (- cos((angle*M_PI)/180) * paddleHeight);
    }
    else if (angle > 0.1)
    {
        x = x - sin((angle*M_PI)/180) * paddleHeight;
        y = y - (- cos((angle*M_PI)/180) * paddleHeight);
    }
    xOut = x;
    yOut = y;
}

void Enemy::DeleteFireball()
{
    if(t)
    {
        t = NULL;
        delete t;
    }
}

void Enemy::Atira()
{      
    GLfloat xa, ya, x, y;
    Fireball *fireball;
    GLfloat th = 235;
   
    t = fireball;
    fireballOn = true;
    
    // if (hDirection == -1)
    // {
    //     if(!on_move)
    //     {
    //         x = baseWidth;
    //         y = GetY() + baseHeight;
    //     }
    //     else if (on_move)
    //     {
    //         x = 0;
    //         y = GetY() + baseHeight;
    //     }
    //     // RotatePoint(x, y, armAngle + 270, xa, ya);
    //     // *fireball = Fireball(xa, ya, armAngle + 270);
    //     *fireball = Fireball(x, y, armAngle + 270);

    // }
    // else
    // {
    //     if(!on_move)
    //     {
    //         x = baseWidth;
    //         y = GetY() + baseHeight;
    //     }
    //     else if (on_move)
    //     {
    //         x = baseHeight;
    //         y = GetY() + baseHeight;
    //     }
    //     // RotatePoint(x, y, -armAngle - 270, xa, ya);
    //     // *fireball = Fireball(xa, ya, -armAngle - 270);
    //     *fireball = Fireball(x, y, -armAngle - 270);
    // } 
}
bool Enemy::Atingido(Fireball *fireball, GLfloat pos)
{
    // GLfloat x, y;
    // if (fireball)
    // {
    //     fireball->GetPos(x,y);

    //     if(abs(sqrt(pow(gX - (-x + pos), 2))) < baseWidth &&
    //        abs(sqrt(pow(gY - (y), 2))) < (baseHeight*1.5))
    //     {
            
    //         defeat = true;
    //         return true;
    //     }
    //     return false;
    // }  
    return false;     
}