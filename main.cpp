#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <atomic>
#include "cmath"

#define A M_PI_2
#define B M_PI_2
#define DELTA M_PI_2
#define FPS 25
#define SKIPPED_FRAMES_TO_UPDATE 0.5
#define DT 0.01

double X_PARAM = 3;
double Y_PARAM = 4;
double TIME = 0;
uint FRAME_COUNTER = 0;
double ANIMATION_SPEED = 1;

struct Point {
    double x;
    double y;

    Point(double x, double y) : x(x), y(y) {}
};

std::vector<Point> points;

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (Point p: points) {
        glVertex2d(p.x, p.y);
    }
    glEnd();
    glutSwapBuffers();

}


void generate_frame_update() {
    double x = A * sin(X_PARAM * TIME + DELTA);
    double y = B * sin(Y_PARAM * TIME);
//    printf("X: %f Y: %f\n", x/2, y/2);
    points.emplace_back(x / 2, y / 2);
    TIME += DT;
    if (TIME > 5) {
        TIME = 0;
        points.clear();
    }
}

void update_frames(int){
    if(ANIMATION_SPEED*(FRAME_COUNTER++) > SKIPPED_FRAMES_TO_UPDATE){
        int frames_to_update = (int) ceil(ANIMATION_SPEED*FRAME_COUNTER / SKIPPED_FRAMES_TO_UPDATE);
        printf("FTU: %d AS: %f\n", frames_to_update, ANIMATION_SPEED);
        for(int i=0;i<frames_to_update;i++) generate_frame_update();
        FRAME_COUNTER=0;
    }
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, update_frames, 0);
}

void init() {
    glPointSize(5.0f);
}

void keyboard_handler(u_char key, int x, int y){
    printf("KEY WAS PRESSED\n");
    switch (key) {
        case '-':
            ANIMATION_SPEED /= 2.0;
            break;
        case '+':
            ANIMATION_SPEED *= 2.0;
            break;
        default:
            ANIMATION_SPEED = 1.0;
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard_handler);
    init();
    update_frames(0);
    glutMainLoop();
    return 0;
}