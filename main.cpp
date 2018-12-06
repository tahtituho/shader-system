/*
* Mostly borrowed from http://lazyfoo.net/tutorials/OpenGL/01_hello_opengl/index2.php
* and from https://github.com/joshb/glsl_lighting
*/

#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <json.h>
#include "Configuration.h"

const char* VERSION = "0.6";

bool handleConfigurations(std::string configurations);
bool initGL();
void update();
void render();
void handleKeyboard(unsigned char key, int x, int y);
void mainLoop(int);
bool initShaders(bool first);
std::string readShaderSource(std::string path);
bool compileShader(const GLenum type, const std::string source, bool first);
void cleanUp();

bool fullscreen;

GLuint program;
GLint timeUniform;
GLint resolutionUniform;

std::string vertexPath;
GLuint vertexShader;
std::string fragmentPath;
GLuint fragmentShader;

DemoSystem::Configuration configurations;

int main(int argc, char* args[])
{
    std::cout << "[INFO]: shader system version " << VERSION << " by tähtituho 2018" << std::endl;
    
    std::string confFile = "configuration.json";
    if(argc > 1) {
        confFile = std::string(args[1]);
    }
    else {
        std::cout << "[INFO]: Use configuration json file as parameter. Defaulting to configuration.json" << std::endl;
    }
    
    if(!configurations.read(confFile)) {
        std::cout << "[INFO]: configuration file " << confFile << " is missing. Using default configuration file configuration.json" << std::endl;
    }

    vertexPath = configurations.shaders.vertex;
    fragmentPath = configurations.shaders.fragment;

    if(vertexPath.empty() || fragmentPath.empty())
    {
        std::cerr << "[ERROR]: provide vertex and fragment shader files as parameter .ie -v vertex.glgl -f fragment.glsl" << std::endl;
        return 1;
    }

    glutInit(&argc, args);
    glutInitContextVersion(2, 1);
 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(configurations.screen.width, configurations.screen.height);
    glutCreateWindow((configurations.demo.group + " : " + configurations.demo.name).c_str());

    GLenum glewError = glewInit();
    if (GLEW_OK != glewError)
    {
        std::cout << "glew error: " << glewGetErrorString(glewError) << std::endl;
        return false;
    }

    std::cout << "[INFO]: opengl vendor:   " << glGetString(GL_VENDOR) << std::endl; 
    std::cout << "[INFO]: opengl renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "[INFO]: opengl version:  " << glGetString(GL_VERSION) << std::endl;
    std::cout << "[INFO]: shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "[INFO]: glew version:    " << glewGetString(GLEW_VERSION) << std::endl;

    if(!initShaders(true))
    {
        std::cerr << "[ERROR]: init shaders error" << std::endl;
        return 1;
    }
    
    if(!initGL())
    {
        std::cerr << "[ERROR]: init error" << std::endl;
        return 1;
    }
    
    glutDisplayFunc(render);
    glutKeyboardFunc(handleKeyboard);
    glutTimerFunc(1000 / configurations.screen.FPS, mainLoop, 0);
    glutMainLoop();

    cleanUp();
    return 0;
}

bool handleConfigurations(std::string path) {

    return true;
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
        std::cerr << "[ERROR]: init opengl error: " << glewGetErrorString(error) << std::endl;
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
    glUseProgram(program);

    GLint time = glutGet(GLUT_ELAPSED_TIME);
    glUniform1f(timeUniform, (GLfloat)time / 1000.0);
    glUniform2f(resolutionUniform, (GLfloat)configurations.screen.width, (GLfloat)configurations.screen.height);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f( 1.0f,  1.0f);
    glVertex2f(-1.0f,  1.0f);
    glEnd();
    glUseProgram(0);
    glutSwapBuffers();
}

void handleKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'f':
            if(fullscreen) 
            {
                glutReshapeWindow(configurations.screen.width, configurations.screen.height);
            }
            else
            {
                glutFullScreen();
            }
            fullscreen = !fullscreen;
            break;
        case 'r':
            initShaders(false);
            break;
    }
}
void mainLoop(int val)
{
    update();
    render();
    glutTimerFunc(1000 / configurations.screen.FPS, mainLoop, val);
}

std::string readShaderSource(std::string path)
{
    std::ifstream ifs(path);
    std::string content = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}
bool initShaders(bool first)
{
    if(first) 
    {
        program = glCreateProgram();
    }
   
   
    std::string vertexSource = readShaderSource(vertexPath);
    std::string fragmentSource = readShaderSource(fragmentPath);

    if(compileShader(GL_VERTEX_SHADER, vertexSource, first) == false) 
    {
        return false;
    }
    if(compileShader(GL_FRAGMENT_SHADER, fragmentSource, first) == false)
    {
        return false;
    }
    if(first == false)
    {
        glDeleteProgram(program);
        program = glCreateProgram();
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
   
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
        std::cerr << "[ERROR]: program linking error: " << log << std::endl; 
        delete[] log;
        if(first)
        {
            glDeleteProgram(program);
            program = 0;
        }
        
        return false;

    }
    timeUniform = glGetUniformLocation(program, "time");
    resolutionUniform = glGetUniformLocation(program, "resolution");
    return true;

}

bool compileShader(const GLenum type, std::string source, bool first)
{
    GLuint shader = glCreateShader(type);
    const GLchar* sourceChar = (const GLchar*)source.c_str();
    glShaderSource(shader, 1, &sourceChar, 0);
    glCompileShader(shader);

    GLint compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if(compileResult == GL_FALSE)
    {
        GLint logLength;
        char* log;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        log = new char[logLength];
        GLint infoLogStatus;
        glGetShaderInfoLog(shader, logLength, &infoLogStatus, log);
        if(type == GL_VERTEX_SHADER)
        {
            std::cerr << "[ERROR]: vertex shader compile error: " << log << std::endl; 
        }
        else
        {
            std::cerr << "[ERROR]: fragment shader compile error: " << log << std::endl; 
        }
        
        delete[] log;
        
        glDeleteShader(shader);
        program = 0;

        return false;
    }
    if(type == GL_VERTEX_SHADER)
    {      
        vertexShader = shader;
    }
    else
    {
        fragmentShader = shader;
    }

    return true;
}
void cleanUp()
{
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
    program = 0;
}