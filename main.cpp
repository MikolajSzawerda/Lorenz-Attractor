#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "cmath"

#define A M_PI_2
#define B M_PI_2
#define DELTA M_PI_2

double X_PARAM = 1;
double Y_PARAM = 2;
double TIME = 0;
int LAST_UPDATE = 0;
double ANIMATION_SPEED = 1;

struct Point {
    double x;
    double y;

    Point(double x, double y) : x(x), y(y) {}
};

std::vector<Point> points;

void displayMe() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (Point p: points) {
        glVertex2d(p.x, p.y);
    }
    glEnd();
    glutSwapBuffers();

}


void main_loop() {
    double x = A * sin(X_PARAM * TIME + DELTA);
    double y = B * sin(Y_PARAM * TIME);
    int dt = glutGet(GLUT_ELAPSED_TIME) - LAST_UPDATE;
    LAST_UPDATE += dt;
    points.emplace_back(x / 2, y / 2);
    TIME += ANIMATION_SPEED * (dt/ 1000.0);
    if (TIME > 10) {
        TIME = 0;
        points.clear();
    }
    glutPostRedisplay();
}


void init() {
    glPointSize(5.0f);
}

void keyboard_handler(u_char key, int x, int y){
    printf("KEY WAS PRESSED\n");
    switch (key) {
        case '-':
            ANIMATION_SPEED /= 2;
            break;
        case '+':
            ANIMATION_SPEED *= 2;
            break;
        default:
            ANIMATION_SPEED = 1;
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");
    glutDisplayFunc(displayMe);
    glutIdleFunc(main_loop);
    glutKeyboardFunc(keyboard_handler);
    init();
    glutMainLoop();
    return 0;
}