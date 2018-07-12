/*
* Mostly borrowed from http://lazyfoo.net/tutorials/OpenGL/01_hello_opengl/index2.php
* and from https://github.com/joshb/glsl_lighting
*/

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

bool initGL();
void update();
void render();
void mainLoop(int);
bool initShaders();
bool compileShader(GLenum type, const char* source);
void cleanUp();

GLuint program;

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
        std::cout << "init error" << std::endl;
        return 1;
    }
    
    std::cout << "opengl vendor:   " << glGetString(GL_VENDOR) << std::endl; 
    std::cout << "opengl renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "opengl version:  " << glGetString(GL_VERSION) << std::endl;
    
    if(!initShaders())
    {
        std::cout << "init shaders error" << std::endl;
    }
    
    glutDisplayFunc(render);
    glutTimerFunc(1000 / SCREEN_FPS, mainLoop, 0);
    glutMainLoop();
    
    cleanUp();
    return 0;
}

bool initGL() {
    GLenum glewError = glewInit();
    if (GLEW_OK != glewError)
    {
        std::cout << "glew rrror: " << glewGetErrorString(glewError) << std::endl;
        return false;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::cout << "init opengl error: " << glewGetErrorString(error) << std::endl;
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

bool initShaders()
{
    program = glCreateProgram();
    //Create shaders and other stuff
    GLint linkStatus;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE)
    {
        GLint logLength;
        char* log;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        log = new char[logLength];
        GLint infoLogStatus;
        glGetProgramInfoLog(program, logLength, &infoLogStatus, log);
        std::cout << "Program linking error: " << log << std::endl; 
        delete[] log;
        glDeleteProgram(program);
        program = 0;
        return false;

    }
    return true;

}

bool compileShader(GLenum type, std::string source)
{
    GLuint shader;
    GLint sourceLength = source.length();
    return true;
}
void cleanUp()
{
    glDeleteProgram(program);
    program = 0;
}