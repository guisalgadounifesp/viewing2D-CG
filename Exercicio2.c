// Guilherme Salgado Alves 
// RA: 140083

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int windowWidth = 900, windowHeight = 900, clickCount = 1;
float startPointX = 1, startPointY = 1, endPointX = 0, endPointY = 0;
float xViewMin = -0.5, xViewMax = 0.5, yViewMin = -0.5, yViewMax = 0.5;

void drawOutline()
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(xViewMin, yViewMax);
    glVertex2f(xViewMax, yViewMax);
    glVertex2f(xViewMax, yViewMin);
    glVertex2f(xViewMin, yViewMin);
    glEnd();
}

void drawLineSegment()
{
    glBegin(GL_LINES);
    float x1 = startPointX;
    float x2 = endPointX;
    float y1 = startPointY;
    float y2 = endPointY;
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void determinePointPosition(float x, float y, int *positionArray)
{
    if (y > yViewMax)
        positionArray[0] = 1;
    else
        positionArray[0] = 0;

    if (y < yViewMin)
        positionArray[1] = 1;
    else
        positionArray[1] = 0;

    if (x > xViewMax)
        positionArray[2] = 1;
    else
        positionArray[2] = 0;

    if (x < xViewMin)
        positionArray[3] = 1;
    else
        positionArray[3] = 0;
}


void adjustPointPosition(float slope, float *x, float *y, int positionArray[])
{
    if (positionArray[0])
    {
        *x = *x + (yViewMax - *y) / slope;
        *y = yViewMax;
    }
    if (positionArray[1])
    {
        *x = *x + (yViewMin - *y) / slope;
        *y = yViewMin;
    }
    if (positionArray[2])
    {
        *y = *y + slope * (xViewMax - *x);
        *x = xViewMax;
    }
    if (positionArray[3])
    {
        *y = *y + slope * (xViewMin - *x);
        *x = xViewMin;
    }
}



void checkAndDraw()
{
    int positionStartPoint[4], positionEndPoint[4], positionArray[4];
    determinePointPosition(startPointX, startPointY, positionStartPoint);
    determinePointPosition(endPointX, endPointY, positionEndPoint);

    for (int i = 0; i < 4; i++)
    {
        if (positionStartPoint[i] == positionEndPoint[i])
        {
            if (positionStartPoint[i] == 1)
                return;
            else
                positionArray[i] = 0;
        }
        else
        {
            positionArray[i] = 1;
        }
    }

    int positionSum = positionArray[0] + positionArray[1] + positionArray[2] + positionArray[3];
    if (positionSum == 0)
    {
        drawLineSegment();
    }
    else
    {
        float dx = endPointX - startPointX;
        float dy = endPointY - startPointY;
        float slope = dy / dx;
        adjustPointPosition(slope, &startPointX, &startPointY, positionStartPoint);
        adjustPointPosition(slope, &endPointX, &endPointY, positionEndPoint);
        drawLineSegment();
    }
}

void mouseCallback(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            if (clickCount == 0)
            {
                startPointX = 2 * (x / (float)windowWidth) - 1;
                startPointY = (2 * (y / (float)windowHeight) - 1) * (-1);
                clickCount = 1;
            }
            else if (clickCount)
            {
                endPointX = 2 * (x / (float)windowWidth) - 1;
                endPointY = (2 * (y / (float)windowHeight) - 1) * (-1);
                clickCount = 0;
            }
        }
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void displayFunction()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glColor3f(0.0f, 1.0f, 0.0f);
    drawOutline();
    glColor3f(1.0f, 0.0f, 0.0f);
    if (clickCount == 0)
        checkAndDraw();

    glutSwapBuffers();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("2D Viewing");
    glutDisplayFunc(displayFunction);
    glutMouseFunc(mouseCallback);
    init();
    glutMainLoop();
    return 0;
}
