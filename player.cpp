#include "player.h"
#include <math.h>
#include <stdio.h>

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

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
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

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
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

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
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

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
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

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
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

        crossPlayer(a,b,norm);
        normalizePlayer(norm);
        glNormal3f(norm[0], norm[1], norm[2]);
        glVertex3f (-width/2, 0, 0.0);  // Lado Esquerdo Cima
		glVertex3f (width/2, 0, 0.0); // Lado Direito Cima
		glVertex3f (width/2, height, 0.0); // Lado Direito Baixo
		glVertex3f (-width/2, height, 0.0);  // Lado Esquerdo Baixo
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
            glTranslatef(0,y,-7);
        else
            glTranslatef(0,y,-9);
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
    if(ground < gY-legHeight)
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

void Player::MoveInX(GLfloat dx, GLdouble deltaTime)
{
    gX += dx*percentual*deltaTime/15;
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