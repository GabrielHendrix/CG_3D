#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "player.h"
#include "fireball.h"
#include "enemy.h"
#include "platform.h"
#include "objloader.h"
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
using boost::property_tree::ptree;
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
using namespace std; 
std::string XML_PATH1;
#define INC_KEY 1
#define INC_KEYIDLE 3.0

//Key status
int keyStatus[256];
// Window dimensions
const GLint Width = 500;
const GLint Height = 500;

//Componentes do mundo virtual sendo modelado
Player player; 
std::vector<Enemy> enemies;
int CalculatesNumberOfEnemies();
int CalculatesNumberOfPlatforms();
int CalculatesLocationAndSizeOfPlayer();
int VectorPlatformsCreator();
int CalculatesViewsize();
// camera parameters
int buttonDown = 0;
double camXYAngle;
double camXZAngle;
int lastX;
int lastY;
GLfloat ballZ, ballY;
int vet[9999]; 
static char str[1000];
void * font = GLUT_BITMAP_9_BY_15;
std::vector<Platform> platforms, background;
Fireball* fireball;
Fireball* fireballEnemy;
bool onJump, onFall, pressJump, mouseRight, mouseLeft, gndIsPlat;
int aux2, incJump, atingido, incAir, incAirControl, numberOfEnemies, numberOfPlatforms;
Enemy* enemiesPointer;
GLint collisionDetect, aux, bgDetect, damage, ind, ViewingWidth, ViewingHeight, ground;
GLfloat shootAngle, previousGround, newGround, jumpSize, deltaX, mousePosY, detectedEnemyGnd, enemyGnd;
mesh playerArm;

//ID das meshes lidas
int movIdle = -1;
int movIdleFire = -1;
int movRunFire = -1;
int movRunEnemy[9999];
int movPunch = -1;
int movRun = -1;
int movDance = -1;
//Controles dos golpes
int running = 0;
int fire = 0;
int firing = 0;
int punching = 0;
int dancing = 0;
int acting = 0;
int currentTimeAnim = 0;
int lastTimeAnim = 0;
int updateDrawing = 0;

//Usar meshlab para obter os pontos abaixo
void drawPlayer(){
     // glMatrixMode(GL_MODELVIEW);
     
     //Escolhe entre iniciar o desenho do chute ou soco
     if (!fire && running && !acting){
          player.drawInit(movRun);
          acting = 1;
     }
     else if (fire && running && !acting){
          player.drawInit(movRunFire);
          acting = 1;
     }
     else if(!running && fire && !acting){
          player.drawInit(movIdleFire);
          acting = 1;
     }
     else if(!fire && !running && acting){
          player.drawInit(movIdle);
          acting = 0;
     }
     // if (!running && !acting){
     //      player.drawInit(movIdle);
     //      acting = 1;
     // }
//    } else if (punching && ! acting){
//        player.drawInit(movPunch);
//        acting = 1;
//    } else if (dancing && ! acting){
//        player.drawInit(movDance);
//        acting = 1;
//    }

     //Desenha as proximas frames ateh acabar
     int rtn = player.drawNext();
     //Reinicia com o movimento de parado esperando a luta
     if (rtn){
          if(fire && !running){
               player.drawInit(movIdleFire);
          }
          else if(fire && running){
               player.drawInit(movRunFire);
          }
          else if(!fire && running){
               player.drawInit(movRun);
          }
          else{
               player.drawInit(movIdle);
          }
          acting = 0;
          punching = 0;
          running = 0;
          dancing = 0;
          fire = 0;
     }
}

//Usar meshlab para obter os pontos abaixo
void drawEnemy(int iter){
     //Desenha as proximas frames ateh acabar
     int rtn = enemiesPointer[iter].drawNext();
     //Reinicia com o movimento de parado esperando a luta
     if (rtn){
          enemiesPointer[iter].drawInit(movRunEnemy[iter]);
          acting = 0;
          punching = 0;
          running = 0;
          dancing = 0;
     }
}

void InitGame(const std::string arg)
{
     XML_PATH1 = arg;
     fireball = NULL; 
     // fireballEnemy = NULL;
     onJump = false;
     onFall = false;
     pressJump = false;
     incJump = 0;
     atingido = 0;
     incAir = 0;
     ind = 0;
     incAirControl = 0;
     numberOfEnemies = CalculatesNumberOfEnemies();
     numberOfPlatforms = CalculatesNumberOfPlatforms();
     camXYAngle=0;
     camXZAngle=0;
     lastX = 0;
     lastY = 0;
     enemiesPointer = NULL;
     aux = CalculatesViewsize();
     // Viewing dimensions
     ViewingWidth = aux;
     ViewingHeight = aux;
     aux = VectorPlatformsCreator();
     aux2 = CalculatesLocationAndSizeOfPlayer();
     ground = background[0].GetH()/2;
     previousGround = background[0].GetH()/2;
     newGround = background[0].GetH()/2;
     jumpSize = 0;
     deltaX = 0;
     shootAngle = 0.0;
     mousePosY = 0;
     collisionDetect = 2;
     mouseRight = false;
     mouseLeft = false;
     bgDetect = 0;
     gndIsPlat = false;
     enemiesPointer = &enemies[0];
     damage = ViewingWidth/4;
     
}
void Restart(void)
{
     if(keyStatus[(int)('r')])
     {
          enemies.clear();
          aux2 = CalculatesLocationAndSizeOfPlayer();
          ground = background[0].GetH()/2;
          previousGround = background[0].GetH()/2;
          newGround = background[0].GetH()/2;
          atingido = 0; 
          damage = ViewingWidth/4;
          bgDetect = 0;
          for (int i = 0; i < numberOfEnemies; i++)
          {
               enemiesPointer[i].SetDefeatState(false);  
          }
     }
}

int StringToInt(std::string value)
{
     int n = value.length();
     char aux[n+1];
     strcpy(aux, value.c_str());
     char* token = strtok(aux, ".");
     return atoi(token);
}

GLint CalculatesViewsize()
{
     boost::property_tree::ptree pt1;
     boost::property_tree::read_xml( XML_PATH1, pt1  );
     int iter = 0;
     GLint viewSize = 0;
     // Traverse property tree example
     BOOST_FOREACH( boost::property_tree::ptree::value_type const& node, pt1.get_child( "svg" ) ) 
     {
          boost::property_tree::ptree subtree = node.second;         

          if( node.first == "rect" ) 
          {
               BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, subtree.get_child( "" ) ) 
               {
                    std::string label = v.first;

                    if ( label != "<xmlattr>" )
                    {
                         std::string value = subtree.get<std::string>( label );
                    }
                    else
                    {
                         std::string width = subtree.get<std::string>( "<xmlattr>.width" );
                         std::string height = subtree.get<std::string>( "<xmlattr>.height" );
                         std::string x = subtree.get<std::string>( "<xmlattr>.x" );
                         std::string y = subtree.get<std::string>( "<xmlattr>.y" );
                         std::string fill = subtree.get<std::string>( "<xmlattr>.fill" );

                         if (fill == "blue")
                         {
                              std::string width = subtree.get<std::string>( "<xmlattr>.width" );
                              std::string height = subtree.get<std::string>( "<xmlattr>.height" );
                              std::string x = subtree.get<std::string>( "<xmlattr>.x" );
                              std::string y = subtree.get<std::string>( "<xmlattr>.y" );
                              std::string fill = subtree.get<std::string>( "<xmlattr>.fill" );                              
                              viewSize = StringToInt(height);
                              background.push_back(Platform(StringToInt(x), StringToInt(y), 0, StringToInt(height), StringToInt(width),(GLint)viewSize/2,0,0,255));
                         }
                    }
               }
          }
     }
     return viewSize;
}

GLint VectorPlatformsCreator()
{
     boost::property_tree::ptree pt1;
     boost::property_tree::read_xml( XML_PATH1, pt1  );
     int iter = 0;
     // Traverse property tree example
     BOOST_FOREACH( boost::property_tree::ptree::value_type const& node, pt1.get_child( "svg" ) ) 
     {
          boost::property_tree::ptree subtree = node.second;         

          if( node.first == "rect" ) 
          {
               BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, subtree.get_child( "" ) ) 
               {
                    std::string label = v.first;

                    if ( label != "<xmlattr>" )
                    {
                         std::string value = subtree.get<std::string>( label );
                    }
                    else
                    {
                         std::string width = subtree.get<std::string>( "<xmlattr>.width" );
                         std::string height = subtree.get<std::string>( "<xmlattr>.height" );
                         std::string x = subtree.get<std::string>( "<xmlattr>.x" );
                         std::string y = subtree.get<std::string>( "<xmlattr>.y" );
                         std::string fill = subtree.get<std::string>( "<xmlattr>.fill" );
                         if (fill == "black")
                         {
                              platforms.push_back(Platform(StringToInt(x), StringToInt(y), 0, StringToInt(height), StringToInt(width),(GLint)ViewingHeight/2,0,0,0));
                              iter++;
                         }
                    }
               }
          }
     }
     return iter;
}

int CalculatesLocationAndSizeOfPlayer()
{
     boost::property_tree::ptree pt1;
     boost::property_tree::read_xml( XML_PATH1, pt1  );
     // Traverse property tree example
     BOOST_FOREACH( boost::property_tree::ptree::value_type const& node, pt1.get_child( "svg" ) ) 
     {
          boost::property_tree::ptree subtree = node.second;  

          if( node.first == "circle" ) 
          {
               BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, subtree.get_child( "" ) ) 
               {
                    std::string label = v.first;

                    if ( label != "<xmlattr>" )
                    {
                         std::string value = subtree.get<std::string>( label );
                    }
                    else
                    {
                         std::string cx = subtree.get<std::string>( "<xmlattr>.cx" );
                         std::string cy = subtree.get<std::string>( "<xmlattr>.cy" );
                         std::string fill = subtree.get<std::string>( "<xmlattr>.fill" );
                         std::string radius = subtree.get<std::string>( "<xmlattr>.r" );
                         
                         if (fill == "green")
                         {
                              player.SetX(StringToInt(cx));
                              player.SetY(StringToInt(radius)-StringToInt(cy));
                              player.SetRadius(StringToInt(radius)*0.3);
                              player.SetPercentual((StringToInt(radius)*3)/130.0);
                         }
                         if (fill == "red")
                         {
                              enemies.push_back(Enemy(StringToInt(cx), StringToInt(cy), 0, (StringToInt(radius)*3)/130.0, StringToInt(radius)*0.3));
                         }
                    }
               }
          }
     }
     return 0;
}

void PrintEndGame(GLfloat x, GLfloat y)
{
     glColor3f(0, 255, 0);
     //Cria a string a ser impressa
     char *tmpStr;
     sprintf(str, "Ganhou!");
     //Define a posicao onde vai comecar a imprimir
     glRasterPos2f(x, y);
     //Imprime um caractere por vez
     tmpStr = str;
     while( *tmpStr )
     {
          glutBitmapCharacter(font, *tmpStr);
          tmpStr++;
     }
}

void PrintGameOver(GLfloat x, GLfloat y)
{
     glColor3f(0, 255, 0);
     //Cria a string a ser impressa
     char *tmpStr;
     sprintf(str, "Perdeu!");
     //Define a posicao onde vai comecar a imprimir
     glRasterPos2f(x, y);
     //Imprime um caractere por vez
     tmpStr = str;
     while( *tmpStr )
     {
          glutBitmapCharacter(font, *tmpStr);
          tmpStr++;
     }
}

void PrintScore(GLfloat x, GLfloat y)
{
     glColor3f(1.0, 1.0, 1.0);
     //Cria a string a ser impressa
     char *tmpStr;
     GLint enemiesDefeated = 0;
     for (int i=0; i<numberOfEnemies; i++)
     {
          if(enemiesPointer[i].GetDefeatState())
          {
               enemiesDefeated++;
          }
     }
     atingido = enemiesDefeated;
     sprintf(str, "Inimigos atingidos: %d/%d", atingido, numberOfEnemies);
     //Define a posicao onde vai comecar a imprimir
     glRasterPos2f(x, y);
     //Imprime um caractere por vez
     tmpStr = str;
     while( *tmpStr )
     {
          glutBitmapCharacter(font, *tmpStr);
          tmpStr++;
     }

}

void DrawRect(GLint height, GLint width, GLint lenght, GLfloat R, GLfloat G, GLfloat B)
{
	/* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
	glColor3f (R, G, B);
	/* Desenhar um polígono branco (retângulo) */
	glBegin(GL_POLYGON);
		glVertex3f (0, 0, lenght);  // Lado Esquerdo Cima
		glVertex3f (width, 0, lenght); // Lado Direito Cima
		glVertex3f (width, height, lenght); // Lado Direito Baixo
		glVertex3f (0, height, lenght);  // 
	glEnd();
}

void PrintLife(GLfloat x, GLfloat y)
{
     glPushMatrix();
     glTranslatef(x - ViewingWidth/8, y, 0);
     DrawRect(ViewingWidth/20, ViewingWidth/4, 0, 0, 0, 0);
     DrawRect(ViewingWidth/20, abs(damage), 1, 1, 0, 0);
     glPopMatrix();
     glColor3f(1.0, 1.0, 1.0);
     char *tmpStr;
     // sprintf(str, "LIFE: %d", abs(damage));
     sprintf(str, "LIFE");
     glRasterPos2f(x - ViewingWidth/8, y - ViewingWidth/40 - 1);
     tmpStr = str;
     while( *tmpStr )
     {
          glutBitmapCharacter(font, *tmpStr);
          tmpStr++;
     }
}
//Funcao auxiliar para normalizar um vetor a/|a|
void normalize(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

//Funcao auxiliar para fazer o produto vetorial entre dois vetores a x b = out
void cross(float a[3], float b[3], float out[3])
{
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

GLfloat degToRad(int degree)
{
	return (degree * M_PI)/180;
}


void DrawAxes(double size)
{
    GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, 
            no_mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);

    //x axis
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_r);
        glColor3fv(mat_ambient_r);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();

    //y axis
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_g);
        glColor3fv(mat_ambient_g);
        glRotatef(90,0,0,1);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();

    //z axis
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_b);
        glColor3fv(mat_ambient_b);
        glRotatef(-90,0,1,0);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();
    
}
void renderScene(void)
{
          if (updateDrawing){
               updateDrawing = 0;
          }else{
               return;
          }
          glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
          // glPushMatrix();
          // glTranslatef(0,0,0);
          glLoadIdentity();
          glClearColor (0.30, 0.30, 1.0, 0.0);
          // glClearColor (0.0,0.0,0.0,1.0);
          glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          
          //Utiliza uma esfera de raio zoom para guiar a posicao da camera
          //baseada em dois angulos (do plano XZ e do plano XY)
          // gluLookAt( sin(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)),
          //                                    sin((camXYAngle*M_PI/180)),
          //           cos(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)),
          //           0, 0, 0,
          //           0, 1, 0);
  
          
          gluLookAt(-ViewingHeight/4, 0, 0,//-ViewingHeight/4,
                    ViewingHeight/2, 0, 0,
                    0, 1, 0);
          
          glRotatef((int)camXZAngle,0,0,1);
          glRotatef((int)camXYAngle,0,1,0);
          glTranslatef(0,-(player.GetY()+player.GetPlayerCamHeight()),-player.GetZ());
       
          GLfloat light_position[] = {0, 140, -20, 1};
          glLightfv(GL_LIGHT0,GL_POSITION,light_position); //GL_SPOT_DIRECTION GL_POSITION

          if(!keyStatus[(int)('r')])
          {
               if(damage <= 0)
               {
                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    PrintGameOver(-5,0); 
               }
               else if(bgDetect == 1) //(atingido == numberOfEnemies)
               {
                    // Clear the screen.                                                         
                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    PrintEndGame(-5,0);
               
               }
               else //if(atingido < numberOfEnemies)
               {
                    // Clear the screen.                                                         
                    // glClear(GL_COLOR_BUFFER_BIT);
                    // (background[0].GetY() + (background[0].GetH()/2))
                    glPushMatrix();
                    glTranslatef(-player.GetX(),background[0].GetY() + (background[0].GetH()/2),0);
                    background[0].Draw();
                    glPopMatrix();
  
          
                    // if(fireballEnemy)
                    // {
                    //      GLfloat xAux, yAux;
                    //      glPushMatrix();
                    //      fireballEnemy->GetPos(xAux, yAux);
                    //      glTranslatef(enemiesPointer[ind].GetX()- player.GetX(), 0,0);
                    //      fireballEnemy->Draw();
                    //      glPopMatrix();
                    // }
                    
                    for (int j = numberOfPlatforms-1; j >= 0; j--)
                    {
                         glPushMatrix();
                         glTranslatef(-player.GetX(),background[0].GetY() + (background[0].GetH()/2),0);
                         platforms[j].Draw();
                         glPopMatrix();
                    }

                    if(fireball) 
                    {
                         // glPushMatrix();
                         fireball->Draw(); 
                              // DrawAxes(5);    
                         // glPopMatrix();
                    }
        
                    glPushMatrix();
                         GLfloat mT[4][4] = { 1,0,0,0,
                                              0,1,0,0,
                                              0,0,1,0,
                                              0,0,player.GetZ(),1};
                         glMultMatrixf(&mT[0][0]);

                         GLfloat m[4][4] = { (GLfloat)cos(degToRad(player.GetHDirection())),0,(GLfloat)sin(degToRad(player.GetHDirection())),0,
                                            0,1,0,0,
                                            (GLfloat)-sin(degToRad(player.GetHDirection())),0,(GLfloat)cos(degToRad(player.GetHDirection())),0,
                                            0, 0, 0, 1};
                         glMultMatrixf(&m[0][0]);
                         // glRotatef(player.GetHDirection(), 0, 1, 0);
                         glRotatef(90, 0, 1, 0);

                         glTranslatef(0, player.GetY(), 0);
                         
                         glScalef(player.GetRadius()*2, player.GetRadius()*2, player.GetRadius()*2);
                         // glRotatef(player.GetHDirection(), 0, 1, 0);
                         drawPlayer();
                         // printf("hDirection: %d\n", player.GetHDirection());
                         if(firing)
                         {
                              player.SetPosArm(playerArm.vertsPos[661].x,
                                               playerArm.vertsPos[661].y,
                                               playerArm.vertsPos[661].z);
                              glTranslatef(player.vecMeshes[player.currentMovID][player.currentFrame].vertsPos[13704].x,
                                           player.vecMeshes[player.currentMovID][player.currentFrame].vertsPos[13704].y,
                                           player.vecMeshes[player.currentMovID][player.currentFrame].vertsPos[13704].z);
                              glRotatef(-90, 0, 1, 0);
                              glRotatef(player.GetArmAngle()+90, 0, 0, 1);
                              playerArm.draw();
                         }
                             
                         DrawAxes(2);
                    glPopMatrix();
                    for (int i = 0; i < numberOfEnemies; i++)
                    {  
                         if(!enemiesPointer[i].GetDefeatState())
                         {
                              glPushMatrix(); 
                              glTranslatef(enemiesPointer[i].GetX() - player.GetX(), enemiesPointer[i].GetY(),-ViewingHeight/4);
                                   drawEnemy(i);
                                   // glPushMatrix(); 
                                   //      glTranslatef(0, player.GetRadius()/2,0);
                                   //      glutSolidCube(player.GetRadius()/2);
                                   // glPopMatrix();
                                   enemiesPointer[i].SetZ(-ViewingHeight/4);
                              glPopMatrix();
                              glPushMatrix(); 
                              glTranslatef((enemiesPointer[i].GetX() + (enemiesPointer[i].GetRadius()/2)) - player.GetX(), 
                                           (enemiesPointer[i].GetY()+(player.GetRadius()*4)), 
                                           -ViewingHeight/4 + (enemiesPointer[i].GetRadius()/2));
                                   glutSolidCube(player.GetRadius());
                              glPopMatrix();
                         }
                    }

                    glDisable(GL_LIGHTING);
                    glDisable(GL_LIGHT0);
                    glDisable(GL_DEPTH_TEST);
                         PrintLife(-ViewingWidth/2 + 20, ViewingHeight/2 - 10);
                         PrintScore(5, ViewingHeight/2 - 5);
                    glEnable(GL_LIGHTING);
                    glEnable(GL_LIGHT0);
                    glEnable(GL_DEPTH_TEST); 
               }
               glutSwapBuffers(); 
          }
          else
          {
               Restart();
          }
}

void keyPress(unsigned char key, int x, int y)
{
    switch(key)
    {
          case 'a':
          case 'A':
               keyStatus[(int)('a')] = 1; //Using keyStatus trick
               if (acting) break;
               running = 1;
               break;
          case 'd':
          case 'D':
               keyStatus[(int)('d')] = 1; //Using keyStatus trick
               if (acting) break;
               running = 1;
               break; 
          case 's':
          case 'S':
               keyStatus[(int)('s')] = 1; //Using keyStatus trick
               if (acting) break;
               running = 1;
               break;
          case 'w':
          case 'W':
               keyStatus[(int)('w')] = 1; //Using keyStatus trick
               if (acting) break;
               running = 1;
               break; 
          case ' ':
               keyStatus[(int)(' ')] = 1;
               firing = 1;
               if (acting) break;
               fire = 1;
               break;
          case 'r':
          case 'R':
               keyStatus[(int)('r')] = 1; //Using keyStatus trick
               break;
          case 't':
          case 'T':
               keyStatus[(int)('t')] = 1; //Using keyStatus trick
               break;
          case 'x':
          case 'X':
               keyStatus[(int)('x')] = 1; //Using keyStatus trick
               break;
          case 'y':
          case 'Y':
               keyStatus[(int)('y')] = 1; //Using keyStatus trick
               break;
          case '2':
               keyStatus[(int)('2')] = 1; //Using keyStatus trick
               break;
          case '4':
               keyStatus[(int)('4')] = 1; //Using keyStatus trick
               break;
          case '6':
               keyStatus[(int)('6')] = 1; //Using keyStatus trick
               break;
          case '8':
               keyStatus[(int)('8')] = 1; //Using keyStatus trick
               break;
          case 27 :
               exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ReSetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
}

int CalculatesNumberOfPlatforms()
{
     boost::property_tree::ptree pt1;
     boost::property_tree::read_xml( XML_PATH1, pt1  );
     int numOfPlatforms = 0;
     
     // Traverse property tree example
     BOOST_FOREACH( boost::property_tree::ptree::value_type const& node, pt1.get_child( "svg" ) ) 
     {
          boost::property_tree::ptree subtree = node.second;         

          if( node.first == "rect" ) 
          {
               BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, subtree.get_child( "" ) ) 
               {
                    std::string label = v.first;

                    if ( label != "<xmlattr>" )
                    {
                         std::string value = subtree.get<std::string>( label );
                    }
                    else
                    {
                         std::string fill = subtree.get<std::string>( "<xmlattr>.fill" );
                         if (fill == "black")
                         {
                              numOfPlatforms++;
                         }
                    }
               }
          }
     }
     return numOfPlatforms;
}



int CalculatesNumberOfEnemies()
{
     boost::property_tree::ptree pt1;
     boost::property_tree::read_xml( XML_PATH1, pt1  );
     int numOfEnemies = 0;
     // Traverse property tree example
     BOOST_FOREACH( boost::property_tree::ptree::value_type const& node, pt1.get_child( "svg" ) ) 
     {
          boost::property_tree::ptree subtree = node.second;  

          if( node.first == "circle" ) 
          {
               BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, subtree.get_child( "" ) ) 
               {
                    std::string label = v.first;

                    if ( label != "<xmlattr>" )
                    {
                         std::string value = subtree.get<std::string>( label );
                    }
                    else
                    {
                         std::string fill = subtree.get<std::string>( "<xmlattr>.fill" );

                         if (fill == "red")
                         {
                              numOfEnemies++;
                         }
                    }
               }
          }
     }
     return numOfEnemies;
}

void init(void)
{     
     ReSetKeyStatus();
     // The color the windows will redraw. Its done to erase the previous frame.
     // glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).
     
     // glShadeModel (GL_FLAT);
     glShadeModel (GL_SMOOTH);
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     // glEnable(GL_LIGHT1);
     glEnable(GL_DEPTH_TEST);
     // glDepthFunc(GL_LEQUAL);
     // glDepthMask(GL_TRUE);
     // glDepthRange(0.0, 1.0);
     // glEnable(GL_CULL_FACE);
     // glCullFace(GL_BACK);
     // glFrontFace(GL_CW);
     movIdle =  player.loadMeshAnim("Blender/megaman_idle/megaman_idle_######.obj", 14);
     movIdleFire =  player.loadMeshAnim("Blender/megaman_idle_fire/megaman_idle_fire_######.obj", 14);
     movRun =  player.loadMeshAnim("Blender/megaman_run/megaman_run_######.obj", 12);
     movRunFire =  player.loadMeshAnim("Blender/megaman_run_fire/megaman_run_fire_######.obj", 12);
     playerArm.loadMesh("Blender/megaman_idle_fire_arm.obj");
     playerArm.loadText("Blender/megaman_arm_color.bmp");
     player.loadTexture("Blender/megaman_texture.bmp");
     player.drawInit(movIdle);


     // movIdleEnemy =  enemy.loadMeshAnim("Blender/megaman_idle/megaman_idle_######.obj", 14);
     for (int i = 0; i < numberOfEnemies; i++)
     {  
          movRunEnemy[i] =  enemiesPointer[i].loadMeshAnim("Blender/megaman_run/megaman_run_######.obj", 12);
          enemiesPointer[i].loadTexture("Blender/megaman_texture.bmp");
          enemiesPointer[i].drawInit(movRunEnemy[i]);
     }
     // glMatrixMode(GL_PROJECTION); // Select the projection matrix    
     // gluPerspective (50, 1, 
     //               5, 200);
     // glOrtho(-(ViewingWidth/2),     // X coordinate of left edge             
     //           (ViewingWidth/2),     // X coordinate of right edge            
     //           -(ViewingHeight/2),     // Y coordinate of bottom edge             
     //           (ViewingHeight/2),     // Y coordinate of top edge             
     //           -100,     // Z coordinate of the “near” plane            
     //           100);    // Z coordinate of the “far” plane
    
//     gluLookAt(1,2,5, 0,0,0, 0,1,0);

}

void idle(void)
{
     if(!keyStatus[(int)('r')])
     {
          double inc = INC_KEYIDLE;
          // Elapsed time from the initiation of the game.
          currentTimeAnim = glutGet(GLUT_ELAPSED_TIME);

          int fatorTempo = 80;
          if (currentTimeAnim - lastTimeAnim > fatorTempo){
               lastTimeAnim = currentTimeAnim;
               updateDrawing = 1;
          }
          
          static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
          GLint auxGround = 0;
          GLdouble currentTime, timeDiference, jumpTimeDif, deltaJump, deltaShoot, deltaTimeShoot;
          if(previousTime < 1000)
          {
               deltaShoot = glutGet(GLUT_ELAPSED_TIME);
          }
          //Pega o tempo que passou do inicio da aplicacao
          currentTime = glutGet(GLUT_ELAPSED_TIME);
          
          // Calcula o tempo decorrido desde de a ultima frame.
          timeDiference = currentTime - previousTime;
          player.SetTime(timeDiference);
          //Atualiza o tempo do ultimo frame ocorrido
          previousTime = currentTime;
          //Treat keyPress
          fireball = player.GetFireball();
    
          // if((currentTime - deltaShoot) > 3000)
          // {
          //      deltaShoot = glutGet(GLUT_ELAPSED_TIME);
          //      ind = player.DetectDistance(&enemiesPointer[0], numberOfEnemies);
          //      shootAngle = atanf((player.GetY() -  enemiesPointer[ind].GetY())/sqrt(pow(player.GetX() - enemiesPointer[ind].GetX(), 2))) * (180/M_PI);
          //      enemiesPointer[ind].SetArmAngle(shootAngle);
          //      enemiesPointer[ind].Aiming(true);
          // }
          // if((currentTime - deltaTimeShoot) > 1000 && !fireballEnemy && !enemiesPointer[ind].GetDefeatState())
          // {
          //      deltaTimeShoot = glutGet(GLUT_ELAPSED_TIME);
          //      enemiesPointer[ind].SetArmAngle(shootAngle);
               
          //      enemiesPointer[ind].Atira();
          //      fireballEnemy = enemiesPointer[ind].GetFireball();               
          // }
          auxGround = player.DetectGround(&platforms[0], &enemiesPointer[0], numberOfPlatforms, numberOfEnemies, background[0].GetY() + (background[0].GetH()/2));
          collisionDetect = player.DetectCollision(&platforms[0], &enemiesPointer[0], numberOfPlatforms, numberOfEnemies, background[0].GetY() +  (background[0].GetH()/2));
          bgDetect = player.DetectBackground(&background[0], 1, background[0].GetY() + (background[0].GetH()/2));
          if(auxGround != -8888)
          {
               // printf("background[0].GetY(): %f\n", background[0].GetY());
               // printf("ViewingWidth/2: %f\n", ViewingWidth/2);
               newGround = auxGround - (background[0].GetY() + (background[0].GetH()/2));
          }
          else
          {
               newGround = background[0].GetH()/2;
          }
          player.SetGround(-newGround);
          if(updateDrawing)
          {
               if(keyStatus[(int)('2')])
               {
                    if(camXZAngle > -60)
                         camXZAngle-=2;
               }
               if(keyStatus[(int)('8')])
               {
                    if(camXZAngle < 60)
                         camXZAngle+=2;
               }
               if(keyStatus[(int)('4')])
               {
                    camXYAngle-=2;
               }
               if(keyStatus[(int)('6')])
               {
                    camXYAngle+=2;
               }
          }
          
          if(keyStatus[(int)('a')] && !onJump && !onFall)
          {
               running = 1;
               player.Rotate(-inc,timeDiference);
               player.OnMove(true);
          }
          
          if(keyStatus[(int)('d')] && !onJump && !onFall)
          {
               running = 1;
               player.Rotate(inc,timeDiference);
               player.OnMove(true);
          }

          if(keyStatus[(int)('s')] && !onJump && !onFall)
          {
               running = 1;
               player.Move(-inc,timeDiference);
               player.OnMove(true);
          }
          
          if(keyStatus[(int)('w')] && !onJump && !onFall)
          {
               running = 1;
               player.Move(inc,timeDiference);
               player.OnMove(true);
          }
          if(!keyStatus[(int)('a')] && !keyStatus[(int)('d')] && !keyStatus[(int)('w')] && !keyStatus[(int)('s')] && !onJump && !onFall)
          {
               running = 0;
               player.OnMove(false);
               player.OnJump(false);
          }
          if(mouseRight && !onJump  && !onFall && !pressJump)
          {
               player.OnJump(true);
               onJump = true;
               jumpSize = player.GetY();
               pressJump = true;
          }
          if((abs(player.GetRadius()*26) <= abs(player.GetY()-jumpSize)) || collisionDetect!=2 || !mouseRight)
          {
               player.OnJump(false);
               onJump = false;
          }

          if(onJump)
          {
               
               if(keyStatus[(int)('s')])
               {
                    if(collisionDetect==2)
                    {
                         player.Jump(-(inc), (inc), timeDiference);
                         deltaX = -inc;
                    }
               }
               else if(keyStatus[(int)('w')])
               {
                    if(collisionDetect==2)
                    {
                         player.Jump((inc), (inc), timeDiference);
                         deltaX = inc;
                    }
               }
               else 
               {
                    if(collisionDetect==2)
                    {
                         player.Jump(0, (inc), timeDiference);
                         deltaX = 0;
                    }
               }
          }
          if(!onJump)
          {
               onFall = player.GravityEffect(deltaX, timeDiference);
               player.OnJump(onFall);
               if(keyStatus[(int)('s')])
               {
                    if(deltaX > 0)
                         deltaX -= deltaX*0.1;
               }
               else if(keyStatus[(int)('w')])
               {
                    if(deltaX < 0)
                         deltaX -= deltaX*0.1;
               }
               if(!onFall)
               {
                    deltaX = 0;
               }
          }
     
          if(!mouseRight)
          {
               pressJump = false;
          }
          if(keyStatus[(int)(' ')])
          {
               player.Aiming(true);
               fire = 1;
          }
          if(!keyStatus[(int)(' ')])
          {
               fire = 0;
               firing = 0;
               player.Aiming(false);
          }
          if(keyStatus[(int)('t')])
          {
               damage = ViewingWidth/20;
          }
          if(keyStatus[(int)('y')])
          {
               damage = ViewingWidth/4;
          }
          if(fireball)
          {    
               fireball->Move(timeDiference);
          
               if (fireball->DetectBulletCollision(&platforms[0], 
               numberOfPlatforms, player.GetX(), 
               background[0].GetY() + (background[0].GetH()/2)) ||
               fireball->DetectBackground(&background[0], 
               numberOfPlatforms, player.GetX(), 
               background[0].GetY() + (background[0].GetH()/2)) || 
               (fireball->GetDeltaDist() > ViewingWidth/2))
               // if(fireball->GetDeltaDist  () > ViewingWidth/2)
               { 
                    fireball = NULL;
                    delete fireball;
                    player.DeleteFireball();
               }
          
          }

          // if(fireballEnemy)
          // {    
          //      fireballEnemy->Move(timeDiference);
               
          //      if (fireballEnemy->DetectBulletCollision(&platforms[0], 
          //      numberOfPlatforms, enemiesPointer[ind].GetX(), 
          //      background[0].GetY() + (ViewingWidth/2)) ||
          //      fireballEnemy->DetectBackground(&background[0], 
          //      numberOfPlatforms, enemiesPointer[ind].GetX(), 
          //      background[0].GetY() + (ViewingWidth/2)))
          //      { 
          //           fireballEnemy = NULL;
          //           enemiesPointer[ind].DeleteFireball();
          //      }

          //      if (player.Atingido(fireballEnemy, enemiesPointer[ind].GetX()))
          //      {
          //           fireballEnemy = NULL;
          //           enemiesPointer[ind].DeleteFireball();
          //           if(damage > 0)
          //                damage -= ViewingWidth/20;
          //      }
          
          // }
          for(int i=0; i<numberOfEnemies; i++)
          {
               if(fireball)
               {
                    if (enemiesPointer[i].Atingido(fireball, player.GetX()) &&
                         !enemiesPointer[i].GetDefeatState())
                    {   
                         fireball = NULL;
                         delete fireball;
                         player.DeleteFireball();
                    }
               }
               if(!enemiesPointer[i].GetGroundState() && !enemiesPointer[i].GetDefeatState())
               {
                    detectedEnemyGnd = enemiesPointer[i].DetectGround(&platforms[0], 
                                                       numberOfPlatforms,
                                                       background[0].GetY() + (background[0].GetH()/2));
                    if(detectedEnemyGnd != -8888)
                    {
                         enemyGnd = platforms[detectedEnemyGnd].GetY() - (background[0].GetY() + (background[0].GetH()/2));
                         // enemyGnd = platforms[detectedEnemyGnd].GetY()  - (background[0].GetY() + (ViewingWidth/2));
                         gndIsPlat = true;
                    }
                    else
                    {
                         gndIsPlat = false;
                         enemyGnd = background[0].GetH()/2;
                    }
                    enemiesPointer[i].SetGround(-enemyGnd, gndIsPlat, detectedEnemyGnd);
                    enemiesPointer[i].SetY(enemyGnd);
                    // enemiesPointer[i].GravityEffect(0,timeDiference);
               }
               // Control animationb
               if (enemiesPointer[i].DetectCollision(&platforms[0], 
                                             numberOfPlatforms, 
                                             background[0].GetY() + (ViewingWidth/2)) || 
                                             enemiesPointer[i].DetectBackground(&background[0], 
                                             1, 
                                             background[0].GetY() + (background[0].GetH()/2)) && 
                                             !enemiesPointer[i].GetDefeatState())
               {
                    enemiesPointer[i].SetInc(-1);
               }
               
               if(collisionDetect != 0)
               {
                    enemiesPointer[i].MoveInX(inc*enemiesPointer[i].GetInc(),timeDiference);
                    enemiesPointer[i].OnMove(true);
               }
               else
               {
                   enemiesPointer[i].SetInc(-1);; 
               }
          }
          
          glutPostRedisplay();
     }
}

void mouse(int button, int state, int x, int y)
{    
     if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
     {
          if (!fireball && keyStatus[(int)(' ')])
          {
               player.Atira();
               
               mouseLeft = true;
               ballY = player.GetY();
               ballZ = player.GetZ();
          }
          if (fireball && keyStatus[(int)(' ')])
          {
               fireball = NULL;
               delete fireball;
               player.DeleteFireball();
          }

     } 
     else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
     {
          mouseLeft = false;
     }
     else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
     {
          mouseRight = true;
     }
     else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
     {
          mouseRight = false;
     }
}
void mousePosition(int x, int y)
{
     if(keyStatus[(int)('x')])
     {
          camXYAngle += x - lastX;
          camXYAngle = (int)camXYAngle % 360;
          if (camXZAngle + (y - lastY) < 60 && camXZAngle + (y - lastY) > -60)
          {
               camXZAngle += y - lastY;
               camXZAngle = (int)camXZAngle % 360;      
          }
          lastY = y;
          lastX = x;     
     }
     else
     {
          GLfloat fY = ((GLfloat)(Width - y+player.GetY())/Width);
          mousePosY = 235 + fY*90;
          player.SetArmAngle(mousePosY);
     }
}

void changeCamera(int angle, int w, int h)
{
    glMatrixMode (GL_PROJECTION);

    glLoadIdentity ();
//      glOrtho(-(ViewingWidth/2),     // X coordinate of left edge             
//                (ViewingWidth/2),     // X coordinate of right edge            
//                -(ViewingHeight/2),     // Y coordinate of bottom edge             
//                (ViewingHeight/2),     // Y coordinate of top edge             
//                -100,     // Z coordinate of the “near” plane            
//                100);    // Z coordinate of the “far” plane
    
//     gluLookAt(1,2,5, 0,0,0, 0,1,0);
    gluPerspective (angle, 
            (GLfloat)w / (GLfloat)h, 1, 150.0);

    glMatrixMode (GL_MODELVIEW);
}

void reshape (int w, int h) {

    glViewport (0, 0, (GLsizei)w, (GLsizei)h);

    changeCamera(60, w, h);
}


int main(int argc, char *argv[])
             {
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int count;
    cout << "\nCommand-line arguments: \n";
    for(count=0; count < argc; count++)
    {
         cout << "argv [" << count << "]  " << argv[count] << "\n";
    }
    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Computação Gráfica -Trabalho 3D ");
    InitGame(argv[1]);
    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mousePosition);
    init();
 
    glutMainLoop();
 
    return 0;
}
