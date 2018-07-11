/*
* Mostly borrowed from http://lazyfoo.net/tutorials/OpenGL/01_hello_opengl/index2.php
* and from https://github.com/joshb/glsl_lighting
*/

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
//#include <GL/gl.h>
//#include <GL/glext.h>
//#include <GL/glu.h>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

bool initGL();
void update();
void render();
void mainLoop(int);
bool compileShader();

int main(int argc, char* args[])
{
    std::cout << "shader system version 0.1" << std::endl << "by tähtituho 2018" << std::endl;

    glutInit(&argc, args);
    glutInitContextVersion(2, 1);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("shader system");

    if(!initGL())
    {
        std::cout << "fatal error" << std::endl;
        return 1;
    }
    glutDisplayFunc(render);
    glutTimerFunc(1000 / SCREEN_FPS, mainLoop, 0);
    glutMainLoop();
    return 0;
}

bool initGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        return false;
    }
    return true;

}

void update()
{

}
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f( 1.0f,  1.0f);
    glVertex2f(-1.0f,  1.0f);
    glEnd();
    glutSwapBuffers();
}

void mainLoop(int val)
{
    update();
    render();
    glutTimerFunc(1000 / SCREEN_FPS, mainLoop, val);
}