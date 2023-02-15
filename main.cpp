#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "cmath"

#define A M_PI_2
#define B M_PI_2
#define C M_PI_2
#define DELTA M_PI_2
#define FPS 25
#define SKIPPED_FRAMES_TO_UPDATE 0.125
#define DT 0.01

double X_PARAM = 3;
double Y_PARAM = 4;
double Z_PARAM = 5;
double TIME = 0;
uint FRAME_COUNTER = 0;
double ANIMATION_SPEED = 1;
GLfloat ROTATION_ANGLE = 0.0;

struct Point {
    double x;
    double y;
    double z;

    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

std::vector<Point> points;

void draw_curves() {
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (Point p: points) glVertex3d(p.x, p.y, p.z);
    glEnd();
}

void draw_cube(){

    glColor3f(1, 1, 1);
    glBegin( GL_LINES );
    glVertex3f( 1.0, 1.0, 1.0 );
    glVertex3f( 1.0, - 1.0, 1.0 );
    glVertex3f( 1.0, - 1.0, 1.0 );
    glVertex3f( 1.0, - 1.0, - 1.0 );
    glVertex3f( 1.0, - 1.0, - 1.0 );
    glVertex3f( 1.0, 1.0, - 1.0 );
    glVertex3f( 1.0, 1.0, - 1.0 );
    glVertex3f( 1.0, 1.0, 1.0 );
    glVertex3f( - 1.0, 1.0, 1.0 );
    glVertex3f( - 1.0, - 1.0, 1.0 );
    glVertex3f( - 1.0, - 1.0, 1.0 );
    glVertex3f( - 1.0, - 1.0, - 1.0 );
    glVertex3f( - 1.0, - 1.0, - 1.0 );
    glVertex3f( - 1.0, 1.0, - 1.0 );
    glVertex3f( - 1.0, 1.0, - 1.0 );
    glVertex3f( - 1.0, 1.0, 1.0 );
    glVertex3f( 1.0, 1.0, 1.0 );
    glVertex3f( - 1.0, 1.0, 1.0 );
    glVertex3f( 1.0, - 1.0, 1.0 );
    glVertex3f( - 1.0, - 1.0, 1.0 );
    glVertex3f( 1.0, - 1.0, - 1.0 );
    glVertex3f( - 1.0, - 1.0, - 1.0 );
    glVertex3f( 1.0, 1.0, - 1.0 );
    glVertex3f( - 1.0, 1.0, - 1.0 );
    // koniec definicji prymitywu
    glEnd();
}


void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -7.0f);
    glRotatef(ROTATION_ANGLE, 0, 1.0f, 0);
    draw_curves();
    draw_cube();
    glutSwapBuffers();
    ROTATION_ANGLE += 1.0;
}


void generate_frame_update() {
    double x = A * sin(X_PARAM * TIME + DELTA);
    double y = B * sin(Y_PARAM * TIME);
    double z = C * sin(Z_PARAM * TIME);
    points.emplace_back(x / 2, y / 2, z / 2);
    TIME += DT;
    if (TIME > 8) {
        TIME = 0;
        points.clear();
    }
}

void update_frames(int) {
    if (ANIMATION_SPEED * (FRAME_COUNTER++) > SKIPPED_FRAMES_TO_UPDATE) {
        int frames_to_update = (int) ceil(ANIMATION_SPEED * FRAME_COUNTER / SKIPPED_FRAMES_TO_UPDATE);
        for (int i = 0; i < frames_to_update; i++) generate_frame_update();
        FRAME_COUNTER = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, update_frames, 0);
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat) width / (GLfloat) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void keyboard_handler(u_char key, int x, int y) {
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
    glutReshapeFunc(reshape);
    init();
    update_frames(0);
    glutMainLoop();
    return 0;
}