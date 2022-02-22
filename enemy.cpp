#include "enemy.h"
#include <math.h>
#include <stdio.h>


void normalizeEnemy(float a[3])
{
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}
void crossEnemy(GLfloat a[3], GLfloat b[3], GLfloat out[3])
{
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}
void Enemy::DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
	/* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
	glColor3f (R, G, B);
	/* Desenhar um polígono branco (retângulo) */
    GLint lenght = 2;
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
        
        // down face
        a[0] = -width/2;
        a[1] = 0;
        a[2] = lenght;

        b[0] = width/2;
        b[1] = 0;
        b[2] = 0.0;

        crossEnemy(a,b,norm);
        normalizeEnemy(norm);
        glNormal3f(norm[0], norm[1], norm[2]);
        glVertex3f (gX, -gY-height, 0.0);  // Lado Esquerdo Baixo 
        glVertex3f (gX+width, -gY-height, 0.0); // Lado Direito Baixo
        glVertex3f (gX+width, -gY-height, -lenght); // Lado Direito Cima
        glVertex3f (gX, -gY-height, -lenght);  // Lado Esquerdo Cima

        glVertex3f (-width/2, 0, 0.0);  // Lado Esquerdo Baixo 
        glVertex3f (width/2, 0, 0.0); // Lado Direito Baixo        
        glVertex3f (-width/2, 0, -lenght);  // Lado Esquerdo Cima
        glVertex3f (width/2, 0, -lenght); // Lado Direito Cima
        //left face
        a[0] = -width/2;
        a[1] = 0;
        a[2] = -lenght;

        b[0] = -width/2;
        b[1] = height;
        b[2] = 0.0;

        crossEnemy(a,b,norm);
        normalizeEnemy(norm);
        glNormal3f(norm[0], norm[1], norm[2]);
        glVertex3f (-width/2, 0, 0.0);  // Lado Esquerdo Cima
        glVertex3f (-width/2, 0, -lenght); // Lado Direito Cima
        glVertex3f (-width/2, height, -lenght); // Lado Direito Baixo
        glVertex3f (-width/2, height, 0.0);  // Lado Esquerdo Baixo
        
        //right face
        a[0] = width/2;
        a[1] = height;
        a[2] = 0.0;

        b[0] = width/2;
        b[1] = 0;
        b[2] = -lenght;

        crossEnemy(a,b,norm);
        normalizeEnemy(norm);
        glNormal3f(norm[0], norm[1], norm[2]);

        glVertex3f (width/2, 0, 0.0);  // Lado Esquerdo Cima
        glVertex3f (width/2, 0, -lenght); // Lado Direito Cima
        glVertex3f (width/2, height, -lenght); // Lado Direito Baixo
        glVertex3f (width/2, height, 0.0);  // Lado Esquerdo Baixo
        
        // front face
        a[0] = width/2;
        a[1] = height;
        a[2] = -lenght;

        b[0] = -width/2;
        b[1] = 0;
        b[2] = -lenght;

        crossEnemy(a,b,norm);
        normalizeEnemy(norm);
        glNormal3f(norm[0], norm[1], norm[2]);
        glVertex3f (-width/2, 0, -lenght);  // Lado Esquerdo Cima
        glVertex3f (width/2, 0, -lenght); // Lado Direito Cima
        glVertex3f (width/2, height, -lenght); // Lado Direito Baixo
        glVertex3f (-width/2, height, -lenght);  // Lado Esquerdo Baixo 


        // up face
        a[0] = -width/2;
        a[1] = height;
        a[2] = -lenght;

        b[0] = width/2;
        b[1] = height;
        b[2] = 0;

        crossEnemy(a,b,norm);
        normalizeEnemy(norm);
        glNormal3f(norm[0], norm[1], norm[2]);
        glVertex3f (width/2, height, 0.0); // Lado Direito Baixo        
        glVertex3f (-width/2, height, 0.0);  // Lado Esquerdo Baixo
        glVertex3f (-width/2, height, -lenght);  // Lado Esquerdo Cima
        glVertex3f (width/2, height, -lenght); // Lado Direito Cima
         
        // back face
        a[0] = -width/2;
        a[1] = height;
        a[2] = 0.0;

        b[0] = gX+width;
        b[1] = 0;
        b[2] = 0.0;

        crossEnemy(a,b,norm);
        normalizeEnemy(norm);
        glNormal3f(norm[0], norm[1], norm[2]);
        glVertex3f (-width/2, 0, 0.0);  // Lado Esquerdo Cima
		glVertex3f (width/2, 0, 0.0); // Lado Direito Cima
		glVertex3f (width/2, height, 0.0); // Lado Direito Baixo
		glVertex3f (-width/2, height, 0.0);  // Lado Esquerdo Baixo
    glEnd();
}


void Enemy::DrawHead(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B)
{
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(thetaWheel, 0, 0, 1);
    DrawCircle(radiusWheel, R, G, B);
    glPopMatrix();
}

void Enemy::DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);
    glPointSize(1.0);
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i+=1) 
            // for (int j = 0; j < 200; j++)
                // glVertex3f(radius * cos(i), radius * sin(i), (float)(-j/75));
            glVertex3f(radius * cos(i), radius * sin(i), 0.0);
    glEnd();
}

void Enemy::DrawArm(GLfloat x, GLfloat y, GLfloat theta, GLfloat theta2, GLfloat R, GLfloat G, GLfloat B)
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

void Enemy::DrawLeg(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat R, GLfloat G, GLfloat B)
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

void Enemy::DrawBody(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta0, GLfloat theta1, GLfloat theta01, GLfloat theta11)
{
    int R, G, B;
    R = 255;
    G = 0;
    B = 0; 
    if(on_move == false)
    {
        glRotatef(direction,0,1,0);
        if(aiming)
        {
            DrawArm(baseWidth, baseHeight, armAngle + 270, 0,R,G,B);
        }
        else
        {
            DrawArm(baseWidth, baseHeight, -170,0,R,G,B);
        }
        DrawLeg(baseWidth/2, 0, 180, 0,R,G,B);
        DrawHead(0, baseHeight+radiusWheel, thetaWheel,R,G,B);
        // DrawHead(0, baseHeight+baseHeight/2, thetaWheel,R,G,B);
        DrawRect(baseHeight, baseWidth*1.5,R,G,B);
        DrawArm(-baseWidth, baseHeight, 170,0,R,G,B);      
        DrawLeg(-baseWidth/2, 0, 180, 0,R,G,B);
    }
    else if (on_move)
    {
        glRotatef(direction,0,1,0);
        if(aiming)
        {
            DrawArm(0, baseHeight, armAngle + 270, 0,R,G,B);
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
    }
    // printf("gX: %f\n", gX);
    // printf("gy: %f\n", gY);
    // printf("ground: %f\n", ground);
    // printf("armAngle: %f\n", armAngle);
}
bool Enemy::GravityEffect(GLfloat dx, GLdouble deltaTime)
{
    if(ground < gY-legHeight)
    {
        // gY -= percentual*deltaTime;
        gY = ground + legHeight;
        // gX += (dx/9)*percentual*deltaTime;
    }

    return true; 
}
void Enemy::MoveInX(GLfloat dx, GLdouble deltaTime)
{
    gX += dx*percentual*deltaTime/15;
    if(dx > 0)
    {
        direction = 0;
    }
    if(dx < 0)
    {
        direction = 180;
    }
    
    if(ground < gY-legHeight)
        gY = gY;
    else
    gY = ground - (cos((abs(gTheta0)*M_PI)/180) * legHeight ); // * deltaTime/15;
    gTheta0 += inc0 * abs(dx) ;
    gTheta1 += inc1 * abs(dx);
    gTheta01 += inc01 * abs(dx);
    gTheta11 += inc11 * abs(dx);
    if(gTheta01 == -45 || gTheta01 == 0)
        inc01 *= -1;
    if(gTheta11 == -45 || gTheta11 == 0)
        inc11 *= -1;
    if(gTheta1 == 225 || gTheta1 == 135)
        inc1 *= -1;
    if(gTheta0 == -225 || gTheta0 == -135)
        inc0 *= -1;
    if(gTheta1 == 225) 
    {
        inc11 = 0;
        gTheta11 = 0;
    }
    if(gTheta1 == 135)
    {
        inc11 = -1; 
        gTheta11 = 0;
    }
    if(gTheta0 == -225) 
    {
        inc01 = -1;
        gTheta01 = 0;
    }
    if(gTheta0 == -135)
    {
        inc01 = 0; 
        gTheta01 = 0;
    }
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
        if(((gX-baseWidth >= platforms[i].GetX()) && 
            (gX+baseWidth <= (platforms[i].GetX() + platforms[i].GetW()))) &&
            // gY - legHeight/4 >= y)
            sqrt(pow((gY + legHeight) - platforms[i].GetY(),2)) <= legHeight)
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
    
    if (hDirection == -1)
    {
        if(!on_move)
        {
            x = baseWidth;
            y = GetY() + baseHeight;
        }
        else if (on_move)
        {
            x = 0;
            y = GetY() + baseHeight;
        }
        // RotatePoint(x, y, armAngle + 270, xa, ya);
        // *fireball = Fireball(xa, ya, armAngle + 270);
        *fireball = Fireball(x, y, armAngle + 270);

    }
    else
    {
        if(!on_move)
        {
            x = baseWidth;
            y = GetY() + baseHeight;
        }
        else if (on_move)
        {
            x = baseHeight;
            y = GetY() + baseHeight;
        }
        // RotatePoint(x, y, -armAngle - 270, xa, ya);
        // *fireball = Fireball(xa, ya, -armAngle - 270);
        *fireball = Fireball(x, y, -armAngle - 270);
    } 
}
bool Enemy::Atingido(Fireball *fireball, GLfloat pos)
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