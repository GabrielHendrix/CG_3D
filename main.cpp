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
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
using boost::property_tree::ptree;
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

using namespace std;


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
double camXYAngle=0;
double camXZAngle=0;
int lastX = 0;
int lastY = 0;
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

//ID das meshes lidas
int movIdle = -1;
int movPunch = -1;
int movKick = -1;
int movDance = -1;

//Controles dos golpes
int kicking = 0;
int punching = 0;
int dancing = 0;
int acting = 0;
int currentTimeAnim = 0;
int lastTimeAnim = 0;
int updateDrawing = 0;

//Usar meshlab para obter os pontos abaixo
void desenhaJogador(){
     // glMatrixMode(GL_MODELVIEW);
     
//    //Escolhe entre iniciar o desenho do chute ou soco
//    if (kicking && !acting){
//        player.drawInit(movKick);
//        acting = 1;
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
          player.drawInit(movIdle);
          acting = 0;
          punching = 0;
          kicking = 0;
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
     
     enemiesPointer = NULL;
     aux = CalculatesViewsize();
     // Viewing dimensions
     ViewingWidth = aux;
     ViewingHeight = aux;
     aux = VectorPlatformsCreator();
     aux2 = CalculatesLocationAndSizeOfPlayer();
     ground = ViewingHeight/2;
     previousGround = ViewingHeight/2;
     newGround = ViewingHeight/2;
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
          ground = ViewingHeight/2;
          previousGround = ViewingHeight/2;
          newGround = ViewingHeight/2;
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

void PlatformsInsertionSort(int size)
{
     int x, i, j, aux;
     int vetAux[size];
     for(i = 0; i < size; i++)
     {
          vet[i] = i;
          vetAux[i] = platforms[i].GetX();
     }
     // for(i = 0; i < size; i++)
     // {
     //      vet[i] = i;
     //      vetAux[i] = i;
     // }
     i = 0;
     for(i = i + 1; i <= size-1; i++)
     {
          j = i;
          x = vetAux[i];
          aux = vet[i];
          while(j > 0 && vetAux[j - 1] > x)
          {
               vetAux[j] = vetAux[j - 1];
               vet[j] = vet[j - 1];
               j = j - 1;
          }
          vetAux[j] = x;
          vet[j] = aux;
     }
     for(i = 0; i < size; i++)
     {
          printf("vet[%d]: %d\n", i, vet[i]);
     }
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
     PlatformsInsertionSort(iter);
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
     PlatformsInsertionSort(iter);
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
                              player.SetRadius(StringToInt(radius));
                              player.SetPercentual((StringToInt(radius)*3)/130.0);
                         }
                         if (fill == "red")
                         {
                              enemies.push_back(Enemy(StringToInt(cx), StringToInt(cy), (StringToInt(radius)*3)/130.0, StringToInt(radius)));
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

void renderScene(void)
{
          if (updateDrawing){
               updateDrawing = 0;
          }else{
               return;
          }
          glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
          glLoadIdentity();
          glClearColor (0.30, 0.30, 1.0, 0.0);
          // glClearColor (0.0,0.0,0.0,1.0);
          glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          
          //Utiliza uma esfera de raio zoom para guiar a posicao da camera
          //baseada em dois angulos (do plano XZ e do plano XY)
          gluLookAt( sin(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)),
                                             sin((camXYAngle*M_PI/180)),
                    cos(camXZAngle*M_PI/180)*cos((camXYAngle*M_PI/180)),
                    0, 0, 0,
                    0, 1, 0);
          // GLfloat light_position[] = { 0, 0, 0, 1.0 };
          // glLightfv(  GL_LIGHT0, GL_POSITION, light_position);
          GLfloat light_position[] = {0, 140, -20, 1};
          glLightfv(GL_LIGHT0,GL_POSITION,light_position); //GL_SPOT_DIRECTION
          glDisable(GL_LIGHTING);
          glPointSize(15);
          glColor3f(1.0,1.0,0.0);
          glBegin(GL_POINTS);
               glVertex3f(light_position[0],light_position[1],light_position[2]);
          glEnd();    
          glEnable(GL_LIGHTING);
          // GLfloat light_position1[] = { 10.0, (GLfloat) ViewingHeight, 10, 1.0 };
          // glLightfv(  GL_LIGHT1, GL_POSITION, light_position1);
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
               
                    // glDisable(GL_LIGHTING);
                    // glDisable(GL_LIGHT0);
                    // glDisable(GL_DEPTH_TEST);
                    // glDisable(GL_CULL_FACE);
                         glPushMatrix();
                         glTranslatef(-player.GetX(),background[0].GetY() + (ViewingWidth/2),0);
                         background[0].Draw();
                         glPopMatrix();
                    // glEnable(GL_LIGHTING);
                    // glEnable(GL_LIGHT0);
                    // glEnable(GL_DEPTH_TEST);
                    // glEnable(GL_CULL_FACE); 
                    
                    
                    //* Enemy 

                    if(fireball) 
                    {
                         fireball->Draw();      
                    }
          
                    if(fireballEnemy)
                    {
                         GLfloat xAux, yAux;
                         glPushMatrix();
                         fireballEnemy->GetPos(xAux, yAux);
                         glTranslatef(enemiesPointer[ind].GetX()- player.GetX(), 0,0);
                         fireballEnemy->Draw();
                         glPopMatrix();
                    }
                    
                    glPushMatrix();
                    glTranslatef(-player.GetX(),background[0].GetY() + (ViewingWidth/2),0);

                    for (int j = numberOfPlatforms-1; j >= 0; j--)
                    {
                         platforms[vet[j]].Draw();
                    }
                    glPopMatrix(); 
                    glDisable(GL_CULL_FACE);
                         desenhaJogador();
                    glEnable(GL_CULL_FACE);  
                    
                    // glPushMatrix();
                    // glTranslatef(-player.GetX(),background[0].GetY() + (ViewingWidth/2),0);

                    // for (int j = 0; j < numberOfPlatforms; j++)
                    // for (int j = numberOfPlatforms-1; j >= 0; j--)
                    // {
                    //      GLfloat auxY = - (platforms[vet[j]].GetY() - (background[0].GetY() + (ViewingWidth/2)));
                    //      if (player.GetX() < platforms[vet[j]].GetX() || player.GetY() > auxY)
                    //      {
                    //           // printf("platform GY: %f \n", auxY);
                    //           platforms[vet[j]].Draw();
                    //      }
                    // }
                    // glPopMatrix();
                    // player.Draw();
          
               
                    
                    // printf("player GY: %f", player.GetY());

                    // for (int i = 0; i < numberOfEnemies; i++)
                    // {  
                    //      if(!enemiesPointer[i].GetDefeatState())
                    //      {
                    //           glPushMatrix(); 
                    //           glTranslatef(enemiesPointer[i].GetX() - player.GetX(), enemiesPointer[i].GetY(),0);
                    //           enemiesPointer[i].Draw();
                    //           glPopMatrix();
                    //      }
                    // }
                    // glPushMatrix();
                    // glTranslatef(-player.GetX(),background[0].GetY() + (ViewingWidth/2),0);

                    // // for (int j = 0; j < numberOfPlatforms; j++)
                    // for (int j = numberOfPlatforms-1; j >= 0; j--)
                    // {
                    //      GLfloat auxY = - (platforms[vet[j]].GetY() - (background[0].GetY() + (ViewingWidth/2)));
                    //      if (player.GetX() > platforms[vet[j]].GetX()  && player.GetY() < auxY)
                    //      {
                    //           platforms[vet[j]].Draw();
                    //      }
                    // }
                    // glPopMatrix();
                    glDisable(GL_LIGHTING);
                    glDisable(GL_LIGHT0);
                    glDisable(GL_DEPTH_TEST);
                    glDisable(GL_CULL_FACE);
                         // glLoadIdentity();
                         // glClearColor (0.30, 0.30, 1.0, 0.0);
                         // // glClearColor (0.0,0.0,0.0,1.0);
                         // glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                         PrintLife(-ViewingWidth/2 + 20, ViewingHeight/2 - 10);
                         PrintScore(5, ViewingHeight/2 - 5);
                    glEnable(GL_LIGHTING);
                    glEnable(GL_LIGHT0);
                    glEnable(GL_DEPTH_TEST);
                    glEnable(GL_CULL_FACE); 
               }
               glutSwapBuffers(); // Desenha the new frame of the game.
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
               break;
          case 'd':
          case 'D':
               keyStatus[(int)('d')] = 1; //Using keyStatus trick
               break; 
          case 's':
          case 'S':
               keyStatus[(int)('s')] = 1; //Using keyStatus trick
               break;
          case 'w':
          case 'W':
               keyStatus[(int)('w')] = 1; //Using keyStatus trick
               break; 
          case ' ':
               keyStatus[(int)(' ')] = 1;
               break;
          case 'r':
          case 'R':
               keyStatus[(int)('r')] = 1; //Using keyStatus trick
               break;
          case 't':
          case 'T':
               keyStatus[(int)('t')] = 1; //Using keyStatus trick
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
     glDepthFunc(GL_LEQUAL);
     glDepthMask(GL_TRUE);
     glDepthRange(0.0, 1.0);
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
     glFrontFace(GL_CW);
     movIdle =  player.loadMeshAnim("Blender/megaman_run/megaman_run_######.obj", 12);
     player.loadTexture("Blender/megaman_texture.bmp");
     player.drawInit(movIdle);
     glMatrixMode(GL_PROJECTION); // Select the projection matrix    
     // gluPerspective ((GLfloat)ViewingWidth, (GLfloat)ViewingWidth, 
               //     -100, 100);
     glOrtho(-(ViewingWidth/2),     // X coordinate of left edge             
               (ViewingWidth/2),     // X coordinate of right edge            
               -(ViewingHeight/2),     // Y coordinate of bottom edge             
               (ViewingHeight/2),     // Y coordinate of top edge             
               -100,     // Z coordinate of the “near” plane            
               100);    // Z coordinate of the “far” plane
    
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
    
          if((currentTime - deltaShoot) > 3000)
          {
               deltaShoot = glutGet(GLUT_ELAPSED_TIME);
               ind = player.DetectDistance(&enemiesPointer[0], numberOfEnemies);
               shootAngle = atanf((player.GetY() -  enemiesPointer[ind].GetY())/sqrt(pow(player.GetX() - enemiesPointer[ind].GetX(), 2))) * (180/M_PI);
               enemiesPointer[ind].SetArmAngle(shootAngle);
               enemiesPointer[ind].Aiming(true);
          }
          if((currentTime - deltaTimeShoot) > 1000 && !fireballEnemy && !enemiesPointer[ind].GetDefeatState())
          {
               deltaTimeShoot = glutGet(GLUT_ELAPSED_TIME);
               enemiesPointer[ind].SetArmAngle(shootAngle);
               
               enemiesPointer[ind].Atira();
               fireballEnemy = enemiesPointer[ind].GetFireball();               
          }
          auxGround = player.DetectGround(&platforms[0], &enemiesPointer[0], numberOfPlatforms, numberOfEnemies, background[0].GetY() + (ViewingWidth/2));
          collisionDetect = player.DetectCollision(&platforms[0], &enemiesPointer[0], numberOfPlatforms, numberOfEnemies, background[0].GetY() + (ViewingWidth/2));
          bgDetect = player.DetectBackground(&background[0], 1, background[0].GetY() + (ViewingWidth/2));
          if(auxGround != -8888)
          {
               newGround = auxGround - (background[0].GetY() + (ViewingWidth/2));
          }
          else
          {
               newGround = ViewingHeight/2;
          }
          player.SetGround(-newGround);
          if(updateDrawing)
          {
               if(keyStatus[(int)('4')])
               {
                    camXZAngle-=1;
               }
               if(keyStatus[(int)('6')])
               {
                    camXZAngle+=1;
               }
               if(keyStatus[(int)('2')])
               {
                    camXYAngle-=1;
               }
               if(keyStatus[(int)('8')])
               {
                    camXYAngle+=1;
               }
          }
          
          if(keyStatus[(int)('a')] && !onJump && !onFall)
          {
               player.Rotate(inc,timeDiference);
               player.OnMove(true);
          }
          
          if(keyStatus[(int)('d')] && !onJump && !onFall)
          {
               player.Rotate(-inc,timeDiference);
               player.OnMove(true);
          }

          if(keyStatus[(int)('s')] && !onJump && !onFall)
          {
               player.Move(-inc,timeDiference);
               player.OnMove(true);
          }
          
          if(keyStatus[(int)('w')] && !onJump && !onFall)
          {
               player.Move(inc,timeDiference);
               player.OnMove(true);
          }
          if(!keyStatus[(int)('a')] && !keyStatus[(int)('d')] && !keyStatus[(int)('w')] && !keyStatus[(int)('s')] && !onJump && !onFall)
          {
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
          if((abs(player.GetRadius()*8) <= abs(player.GetY()-jumpSize)) || collisionDetect!=2 || !mouseRight)
          {
               player.OnJump(false);
               onJump = false;
          }

          if(onJump)
          {
               
               if(keyStatus[(int)('a')])
               {
                    if(collisionDetect==2)
                    {
                         player.Jump(-(inc), (inc), timeDiference);
                         deltaX = -inc;
                    }
               }
               else if(keyStatus[(int)('d')])
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
               if(keyStatus[(int)('a')])
               {
                    if(deltaX > 0)
                         deltaX -= deltaX*0.1;
               }
               else if(keyStatus[(int)('d')])
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
          }
          if(!keyStatus[(int)(' ')])
          {
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
               background[0].GetY() + (ViewingWidth/2)) ||
               fireball->DetectBackground(&background[0], 
               numberOfPlatforms, player.GetX(), 
               background[0].GetY() + (ViewingWidth/2)) || 
               (fireball->GetDeltaDist() > ViewingWidth/2))
               { 
                    fireball = NULL;
                    delete fireball;
                    player.DeleteFireball();
               }
          
          }

          if(fireballEnemy)
          {    
               fireballEnemy->Move(timeDiference);
               
               if (fireballEnemy->DetectBulletCollision(&platforms[0], 
               numberOfPlatforms, enemiesPointer[ind].GetX(), 
               background[0].GetY() + (ViewingWidth/2)) ||
               fireballEnemy->DetectBackground(&background[0], 
               numberOfPlatforms, enemiesPointer[ind].GetX(), 
               background[0].GetY() + (ViewingWidth/2)))
               { 
                    fireballEnemy = NULL;
                    enemiesPointer[ind].DeleteFireball();
               }

               if (player.Atingido(fireballEnemy, enemiesPointer[ind].GetX()))
               {
                    fireballEnemy = NULL;
                    enemiesPointer[ind].DeleteFireball();
                    if(damage > 0)
                         damage -= ViewingWidth/20;
               }
          
          }
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
                                                       background[0].GetY() + (ViewingWidth/2));
                    if(detectedEnemyGnd != -8888)
                    {
                         enemyGnd = platforms[detectedEnemyGnd].GetY()  - (background[0].GetY() + (ViewingWidth/2));
                         gndIsPlat = true;
                    }
                    else
                    {
                         gndIsPlat = false;
                         enemyGnd = ViewingHeight/2;
                    }
                    enemiesPointer[i].SetGround(-enemyGnd, gndIsPlat, detectedEnemyGnd);
                    enemiesPointer[i].GravityEffect(0,timeDiference);
               }
               // Control animationb
               if (enemiesPointer[i].DetectCollision(&platforms[0], 
                                             numberOfPlatforms, 
                                             background[0].GetY() + (ViewingWidth/2)) || 
                                             enemiesPointer[i].DetectBackground(&background[0], 
                                             1, 
                                             background[0].GetY() + (ViewingWidth/2)) && 
                                             !enemiesPointer[i].GetDefeatState())
               {
                    enemiesPointer[i].SetHDirection(-1);
               }
          
               if(collisionDetect != 0)
               {
                    enemiesPointer[i].MoveInX(inc*enemiesPointer[i].GetHDirection(),timeDiference);
                    enemiesPointer[i].OnMove(true);
               }
               else
               {
                   enemiesPointer[i].SetHDirection(-1);; 
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
     GLfloat fY = ((GLfloat)(Width - y+player.GetY())/Width);
     mousePosY = 235 + fY*90;
     player.SetArmAngle(mousePosY);
}
void mouse_motion(int x, int y)
{
    if (!buttonDown)
        return;
    
    camXZAngle -= x - lastX;
    camXYAngle += y - lastY;

    lastX = x;
    lastY = y;
    glutPostRedisplay();
}
void reshape (int w, int h)
{
    //Ajusta o tamanho da tela com a janela de visualizacao
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        gluPerspective (45, (GLfloat)h/(GLfloat)w, 1, 1000);
    else
        gluPerspective (45, (GLfloat)w/(GLfloat)h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
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
    glutCreateWindow("Tranformations 2D");
    InitGame(argv[1]);
    // Define callbacks.
    glutDisplayFunc(renderScene);
//     glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mousePosition);
    init();
 
    glutMainLoop();
 
    return 0;
}
