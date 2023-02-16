#include <GL/glut.h>
#include <utility>
#include <vector>
#include <iostream>
#include <random>
#include "cmath"

#define DELTA M_PI_2
#define FPS 25
#define SKIPPED_FRAMES_TO_UPDATE 0.125
#define DT 0.005
#define CURVE_LENGTH 100
#define ANIMATION_LENGTH 100
#define NUM_OF_CURVES 200
#define ROTATION_SPEED 0.5


#define SCALE 15.0

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

struct Curve {
    std::vector<Point> points;
    double A,B,C;
    double x_param, y_param, z_param;
    short r, g, b;

    Curve(std::vector<Point> points, double A, double B, double C, double x, double y, double z, short r, short g, short b)
        : points(std::move(points)), A(A), B(B), C(C), x_param(x), y_param(y), z_param(z), r(r), g(g), b(b){}
    Curve(double A, double B, double C, double x, double y, double z, short r, short g, short b)
        : A(A), B(B), C(C), x_param(x), y_param(y), z_param(z), r(r), g(g), b(b){
        Curve::points = std::vector<Point>();
    }
};

std::vector<Curve> LISSAJOUS_CURVES{
        Curve(M_PI_4, M_PI_4, M_PI_4, 7, 8, 9, 255, 0, 0),
        Curve(M_PI_4, M_PI_4, M_PI_4, 7, 9, 8, 0, 255, 0),
        Curve(M_PI_4, M_PI_4, M_PI_4, 8, 7, 9, 0, 0, 255),
        Curve(M_PI_4, M_PI_4, M_PI_4, 8, 9, 7, 0, 255, 255),
        Curve(M_PI_4, M_PI_4, M_PI_4, 9, 8, 7, 255, 255, 255),
        Curve(M_PI_4, M_PI_4, M_PI_4, 9, 7, 8, 255, 0, 255),
};

std::vector<Curve> LORENZ_CURVES;
std::vector<Curve> CURVES;

void draw_curves() {
    float alpha_inc = 1.0f/CURVE_LENGTH;
    for(const auto& curve: CURVES){
        float alpha = alpha_inc*2;
        glBegin(GL_LINE_STRIP);
        for (Point p: curve.points){
            glColor4f(curve.r/255.0f, curve.g/255.0f, curve.b/255.0f, alpha);
            glVertex3d(p.x / SCALE, p.y / SCALE, p.z / SCALE);
            alpha+=alpha_inc;
        }
        glEnd();
    }
}

void draw_cube(){
    float size = 3.0f;
    glColor3f(1,1,1);
    glBegin( GL_LINES );
    glVertex3f( size, size, size );
    glVertex3f( size, - size, size );
    glVertex3f( size, - size, size );
    glVertex3f( size, - size, - size );
    glVertex3f( size, - size, - size );
    glVertex3f( size, size, - size );
    glVertex3f( size, size, - size );
    glVertex3f( size, size, size );
    glVertex3f( - size, size, size );
    glVertex3f( - size, - size, size );
    glVertex3f( - size, - size, size );
    glVertex3f( - size, - size, - size );
    glVertex3f( - size, - size, - size );
    glVertex3f( - size, size, - size );
    glVertex3f( - size, size, - size );
    glVertex3f( - size, size, size );
    glVertex3f( size, size, size );
    glVertex3f( - size, size, size );
    glVertex3f( size, - size, size );
    glVertex3f( - size, - size, size );
    glVertex3f( size, - size, - size );
    glVertex3f( - size, - size, - size );
    glVertex3f( size, size, - size );
    glVertex3f( - size, size, - size );
    glEnd();
}


void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -7.0f);
    glRotatef(ROTATION_ANGLE, 0, 1.0f, 0);
    draw_curves();
    draw_cube();
    glutSwapBuffers();
    ROTATION_ANGLE += ROTATION_SPEED;
}

Point create_lissajous_point(Curve const& curve){
    double x = curve.A * sin(curve.x_param * TIME + DELTA);
    double y = curve.B * sin(curve.y_param * TIME);
    double z = curve.C * sin(curve.z_param * TIME);
    return {x, y, z};
}

Point create_lorenz_point(Curve const& curve){
    Point last_point = curve.points.back();
    double dx = curve.A*(last_point.y-last_point.x);
    double dy = last_point.x*(curve.B-last_point.z)-last_point.y;
    double dz = last_point.x*last_point.y - curve.C*last_point.z;
    double x = last_point.x+dx*DT;
    double y = last_point.y+dy*DT;
    double z = last_point.z+dz*DT;
    return {x, y, z};
}


void generate_frame_update(std::vector<Curve>& curves) {
    for(auto& curve: curves){
//        curve.points.push_back(create_lissajous_point(curve));
        curve.points.push_back(create_lorenz_point(curve));
        if(curve.points.size() > CURVE_LENGTH) curve.points.erase(curve.points.begin());
    }
    TIME += DT;
    if (TIME > ANIMATION_LENGTH) {
        TIME = 0;
        for(auto& curve:curves) curve.points.erase(curve.points.begin()+1, curve.points.end());
    }
}

void update_frames(int) {
    if (ANIMATION_SPEED * (FRAME_COUNTER++) > SKIPPED_FRAMES_TO_UPDATE) {
        int frames_to_update = (int) ceil(ANIMATION_SPEED * FRAME_COUNTER / SKIPPED_FRAMES_TO_UPDATE);
        for (int i = 0; i < frames_to_update; i++) generate_frame_update(CURVES);
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

void fill_lorenz_curves(){
    double x = -0.1;
    double y = -0.2;
    double z = -0.3;
    for(int i=0;i<NUM_OF_CURVES;i++){
        short r = rand()%255;
        short g = rand()%255;
        short b = rand()%255;
        LORENZ_CURVES.push_back(
        Curve(std::vector<Point>{Point(x, y, z)}, 10.0, 28.0, 8/3.0, 0, 0, 0, r, g, b)
        );
        x+=0.1;
        y+=0.1;
        z+=0.1;
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    CURVES = LORENZ_CURVES;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lorenz Attractor");
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard_handler);
    glutReshapeFunc(reshape);
    fill_lorenz_curves();
    init();
    update_frames(0);
    glutMainLoop();
    return 0;
}