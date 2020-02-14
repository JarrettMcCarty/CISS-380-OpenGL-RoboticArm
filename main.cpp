// File   : main.cpp
// Author : Jarrett McCarty

#include <iostream>
#include <GL/freeglut.h>
#include "gl3d.h"
#include "View.h"
#include "SingletonView.h"
#include "Light.h"

void draw_arm();
void draw_shoulder_joint();
void draw_elbow_joint();
void draw_upper_arm();
void draw_base();
void draw_lower_arm();
void draw_palm();
void draw_hand();
void draw_finger();
void draw_fingers();
void sphere(double,  double, double);
void disk();
void cylinder(double, double);
void init();
void display();
void reshape();
void keyboard();


// NOTE: For glFrustum, the left, right, top, bottom refers to the near z-plane
float left;
float right;
float top;
float bottom;

float shoulder_xz_angle;
float shoulder_yz_angle;
float elbow_xz_angle;
float elbow_yz_angle;
float wrist_xz_angle;
float finger_angle;

mygllib::Light light;

void init()
{
    glShadeModel(GL_FLAT);
    left = -1;
    right = 1;
    bottom = -1;
    top = 1;
    shoulder_xz_angle,
        shoulder_yz_angle,
        elbow_xz_angle,
        elbow_yz_angle,
        wrist_xz_angle,
        finger_angle = 0.0;

    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    view.eye(15.0f, 4.0f,9.0f);
    view.ref(0.0f, 0.0f, 0.0f);
    /*
    view.eyex() = 3.0f;//2.0f;
    view.eyey() = 1.0f;//2.0f;
    view.eyez() = 3.0f;//5.0f;
    view.refx = 0.0f;
    view.refy = 0.0f;
    view.refz = 0.0f;
    */
    view.zNear() = 1;
    view.zFar() = 100;
    view.set_projection();
    view.lookat();

    glViewport(0, 0, 400, 400);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);
    mygllib::Material mat(mygllib::Material::BRASS);
    mat.set();
    glShadeModel(GL_SMOOTH);

    light.set();
    mygllib::Light::all_on();
    light.on(); 

    glEnable(GL_NORMALIZE);
}

/*
void triangle()
{
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1,0,0); glVertex3f(0,0,0);
        glColor3f(0,1,0); glVertex3f(1,0,0);
        glColor3f(0,0,1); glVertex3f(0.5,1,0);
    }
    glEnd();
}
*/

void sphere(double radius=1.0,
            double p1=20,
            double p2=20)
{
    glutSolidSphere(radius, p1, p2);
}

void disk()
{
    GLUquadric * p = gluNewQuadric();
    gluQuadricDrawStyle(p, GLU_FILL);
    gluDisk(p, 0, 0.1, 20, 20);
    gluDeleteQuadric(p);
}

void cylinder(double radius=0.5, double length=4)
{
    glPushMatrix();
    {
        glRotatef(-90, 1, 0, 0); 
        GLUquadric * p = gluNewQuadric();
        gluQuadricDrawStyle(p, GLU_FILL); 
        gluCylinder(p, radius, radius, length, 20, 20);
        gluDeleteQuadric(p);
    }
    glPopMatrix();
}

void draw_base()
{
    glPushMatrix();
    {
        glScalef(-2, 0.1, -2);
        glutSolidCube(2.0);
    }
    glPopMatrix();
}

void draw_shoulder_joint()
{
    sphere(); 
}

void draw_upper_arm()
{
    cylinder();
}

void draw_lower_arm()
{
    cylinder();
}

void draw_elbow_joint()
{
    glPushMatrix();
    {
        glTranslatef(0, 4, 0);
        sphere();
    }
    glPopMatrix();
}

void draw_palm()
{
    glutSolidCube(1);
}

void draw_hand()
{
    glPushMatrix();
    {
        draw_palm();
        glTranslatef(0, 0.6, 0);
        draw_fingers();
    }
    glPopMatrix();
}

void draw_finger()
{
    glPushMatrix();
    {
        sphere(0.15);
        glRotatef(-45, 1, 0, 0);
        cylinder(0.1, 0.5);
        glTranslatef(0, 0.5, 0);
        sphere(0.15);
        glRotatef(90, 1, 0, 0);
        cylinder(0.1, 0.5);
        glTranslatef(0, 0.5, 0);
        glRotatef(90, 1, 0, 0);
        disk();
    }
    glPopMatrix();
}

void draw_fingers()
{
    glPushMatrix();
    {
        glTranslatef(0.3, 0, -0.2);
        glRotatef(finger_angle, 1, 0, 0);
        draw_finger();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-0.3, 0, -0.2);
        glRotatef(finger_angle, 1, 0, 0);
        draw_finger();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(0, 0, 0.3);
        glRotatef(180, 0, 1, 0);
        glRotatef(finger_angle, 1, 0, 0);
        draw_finger();
    }
    glPopMatrix();
}

void draw_arm()
{
    glPushMatrix();
    {
        draw_base();
        glPushMatrix();
        {
            glRotatef(shoulder_xz_angle, 0, 1, 0);
            draw_shoulder_joint();
            glPushMatrix();
            {
                glRotatef(shoulder_yz_angle, 1, 0, 0);
                draw_upper_arm();
                glPushMatrix();
                {
                    glRotatef(elbow_xz_angle, 0, 1, 0);
                    draw_elbow_joint();
                    glPushMatrix();
                    {
                        glTranslatef(0, 4, 0);
                        glRotatef(elbow_yz_angle, 1, 0, 0);
                        draw_lower_arm();
                        glPushMatrix();
                        {
                            glTranslatef(0, 4, 0);
                            glRotatef(wrist_xz_angle, 0, 1, 0);
                            draw_hand();
                        }
                        glPopMatrix();
                    }
                    glPopMatrix();
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // NOTE: Make sure depth data is cleared
    mygllib::Light::all_off();
    mygllib::draw_xz_plane();
    mygllib::draw_axes();
    mygllib::Light::all_on();
    
    draw_arm();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, 400, 400);
}

void keyboard(unsigned char key, int x, int y)
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    switch(key)
    {
        case 'x': view.eyex() -= 0.1; break;
        case 'X': view.eyex() += 0.1; break;
        case 'y': view.eyey() -= 0.1; break;
        case 'Y': view.eyey() += 0.1; break;
        case 'z': view.eyez() -= 0.1; break;
        case 'Z': view.eyez() += 0.1; break;
        case 'v': view.fovy() -= 0.1; break;
        case 'V': view.fovy() += 0.1; break;
        case 'a': view.aspect() -= 0.1; break;
        case 'A': view.aspect() += 0.1; break;
        case 'n': view.zNear() -= 0.1; break;
        case 'N': view.zNear() += 0.1; break;
        case 'f': view.zFar() -= 0.1; break;
        case 'F': view.zFar() += 0.1; break;

        case 'l': left -= 0.1; break;
        case 'L': left += 0.1; break;
        case 'r': right -= 0.1; break;
        case 'R': right += 0.1; break;
        case 't': top -= 0.1; break;
        case 'T': top += 0.1; break;
        case 'b': bottom -= 0.1; break;
        case 'B': bottom += 0.1; break;
        case '1': shoulder_xz_angle += 1.0; break;
        case '2': shoulder_xz_angle -= 1.0; break;
        case '3': shoulder_yz_angle += 1.0; break;
        case '4': shoulder_yz_angle -= 1.0; break;
        case '5': elbow_xz_angle += 1.0; break;
        case '6': elbow_xz_angle -= 1.0; break;
        case '7': elbow_yz_angle += 1.0; break;
        case '8': elbow_yz_angle -= 1.0; break;
        case '9': wrist_xz_angle -= 1.0; break;
        case '0': wrist_xz_angle += 1.0; break;
        case '`': finger_angle -= 1.0; break;
        case '~': finger_angle += 1.0; break;
            
        // light position
        case '!': light.x() -= 1.0; break;
        case '@': light.x() += 1.0; break;
        case '#': light.y() -= 1.0; break;
        case '$': light.y() += 1.0; break;
        case '%': light.z() -= 1.0; break;
        case '^': light.z() += 1.0; break;
        // Turning light on and off
        case '&': light.on(); break;
        case '*': light.off(); break;
    }
    
    view.set_projection();
    view.lookat();
    light.set_position();
    glutPostRedisplay();
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("ROBO ARM");
    glEnable(GL_DEPTH_TEST); // NOTE: make sure depth testing is on.
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}


// old code
/*
// Set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right,
              bottom, top,
              zNear, zFar);
// Set model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez,
              refx, refy, refz,
              0.0, 1.0, 0.0);




*/
