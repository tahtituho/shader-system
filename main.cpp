/*
* Mostly borrowed from http://lazyfoo.net/tutorials/OpenGL/01_hello_opengl/index2.php
* and from https://github.com/joshb/glsl_lighting
*/

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

const GLint SCREEN_WIDTH = 640;
const GLint SCREEN_HEIGHT = 480;
const GLint SCREEN_FPS = 60;

bool initGL();
void update();
void render();
void handleKeyboard(unsigned char key, int x, int y);
void mainLoop(int);
bool initShaders(std::string vertexSource, std::string fragmentSource);
std::string readShaderSource(std::string path);
bool compileShader(GLenum type, const std::string source);
void cleanUp();

GLuint program;
GLint timeUniform;
GLint resolutionUniform;

std::string vertexPath;
std::string fragmentPath;

int main(int argc, char* args[])
{
    std::cout << "[INFO]: shader system version 0.2 by tähtituho 2018" << std::endl;
    //glewExperimental = GL_TRUE;
    int c = 0;
    while ((c = getopt(argc, args, "v:f:")) != -1)
    {
        switch(c) 
        {
            case 'v':
                vertexPath = std::string(optarg);
                break;  
            case 'f':
                fragmentPath = std::string(optarg);
                break;
            default:
                std::cerr << "[ERROR]: provide vertex and fragment shader files as parameter .ie -v vertex.glgl -f fragment.glsl" << std::endl;
                return 1;

        }
    }

    if(vertexPath.empty() || fragmentPath.empty())
    {
        std::cerr << "[ERROR]: provide vertex and fragment shader files as parameter .ie -v vertex.glgl -f fragment.glsl" << std::endl;
        return 1;
    }

    glutInit(&argc, args);
    glutInitContextVersion(2, 1);
 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("shader system");

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

    if(!initShaders(vertexPath, fragmentPath))
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
    glutTimerFunc(1000 / SCREEN_FPS, mainLoop, 0);
    glutMainLoop();

    cleanUp();
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
    glUniform2f(resolutionUniform, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);

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
    //Do something with keys, maybe uniform variables
}
void mainLoop(int val)
{
    update();
    render();
    glutTimerFunc(1000 / SCREEN_FPS, mainLoop, val);
}

std::string readShaderSource(std::string path)
{
    std::ifstream ifs(path);
    std::string content = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}
bool initShaders(std::string vertexPath, std::string fragmentPath)
{
    if(!program) 
    {
        program = glCreateProgram();
    }
    
    std::string vertexSource = readShaderSource(vertexPath);
    std::string fragmentSource = readShaderSource(fragmentPath);
    if(compileShader(GL_VERTEX_SHADER, vertexSource) == false || compileShader(GL_FRAGMENT_SHADER, fragmentSource) == false)
    {
        return false;
    }
       
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
        glDeleteProgram(program);
        program = 0;
        return false;

    }
    timeUniform = glGetUniformLocation(program, "time");
    resolutionUniform = glGetUniformLocation(program, "resolution");
    return true;

}

bool compileShader(GLenum type, std::string source)
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
    glAttachShader(program, shader);
    glDeleteShader(shader);
    return true;
}
void cleanUp()
{
    glDeleteProgram(program);
    program = 0;
}