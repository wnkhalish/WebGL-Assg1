#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <Windows.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <gl/GL.h>

typedef float Matrix4x4[4][4];
Matrix4x4 theMatrix;
float ptsIni[8][3] = {
    {80,80,-100},
    {180,80,-100},
    {180,180,-100},
    {80,180,-100},

    {60,60,0},
    {160,60,0},
    {160,160,0},
    {60,160,0}
};
float ptsIniPy[5][3] = {
    {0,100,0},
    {80,0,80},
    {80,0,-80},
    {-80,0,-80},
    {-80,0,80},
};
float ptsIniCu[8][3] = { //cuboid
    {80,80,-100},
    {280,80,-100},
    {280,180,-100},
    {80,180,-100},

    {60,60,0},
    {260,60,0},
    {260,160,0},
    {60,160,0}
};

//Realign above line while execution
int choiceCube, choicePy, choiceCu, choice, choiceRot, choiceRef;
// Initial Co-ordinates 
float ptsFin[8][3]; //cube
float ptsFinPy[5][3]; // pyramid
float ptsFinCu[8][3]; //cuboid

float refptX, refptY, refptZ;                   //Reference points
float TransDistX, TransDistY, TransDistZ;       //Translations along Axes
float ScaleX, ScaleY, ScaleZ;                   //Scaling Factors along Axes
float Alpha, Beta, Gamma, Theta;                 //Rotation angles about Axes
float A, B, C;                                  //Arbitrary Line Attributes
float aa, bb, cc;                               //Arbitrary Line Attributes
float x1, y11, z1, x2, y2, z2;

// spin
GLfloat spinSpeed = 0;
int spinAxis = 0;

void MyInit()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
}

void Spin()
{
    spinSpeed = spinSpeed + 0.04;
    if (spinSpeed > 360)
        spinSpeed = 0;
    glutPostRedisplay();
}

//spin end

void matrixSetIdentity(Matrix4x4 m)    // Initialises the matrix as Unit Matrix
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            m[i][j] = (i == j);
}

void matrixPreMultiply(Matrix4x4 a, Matrix4x4 b)
{// Multiplies matrix a times b, putting result in b
    int i, j;
    Matrix4x4 tmp;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            tmp[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j] + a[i][3] * b[3][j];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            theMatrix[i][j] = tmp[i][j];
}
void Translate(int tx, int ty, int tz)
{
    Matrix4x4 m;
    matrixSetIdentity(m);
    m[0][3] = tx;
    m[1][3] = ty;
    m[2][3] = tz;
    matrixPreMultiply(m, theMatrix);
}
void Scale(float sx, float sy, float sz)
{
    Matrix4x4 m;
    matrixSetIdentity(m);
    m[0][0] = sx;
    m[0][3] = (1 - sx) * refptX;
    m[1][1] = sy;
    m[1][3] = (1 - sy) * refptY;
    m[2][2] = sz;
    m[2][3] = (1 - sy) * refptZ;
    matrixPreMultiply(m, theMatrix);
}
void RotateX(float angle)
{
    Matrix4x4 m;
    matrixSetIdentity(m);
    angle = angle * 22 / 1260;
    m[1][1] = cos(angle);
    m[1][2] = -sin(angle);
    m[2][1] = sin(angle);
    m[2][2] = cos(angle);
    matrixPreMultiply(m, theMatrix);
}
void RotateY(float angle)
{
    Matrix4x4 m;
    matrixSetIdentity(m);
    angle = angle * 22 / 1260;
    m[0][0] = cos(angle);
    m[0][2] = sin(angle);
    m[2][0] = -sin(angle);
    m[2][2] = cos(angle);
    matrixPreMultiply(m, theMatrix);
}
void RotateZ(float angle)
{
    Matrix4x4 m;
    matrixSetIdentity(m);
    angle = angle * 22 / 1260;
    m[0][0] = cos(angle);
    m[0][1] = -sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
    matrixPreMultiply(m, theMatrix);
}

void DrawRotLine(void)
{
    switch (choiceRot)
    {
    case 1: glBegin(GL_LINES);
        glVertex3s(-1000, B, C);
        glVertex3s(1000, B, C);
        glEnd();
        break;
    case 2: glBegin(GL_LINES);
        glVertex3s(A, -1000, C);
        glVertex3s(A, 1000, C);
        glEnd();
        break;
    case 3: glBegin(GL_LINES);
        glVertex3s(A, B, -1000);
        glVertex3s(A, B, 1000);
        glEnd();
        break;
    case 4: glBegin(GL_LINES);
        glVertex3s(x1 - aa * 500, y11 - bb * 500, z1 - cc * 500);
        glVertex3s(x2 + aa * 500, y2 + bb * 500, z2 + cc * 500);
        glEnd();
        break;
    }
}

void Axes(void)
{
    glColor3f(0.0, 0.0, 0.0);               // Set the color to BLACK
    glBegin(GL_LINES);                       // Plotting X-Axis
    glVertex2s(-1000, 0);
    glVertex2s(1000, 0);
    glEnd();

    glBegin(GL_LINES);                       // Plotting Y-Axis
    glVertex2s(0, -1000);
    glVertex2s(0, 1000);
    glEnd();
}

//============================================Cube=====================================================
//spin cube

void Face(GLfloat A[], GLfloat B[], GLfloat C[], GLfloat D[])
{
    glBegin(GL_POLYGON);
    glVertex3fv(A);
    glVertex3fv(B);
    glVertex3fv(C);
    glVertex3fv(D);
    glEnd();
}

void Cube(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat V4[], GLfloat V5[], GLfloat V6[], GLfloat V7[])
{
    glColor3f(1, 0, 0);
    Face(V0, V1, V2, V3); //Front
    glColor3f(0, 1, 0);
    Face(V4, V5, V6, V7); //Back
    glColor3f(0, 0, 1);
    Face(V0, V4, V7, V3); //Left
    glColor3f(1, 1, 0);
    Face(V1, V5, V6, V2); //Right
    glColor3f(1, 0, 1);
    Face(V2, V3, V7, V6); //Bot
    glColor3f(0, 1, 1);
    Face(V0, V1, V5, V4); //Top
}

void spinCube()
{
    GLfloat V[8][3] = {
                            {-0.5, 0.5, 0.5},
                            { 0.5, 0.5, 0.5},
                            { 0.5,-0.5, 0.5},
                            {-0.5,-0.5, 0.5},

                            {-0.5, 0.5,-0.5},
                            { 0.5, 0.5,-0.5},
                            { 0.5,-0.5,-0.5},
                            {-0.5,-0.5,-0.5},
    };
    GLfloat rV[8][3], r;
    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    r = spinSpeed * 3.14 / 180;

    if (spinAxis == 1)
    {
        for (i = 0; i < 8; i++)
        {
            rV[i][0] = V[i][0];
            rV[i][1] = V[i][1] * cos(r) - V[i][2] * sin(r);
            rV[i][2] = V[i][1] * sin(r) + V[i][2] * cos(r);
        }
    }

    if (spinAxis == 2)
    {
        for (i = 0; i < 8; i++)
        {
            rV[i][0] = V[i][2] * sin(r) + V[i][0] * cos(r);
            rV[i][1] = V[i][1];
            rV[i][2] = V[i][2] * cos(r) - V[i][0] * sin(r);
        }
    }

    if (spinAxis == 3)
    {
        for (i = 0; i < 8; i++)
        {
            rV[i][0] = V[i][0] * cos(r) - V[i][1] * sin(r);
            rV[i][1] = V[i][0] * sin(r) + V[i][1] * cos(r);
            rV[i][2] = V[i][2];
        }
    }

    Cube(rV[0], rV[1], rV[2], rV[3], rV[4], rV[5], rV[6], rV[7]);

    glutSwapBuffers();
}

void TransformPointsCube(void)
{
    int i, k;
    float tmp;
    for (k = 0; k < 8; k++)
        for (i = 0; i < 3; i++)
            ptsFin[k][i] = theMatrix[i][0] * ptsIni[k][0] + theMatrix[i][1] * ptsIni[k][1]
            + theMatrix[i][2] * ptsIni[k][2] + theMatrix[i][3];
    // Realign above line while execution
}

void DrawCube(float a[8][3])                 //Display the Figure
{
    int i;
    glColor3f(0.7, 0.4, 0.7);
    glBegin(GL_POLYGON);
    glVertex3f(a[0][0], a[0][1], a[0][2]);
    glVertex3f(a[1][0], a[1][1], a[1][2]);
    glVertex3f(a[2][0], a[2][1], a[2][2]);
    glVertex3f(a[3][0], a[3][1], a[3][2]);
    glEnd();
    i = 0;
    glColor3f(0.8, 0.6, 0.5);
    glBegin(GL_POLYGON);
    glVertex3s(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3s(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glVertex3s(a[5 + i][0], a[5 + i][1], a[5 + i][2]);
    glVertex3s(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glEnd();
    glColor3f(0.2, 0.4, 0.7);
    glBegin(GL_POLYGON);
    glVertex3f(a[0][0], a[0][1], a[0][2]);
    glVertex3f(a[3][0], a[3][1], a[3][2]);
    glVertex3f(a[7][0], a[7][1], a[7][2]);
    glVertex3f(a[4][0], a[4][1], a[4][2]);
    glEnd();
    i = 1;
    glColor3f(0.5, 0.4, 0.3);
    glBegin(GL_POLYGON);
    glVertex3s(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3s(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glVertex3s(a[5 + i][0], a[5 + i][1], a[5 + i][2]);
    glVertex3s(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glEnd();
    i = 2;
    glColor3f(0.5, 0.6, 0.2);
    glBegin(GL_POLYGON);
    glVertex3s(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3s(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glVertex3s(a[5 + i][0], a[5 + i][1], a[5 + i][2]);
    glVertex3s(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glEnd();
    i = 4;
    glColor3f(0.7, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex3f(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3f(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glVertex3f(a[2 + i][0], a[2 + i][1], a[2 + i][2]);
    glVertex3f(a[3 + i][0], a[3 + i][1], a[3 + i][2]);
    glEnd();
}

void displayCube(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Axes();
    glColor3f(0.0, 1.0, 0.0);                // Set the color to RED
    DrawCube(ptsIni);
    matrixSetIdentity(theMatrix);
    switch (choiceCube)
    {
    case 1:   Translate(TransDistX, TransDistY, TransDistZ);
        break;
    case 2:   Scale(ScaleX, ScaleY, ScaleZ);
        break;
    case 3:   switch (choiceRot)
    {
    case 1: DrawRotLine();
        Translate(0, -B, -C);
        RotateX(Alpha);
        Translate(0, B, C);
        break;
    case 2: DrawRotLine();
        Translate(-A, 0, -C);
        RotateY(Beta);
        Translate(A, 0, C);
        break;
    case 3: DrawRotLine();
        Translate(-A, -B, 0);
        RotateZ(Gamma);
        Translate(A, B, 0);
        break;
    case 4: DrawRotLine();
        float MOD = sqrt((x2 - x1) * (x2 - x1) + (y2 - y11) * (y2 - y11) + (z2 - z1) * (z2 - z1));
        aa = (x2 - x1) / MOD;
        bb = (y2 - y11) / MOD;
        cc = (z2 - z1) / MOD;
        Translate(-x1, -y11, -z1);
        float ThetaDash;
        ThetaDash = 1260 * atan(bb / cc) / 22;
        RotateX(ThetaDash);
        RotateY(1260 * asin(-aa) / 22);
        RotateZ(Theta);
        RotateY(1260 * asin(aa) / 22);
        RotateX(-ThetaDash);
        Translate(x1, y11, z1);
        break;
    }
          break;
    case 4: spinCube();
        break;
    }
    TransformPointsCube();
    DrawCube(ptsFin);
    glFlush();
}



//============================================Pyramid=====================================================
//spin pyramid

void Face(GLfloat A[], GLfloat B[], GLfloat C[])
{
    glBegin(GL_TRIANGLES);
    glVertex3fv(A);
    glVertex3fv(B);
    glVertex3fv(C);
    glEnd();
}


void Cube(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat V4[])
{
    glColor3f(1, 0, 0);
    Face(V0, V1, V3);
    glColor3f(0, 1, 0);
    Face(V0, V2, V3);
    glColor3f(0, 0, 1);
    Face(V0, V2, V4);
    glColor3f(1, 1, 0);
    Face(V0, V4, V1);
    glColor3f(1, 0, 1);
    Face(V2, V3, V1);
    Face(V2, V4, V1);


}

void spinPyramid()
{
    GLfloat V[5][3] = {
                            {0.0, 0.5, 0.0},
                            {-0.5,0.0, 0.5},
                            { 0.5,0.0, -0.5},
                            {0.5,0.0, 0.5},
                            {-0.5, 0.0,-0.5},

    };
    GLfloat rV[5][3], r;
    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    r = spinSpeed * 3.14 / 180;

    if (spinAxis == 1)
    {
        for (i = 0; i < 5; i++)
        {
            rV[i][0] = V[i][0];
            rV[i][1] = V[i][1] * cos(r) - V[i][2] * sin(r);
            rV[i][2] = V[i][1] * sin(r) + V[i][2] * cos(r);
        }
    }

    if (spinAxis == 2)
    {
        for (i = 0; i < 5; i++)
        {
            rV[i][0] = V[i][2] * sin(r) + V[i][0] * cos(r);
            rV[i][1] = V[i][1];
            rV[i][2] = V[i][2] * cos(r) - V[i][0] * sin(r);
        }
    }

    if (spinAxis == 3)
    {
        for (i = 0; i < 5; i++)
        {
            rV[i][0] = V[i][0] * cos(r) - V[i][1] * sin(r);
            rV[i][1] = V[i][0] * sin(r) + V[i][1] * cos(r);
            rV[i][2] = V[i][2];
        }
    }

    Cube(rV[0], rV[1], rV[2], rV[3], rV[4]);

    glutSwapBuffers();
}


void TransformPointsPy(void)
{
    int i, k;
    float tmp;
    for (k = 0; k < 5; k++)
        for (i = 0; i < 3; i++)
            ptsFinPy[k][i] = theMatrix[i][0] * ptsIniPy[k][0] + theMatrix[i][1] * ptsIniPy[k][1]
            + theMatrix[i][2] * ptsIniPy[k][2] + theMatrix[i][3];
    // Realign above line while execution
}


void DrawPy(float a[5][3])                 //Display the Figure
{
    int i;
    glColor3f(0.7, 0.4, 0.7);
    glBegin(GL_TRIANGLES);
    glVertex3f(a[0][0], a[0][1], a[0][2]);
    glVertex3f(a[1][0], a[1][1], a[1][2]);
    glVertex3f(a[2][0], a[2][1], a[2][2]);
    glEnd();
    i = 0;
    glColor3f(0.8, 0.6, 0.5);
    glBegin(GL_TRIANGLES);
    glVertex3s(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3s(a[2 + i][0], a[2 + i][1], a[2 + i][2]);
    glVertex3s(a[3 + i][0], a[3 + i][1], a[3 + i][2]);
    glEnd();
    glColor3f(0.2, 0.4, 0.7);
    glBegin(GL_TRIANGLES);
    glVertex3f(a[0][0], a[0][1], a[0][2]);
    glVertex3f(a[4][0], a[4][1], a[4][2]);
    glVertex3f(a[3][0], a[3][1], a[3][2]);
    glEnd();
    glColor3f(0.5, 0.4, 0.3);
    glBegin(GL_TRIANGLES);
    glVertex3s(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3s(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glVertex3s(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glEnd();

    glColor3f(0.7, 0.3, 0.4);
    glBegin(GL_TRIANGLES);
    glVertex3f(a[2 + i][0], a[2 + i][1], a[2 + i][2]);
    glVertex3f(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glVertex3f(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glEnd();

    glColor3f(0.7, 0.3, 0.4);
    glBegin(GL_TRIANGLES);
    glVertex3s(a[2 + i][0], a[2 + i][1], a[2 + i][2]);
    glVertex3s(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glVertex3s(a[3 + i][0], a[3 + i][1], a[3 + i][2]);
    glEnd();
}

void displayPy(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Axes();
    glColor3f(1.0, 0.0, 0.0);                // Set the color to RED
    DrawPy(ptsIniPy);
    matrixSetIdentity(theMatrix);
    switch (choicePy)
    {
    case 1:   Translate(TransDistX, TransDistY, TransDistZ);
        break;
    case 2:   Scale(ScaleX, ScaleY, ScaleZ);
        break;
    case 3:   switch (choiceRot)
    {
    case 1: DrawRotLine();
        Translate(0, -B, -C);
        RotateX(Alpha);
        Translate(0, B, C);
        break;
    case 2: DrawRotLine();
        Translate(-A, 0, -C);
        RotateY(Beta);
        Translate(A, 0, C);
        break;
    case 3: DrawRotLine();
        Translate(-A, -B, 0);
        RotateZ(Gamma);
        Translate(A, B, 0);
        break;
    case 4: DrawRotLine();
        float MOD = sqrt((x2 - x1) * (x2 - x1) + (y2 - y11) * (y2 - y11) + (z2 - z1) * (z2 - z1));
        aa = (x2 - x1) / MOD;
        bb = (y2 - y11) / MOD;
        cc = (z2 - z1) / MOD;
        Translate(-x1, -y11, -z1);
        float ThetaDash;
        ThetaDash = 1260 * atan(bb / cc) / 22;
        RotateX(ThetaDash);
        RotateY(1260 * asin(-aa) / 22);
        RotateZ(Theta);
        RotateY(1260 * asin(aa) / 22);
        RotateX(-ThetaDash);
        Translate(x1, y11, z1);
        break;
    }
          break;
    case 4:  spinPyramid();
        break;
    }
    TransformPointsPy();
    DrawPy(ptsFinPy);
    glFlush();
}

//============================================Cuboid=====================================================
void Cuboid(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat V4[], GLfloat V5[], GLfloat V6[], GLfloat V7[])
{
    glColor3f(1, 0, 0);
    Face(V0, V1, V2, V3); //Front
    glColor3f(0, 1, 0);
    Face(V4, V5, V6, V7); //Back
    glColor3f(0, 0, 1);
    Face(V0, V4, V7, V3); //Left
    glColor3f(1, 1, 0);
    Face(V1, V5, V6, V2); //Right
    glColor3f(1, 0, 1);
    Face(V2, V3, V7, V6); //Bot
    glColor3f(0, 1, 1);
    Face(V0, V1, V5, V4); //Top
}

void spinCuboid()
{
    GLfloat V[8][3] = {
                            {-0.3, 0.3, 0.3},
                            { 0.7, 0.3, 0.3},
                            { 0.7,-0.3, 0.3},
                            {-0.3,-0.3, 0.3},

                            {-0.3, 0.3,-0.3},
                            { 0.7, 0.3,-0.3},
                            { 0.7,-0.3,-0.3},
                            {-0.3,-0.3,-0.3},
    };
    GLfloat rV[8][3], r;
    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    r = spinSpeed * 3.14 / 180;

    if (spinAxis == 1)
    {
        for (i = 0; i < 8; i++)
        {
            rV[i][0] = V[i][0];
            rV[i][1] = V[i][1] * cos(r) - V[i][2] * sin(r);
            rV[i][2] = V[i][1] * sin(r) + V[i][2] * cos(r);
        }
    }

    if (spinAxis == 2)
    {
        for (i = 0; i < 8; i++)
        {
            rV[i][0] = V[i][2] * sin(r) + V[i][0] * cos(r);
            rV[i][1] = V[i][1];
            rV[i][2] = V[i][2] * cos(r) - V[i][0] * sin(r);
        }
    }

    if (spinAxis == 3)
    {
        for (i = 0; i < 8; i++)
        {
            rV[i][0] = V[i][0] * cos(r) - V[i][1] * sin(r);
            rV[i][1] = V[i][0] * sin(r) + V[i][1] * cos(r);
            rV[i][2] = V[i][2];
        }
    }

    Cuboid(rV[0], rV[1], rV[2], rV[3], rV[4], rV[5], rV[6], rV[7]);

    glutSwapBuffers();
}

void TransformPointsCu(void)
{
    int i, k;
    float tmp;
    for (k = 0; k < 8; k++)
        for (i = 0; i < 3; i++)
            ptsFinCu[k][i] = theMatrix[i][0] * ptsIniCu[k][0] + theMatrix[i][1] * ptsIniCu[k][1]
            + theMatrix[i][2] * ptsIniCu[k][2] + theMatrix[i][3];
    // Realign above line while execution
}

void DrawCu(float a[8][3])                 //Display the Figure
{
    int i;
    glColor3f(0.7, 0.4, 0.7);
    glBegin(GL_POLYGON);
    glVertex3f(a[0][0], a[0][1], a[0][2]);
    glVertex3f(a[1][0], a[1][1], a[1][2]);
    glVertex3f(a[2][0], a[2][1], a[2][2]);
    glVertex3f(a[3][0], a[3][1], a[3][2]);
    glEnd();
    i = 0;
    glColor3f(0.8, 0.6, 0.5);
    glBegin(GL_POLYGON);
    glVertex3s(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3s(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glVertex3s(a[5 + i][0], a[5 + i][1], a[5 + i][2]);
    glVertex3s(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glEnd();
    glColor3f(0.2, 0.4, 0.7);
    glBegin(GL_POLYGON);
    glVertex3f(a[0][0], a[0][1], a[0][2]);
    glVertex3f(a[3][0], a[3][1], a[3][2]);
    glVertex3f(a[7][0], a[7][1], a[7][2]);
    glVertex3f(a[4][0], a[4][1], a[4][2]);
    glEnd();
    i = 1;
    glColor3f(0.5, 0.4, 0.3);
    glBegin(GL_POLYGON);
    glVertex3s(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3s(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glVertex3s(a[5 + i][0], a[5 + i][1], a[5 + i][2]);
    glVertex3s(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glEnd();
    i = 2;
    glColor3f(0.5, 0.6, 0.2);
    glBegin(GL_POLYGON);
    glVertex3s(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3s(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glVertex3s(a[5 + i][0], a[5 + i][1], a[5 + i][2]);
    glVertex3s(a[4 + i][0], a[4 + i][1], a[4 + i][2]);
    glEnd();
    i = 4;
    glColor3f(0.7, 0.3, 0.4);
    glBegin(GL_POLYGON);
    glVertex3f(a[0 + i][0], a[0 + i][1], a[0 + i][2]);
    glVertex3f(a[1 + i][0], a[1 + i][1], a[1 + i][2]);
    glVertex3f(a[2 + i][0], a[2 + i][1], a[2 + i][2]);
    glVertex3f(a[3 + i][0], a[3 + i][1], a[3 + i][2]);
    glEnd();
}

void displayCu(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Axes();
    glColor3f(1.0, 0.0, 0.0);                // Set the color to RED
    DrawCu(ptsIniCu);
    matrixSetIdentity(theMatrix);
    switch (choiceCu)
    {
    case 1:   Translate(TransDistX, TransDistY, TransDistZ);
        break;
    case 2:   Scale(ScaleX, ScaleY, ScaleZ);
        break;
    case 3:   switch (choiceRot)
    {
    case 1: DrawRotLine();
        Translate(0, -B, -C);
        RotateX(Alpha);
        Translate(0, B, C);
        break;
    case 2: DrawRotLine();
        Translate(-A, 0, -C);
        RotateY(Beta);
        Translate(A, 0, C);
        break;
    case 3: DrawRotLine();
        Translate(-A, -B, 0);
        RotateZ(Gamma);
        Translate(A, B, 0);
        break;
    case 4: DrawRotLine();
        float MOD = sqrt((x2 - x1) * (x2 - x1) + (y2 - y11) * (y2 - y11) + (z2 - z1) * (z2 - z1));
        aa = (x2 - x1) / MOD;
        bb = (y2 - y11) / MOD;
        cc = (z2 - z1) / MOD;
        Translate(-x1, -y11, -z1);
        float ThetaDash;
        ThetaDash = 1260 * atan(bb / cc) / 22;
        RotateX(ThetaDash);
        RotateY(1260 * asin(-aa) / 22);
        RotateZ(Theta);
        RotateY(1260 * asin(aa) / 22);
        RotateX(-ThetaDash);
        Translate(x1, y11, z1);
        break;
    }
          break;
    case 4: spinCuboid();
        break;
    }
    TransformPointsCu();
    DrawCu(ptsFinCu);
    glFlush();
}


//============================================Main=====================================================

void init(void)
{
    system("Color F0");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // Set the Background color to WHITE
    glOrtho(-454.0, 454.0, -250.0, 250.0, -250.0, 250.0);
    // Set the no. of Co-ordinates along X & Y axes and their gappings
    glEnable(GL_DEPTH_TEST);
    // To Render the surfaces Properly according to their depths
}
int main(int argc, char* argv)
{
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect); // include windows.h

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 500, 500, TRUE);
    glutInit(&argc, &argv);
    
    init();
    printf("============================MENU==========================\n\n");
    printf(">Horizontal line = X-axis, Vertical line = Y-axis.\n");
    printf("There will have original Object on the display.\n");
    printf("==========================================================\n\n");
    printf("Enter your choice number of shape:\n1.Cube\n2.Pyramid\n3.Cuboid\n4.Special Object\n=>");
    scanf_s("%d", &choice);

    switch (choice)
    {
    case 1: 
        
        printf("\nYou have choose CUBE\n");
        printf("Enter your choice number:\n1.Translation\n2.Scaling\n3.Rotation\n4.Spinning\n=>");
        scanf_s("%d", &choiceCube);
        switch (choiceCube)
        {
        case 1:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter Translation along X, Y & Z\n=>");
            scanf_s("%f%f%f", &TransDistX, &TransDistY, &TransDistZ);
            glutDisplayFunc(displayCube);
            break;
        case 2:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter Scaling ratios along X, Y & Z\n=>");
            scanf_s("%f%f%f", &ScaleX, &ScaleY, &ScaleZ);
            glutDisplayFunc(displayCube);
            break;
        case 3:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter your choice for Rotation about axis:\n1.parallel to X - axis.(y = B & z = C)\n2.parallel to Y - axis.(x = A & z = C)\n3.parallel to   Z - axis.(x = A & y = B)\n4.Arbitrary line passing through(x1, y1, z1) & (x2, y2, z2)\n = > ");
            //Realign above line while execution
            scanf_s("%d", &choiceRot);
            switch (choiceRot)
            {
            case 1:  printf("Enter B & C: ");
                scanf_s("%f %f", &B, &C);
                printf("Enter Rot. Angle Alpha: ");
                scanf_s("%f", &Alpha);
                break;
            case 2:  printf("Enter A & C: ");
                scanf_s("%f %f", &A, &C);
                printf("Enter Rot. Angle Beta: ");
                scanf_s("%f", &Beta);
                break;
            case 3:  printf("Enter A & B: ");
                scanf_s("%f %f", &A, &B);
                printf("Enter Rot. Angle Gamma: ");
                scanf_s("%f", &Gamma);
                break;
            case 4:  printf("Enter values of x1 ,y1 & z1:\n");
                scanf_s("%f %f %f", &x1, &y11, &z1);
                printf("Enter values of x2 ,y2 & z2:\n");
                scanf_s("%f %f %f", &x2, &y2, &z2);
                printf("Enter Rot. Angle Theta: ");
                scanf_s("%f", &Theta);
                break;
            }
            glutDisplayFunc(displayCube);
            break;
        case 4:
            glutInitWindowSize(600, 600);
            glutInitWindowPosition(50, 150);
            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
            glutCreateWindow("3D Spin Animation");
            MyInit();
            printf("Enter the Axis of Rotation Animation[ 1->Xaxis | 2->Yaxis | 3->Zaxis ]: ");
            scanf_s("%d", &spinAxis);
            glutDisplayFunc(spinCube);
            glutIdleFunc(Spin);
            
            break;
        default:    printf("Please enter a valid choice!!!\n");
            return 0;
        }
        
        break;

    case 2: 
        printf("\nYou have choose PYRAMID\n");
        printf("Enter your choice number:\n1.Translation\n2.Scaling\n3.Rotation\n4.Spinning\n=>");
        scanf_s("%d", &choicePy);
        switch (choicePy)
        {
        case 1:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter Translation along X, Y & Z\n=>");
            scanf_s("%f%f%f", &TransDistX, &TransDistY, &TransDistZ);
            glutDisplayFunc(displayPy);
            break;
        case 2:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter Scaling ratios along X, Y & Z\n=>");
            scanf_s("%f%f%f", &ScaleX, &ScaleY, &ScaleZ);
            glutDisplayFunc(displayPy);
            break;
        case 3:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter your choice for Rotation about axis:\n1.parallel to X - axis.(y = B & z = C)\n2.parallel to Y - axis.(x = A & z = C)\n3.parallel to   Z - axis.(x = A & y = B)\n4.Arbitrary line passing through(x1, y1, z1) & (x2, y2, z2)\n = > ");
            //Realign above line while execution
            scanf_s("%d", &choiceRot);
            switch (choiceRot)
            {
            case 1:  printf("Enter B & C: ");
                scanf_s("%f %f", &B, &C);
                printf("Enter Rot. Angle Alpha: ");
                scanf_s("%f", &Alpha);
                break;
            case 2:  printf("Enter A & C: ");
                scanf_s("%f %f", &A, &C);
                printf("Enter Rot. Angle Beta: ");
                scanf_s("%f", &Beta);
                break;
            case 3:  printf("Enter A & B: ");
                scanf_s("%f %f", &A, &B);
                printf("Enter Rot. Angle Gamma: ");
                scanf_s("%f", &Gamma);
                break;
            case 4:  printf("Enter values of x1 ,y1 & z1:\n");
                scanf_s("%f %f %f", &x1, &y11, &z1);
                printf("Enter values of x2 ,y2 & z2:\n");
                scanf_s("%f %f %f", &x2, &y2, &z2);
                printf("Enter Rot. Angle Theta: ");
                scanf_s("%f", &Theta);
                break;
            }
            glutDisplayFunc(displayPy);
            break;
        case 4: 
            glutInitWindowSize(600, 600);
            glutInitWindowPosition(50, 150);
            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
            glutCreateWindow("3D Spin Animation");
            MyInit();
            printf("Enter the Axis of Rotation Animation[ 1->Xaxis | 2->Yaxis | 3->Zaxis ]: ");
            scanf_s("%d", &spinAxis);
            glutDisplayFunc(spinPyramid);
            glutIdleFunc(Spin);
            break;
        default:    printf("Please enter a valid choice!!!\n");
            return 0;
        }
        
        break;

    case 3: printf("\nYou have choose CUBOID\n");
        printf("Enter your choice number:\n1.Translation\n2.Scaling\n3.Rotation\n4.Spinning\n=>");
        scanf_s("%d", &choiceCu);
        switch (choiceCu)
        {
        case 1:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter Translation along X, Y & Z\n=>");
            scanf_s("%f%f%f", &TransDistX, &TransDistY, &TransDistZ);
            glutDisplayFunc(displayCu);
            break;
        case 2:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter Scaling ratios along X, Y & Z\n=>");
            scanf_s("%f%f%f", &ScaleX, &ScaleY, &ScaleZ);
            glutDisplayFunc(displayCu);
            break;
        case 3:
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(1362, 750);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(" Basic Transformations ");
            init();
            printf("Enter your choice for Rotation about axis:\n1.parallel to X - axis.(y = B & z = C)\n2.parallel to Y - axis.(x = A & z = C)\n3.parallel to   Z - axis.(x = A & y = B)\n4.Arbitrary line passing through(x1, y1, z1) & (x2, y2, z2)\n = > ");
            //Realign above line while execution
            scanf_s("%d", &choiceRot);
            switch (choiceRot)
            {
            case 1:  printf("Enter B & C: ");
                scanf_s("%f %f", &B, &C);
                printf("Enter Rot. Angle Alpha: ");
                scanf_s("%f", &Alpha);
                break;
            case 2:  printf("Enter A & C: ");
                scanf_s("%f %f", &A, &C);
                printf("Enter Rot. Angle Beta: ");
                scanf_s("%f", &Beta);
                break;
            case 3:  printf("Enter A & B: ");
                scanf_s("%f %f", &A, &B);
                printf("Enter Rot. Angle Gamma: ");
                scanf_s("%f", &Gamma);
                break;
            case 4:  printf("Enter values of x1 ,y1 & z1:\n");
                scanf_s("%f %f %f", &x1, &y11, &z1);
                printf("Enter values of x2 ,y2 & z2:\n");
                scanf_s("%f %f %f", &x2, &y2, &z2);
                printf("Enter Rot. Angle Theta: ");
                scanf_s("%f", &Theta);
                break;
            }
            glutDisplayFunc(displayCu);
            break;
        case 4: 
            glutInitWindowSize(600, 600);
            glutInitWindowPosition(50, 150);
            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
            glutCreateWindow("3D Spin Animation");
            MyInit();
            printf("Enter the Axis of Rotation Animation[ 1->Xaxis | 2->Yaxis | 3->Zaxis ]: ");
            scanf_s("%d", &spinAxis);
            glutDisplayFunc(spinCuboid);
            glutIdleFunc(Spin);
            break;
        default:    printf("Please enter a valid choice!!!\n");
            return 0;
        }
        
        break;

    case 4: 
        break;

    default:
        return 0;
    }


    glutMainLoop();
    return 0;
}