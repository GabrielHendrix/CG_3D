#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <stdio.h>
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
using namespace std;

typedef struct POS_ARM{
    float x;
    float y;
    float z;
} posArm;

typedef struct NORM_ARM{
    float x;
    float y;
    float z;
} normArm;

typedef struct TEX_ARM{
    float u;
    float v;
} texArm;

typedef struct VERTICE_STRIP_ARM{
    posArm vPos;
    normArm vNorm;
    texArm vTex;
} verticeStripArm;

class mesh{
	
    public:
    int texWidth;
    int texHeight;
    GLuint texID;
    vector<posArm> vertsPos; //Lista de posicao dos vertices (original do OBJ)   
    vector<normArm> vertsNorm; //Lista de normais dos vertices (original do OBJ)   
    vector<texArm> vertsTex; //Lista de coor de textura dos vertices (original do OBJ)   
    vector<verticeStripArm> vertsS;//Lista ordenada de vertices das faces para o desenho do modelo via trangle strip
    mesh(){};
    bool loadMesh(string path);//Path é o caminho para o arquivo .obj e deve conter o nome do arquivo.obj
    bool loadText(string path);
    void draw();
};
#endif
