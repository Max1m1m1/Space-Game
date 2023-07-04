/*********
   CTIS164 - Template Source Program
----------
STUDENT : Ozan GUNGOR
SECTION : 4
HOMEWORK: 3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:


    While the cursor is in the area, you are able to throw bows by left click, they are going to explode at the screen boundaries.

*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; bool crsDriven = false;
bool crsHover = false; // current Window width and height

typedef struct crossbow_t {
    vec_t anchor;
    int anglor;
    vec_t movement;
} crossbow_t;

typedef struct bow_t {
    vec_t anchor;
    vec_t movement;
    int anglor;
    int blast;
    bool active;
}bow_t;

bow_t bows[15] = { {-9999,-9999}, {0,0}, 0, 0 };

vec_t crs = {
    0, 0
};

crossbow_t crossbow = {
    {0,0},
    0
};


int spinX(int x, int y, int anglor) {
    return
        x * cos(anglor * D2R) - y * sin(anglor * D2R);
}
int spinY(int x, int y, int anglor) {
    return
        y * cos(anglor * D2R) + x * sin(anglor * D2R);
}






//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void crossbowDrawer(vec_t anchor, int anglor) {
    glColor3ub(255, 0, 0);
    circle(spinX(120, 0, anglor) + anchor.x, spinY(120, 0, anglor) + anchor.y, 20);

    glColor3ub(124, 227, 227);
    glBegin(GL_TRIANGLES);
    glVertex2f(spinX(0, 0, anglor) + anchor.x, spinY(0, 0, anglor) + anchor.y);
    glVertex2f(spinX(120, 20, anglor) + anchor.x, spinY(120, 20, anglor) + anchor.y);
    glVertex2f(spinX(120, -20, anglor) + anchor.x, spinY(120, -20, anglor) + anchor.y);
    glEnd();

    glColor3ub(0, 227, 227);
    glBegin(GL_TRIANGLES);
    glVertex2f(spinX(100, -40, anglor) + anchor.x, spinY(100, -40, anglor) + anchor.y);
    glVertex2f(spinX(20, 0, anglor) + anchor.x, spinY(20, 0, anglor) + anchor.y);
    glVertex2f(spinX(100, -20, anglor) + anchor.x, spinY(100, -20, anglor) + anchor.y);
    glEnd();

    glColor3ub(123, 0, 227);
    glBegin(GL_TRIANGLES);
    glVertex2f(spinX(100, 40, anglor) + anchor.x, spinY(100, 40, anglor) + anchor.y);
    glVertex2f(spinX(100, 20, anglor) + anchor.x, spinY(100, 20, anglor) + anchor.y);
    glVertex2f(spinX(20, 0, anglor) + anchor.x, spinY(20, 0, anglor) + anchor.y);
    glEnd();

    glColor3ub(0, 255, 255);
    glBegin(GL_TRIANGLES);
    glVertex2f(spinX(100, 40, anglor) + anchor.x, spinY(100, 40, anglor) + anchor.y);
    glVertex2f(spinX(100, 40, anglor) + anchor.x, spinY(100, 40, anglor) + anchor.y);
    glVertex2f(spinX(100, -40, anglor) + anchor.x, spinY(100, -40, anglor) + anchor.y);
    glEnd();


    circle(spinX(100, 60, anglor) + anchor.x, spinY(100, 60, anglor) + anchor.y, 7);


    circle(spinX(100, 40, anglor) + anchor.x, spinY(100, 40, anglor) + anchor.y, 7);


    circle(spinX(100, -40, anglor) + anchor.x, spinY(100, -40, anglor) + anchor.y, 7);


    circle(spinX(100, -60, anglor) + anchor.x, spinY(100, -60, anglor) + anchor.y, 7);

    glColor3ub(115, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(spinX(100, -60, anglor) + anchor.x, spinY(100, -60, anglor) + anchor.y);
    glVertex2f(spinX(120, -20, anglor) + anchor.x, spinY(120, -20, anglor) + anchor.y);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(spinX(100, -40, anglor) + anchor.x, spinY(100, -40, anglor) + anchor.y);
    glVertex2f(spinX(140, 0, anglor) + anchor.x, spinY(140, 0, anglor) + anchor.y);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(spinX(100, 40, anglor) + anchor.x, spinY(100, 40, anglor) + anchor.y);
    glVertex2f(spinX(140, 0, anglor) + anchor.x, spinY(140, 0, anglor) + anchor.y);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(spinX(100, 60, anglor) + anchor.x, spinY(100, 60, anglor) + anchor.y);
    glVertex2f(spinX(120, 20, anglor) + anchor.x, spinY(120, 20, anglor) + anchor.y);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(spinX(140, 0, anglor) + anchor.x, spinY(140, 0, anglor) + anchor.y);
    glVertex2f(spinX(20, 0, anglor) + anchor.x, spinY(20, 0, anglor) + anchor.y);
    glEnd();

}


void bow_flush(vec_t pos, int angle, int blast) {
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(spinX(0, 0, angle) + pos.x, spinY(0, 0, angle) + pos.y);
    glVertex2f(spinX(80, 0, angle) + pos.x, spinY(80, 0, angle) + pos.y);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(spinX(80, 20, angle) + pos.x, spinY(80, 20, angle) + pos.y);
    glVertex2f(spinX(100, 0, angle) + pos.x, spinY(100, 0, angle) + pos.y);
    glVertex2f(spinX(80, -20, angle) + pos.x, spinY(80, -20, angle) + pos.y);
    glEnd();
    glColor4f(0, 0, 0, 0.4);
    if (blast > 0)
        circle(pos.x, pos.y, blast * 10);
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(255, 0, 20, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 1, 0);
    glRectf(400, 200, -400, -200);

    glColor3f(1, 1, 1);

    for (int i = -300; i <= 300; i += 20) {
        vprint(-500, i, GLUT_BITMAP_9_BY_15, "OZAN GUNGOR");
    }

    for (int i = -300; i <= 300; i += 20) {
        vprint2(400, i, 0.08, "SPACEBARTO%s", crsDriven ? "STOP" : "START");
    }


    vprint2(-200, 200, 0.5, "ANGLE : %d", crossbow.anglor);

    vprint2(-200, -250, 0.5, "CURSOR : %s", crsHover ? "ON" : "OFF");


    crossbowDrawer(crossbow.anchor, crossbow.anglor);
    for (int i = 0; i < 15; i++) {
        if (bows[i].active) {
            bow_flush(bows[i].anchor, bows[i].anglor, bows[i].blast);
        }
    }


    if (crsHover) {
        glColor4f(1, 1, 0, 0.5);
        circle(crs.x, crs.y, 10);
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    else if (key == ' ')
        crsDriven = !crsDriven;
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && crsDriven) {
        for (int i = 0; i < 15; i++) {
            if (!bows[i].active) {
                bows[i].active = true;
                bows[i].anchor = crossbow.anchor;
                bows[i].movement = mulV(4, crossbow.movement);
                bows[i].anglor = crossbow.anglor;
                break;
            }
        }
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.


    int outx = x - winWidth / 2;
    int outy = winHeight / 2 - y;

    crs.x = outx;
    crs.y = outy;
    crsHover = fabs(crs.x) <= 400 && fabs(crs.y) <= 200;
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (crsDriven) {


        for (int i = 0; i < 15; i++) {
            if (bows[i].active) {

                if (bows[i].blast == 0) {
                    bows[i].anchor = addV(bows[i].anchor, bows[i].movement);
                    if (fabs(bows[i].anchor.x) > 400 || fabs(bows[i].anchor.y) > 200) {
                        bows[i].blast = 10;
                    }
                }
                else {
                    bows[i].blast--;
                    if (bows[i].blast == 0) {
                        bows[i] = { {-9999,-9999}, {0,0}, 0, 0 };
                    }
                }
            }
        }




        if (crsHover) {
            vec_t subt = subV(crs, crossbow.anchor);
            crossbow.movement = unitV(subt);
            crossbow.anchor = addV(crossbow.anchor, crossbow.movement);
            polar_t angleVec = rec2pol(crossbow.movement);
            crossbow.anglor = angleVec.angle;
        }

        else {
            crossbow.anchor = addV(crossbow.anchor, crossbow.movement);
            if (fabs(crossbow.anchor.x) > 340) {
                crossbow.movement.x *= -1;
                crossbow.anglor = (180 - crossbow.anglor) % 360;
            }
            if (fabs(crossbow.anchor.y) > 140) {
                crossbow.movement.y *= -1;
                crossbow.anglor = (360 - crossbow.anglor) % 360;
            }
        }
    }


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Ozan Gungor / Crossbower / H3");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}