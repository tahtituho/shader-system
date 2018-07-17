/*
* Mostly borrowed from http://lazyfoo.net/tutorials/OpenGL/01_hello_opengl/index2.php
* and from https://github.com/joshb/glsl_lighting
*/

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

bool initGL();
void update();
void render();
void handleKeyboard(unsigned char key, int x, int y);
void mainLoop(int);
bool initShaders();
bool compileShader(GLenum type, const std::string source);
void cleanUp();

GLuint program;

char* vertexSource = "#version 120\n"   
		            "void main() {        "
		            "  gl_FragColor[0] = 0.0; "
		            "  gl_FragColor[1] = 0.0; "
		            "  gl_FragColor[2] = 1.0; "
		            "}";
char* fragmentSource = "#version 120\n"
                        "void main() {"
                        "gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
                        "}";

int main(int argc, char* args[])
{
    std::cout << "shader system version 0.1" << std::endl << "by tähtituho 2018" << std::endl;
    //glewExperimental = GL_TRUE;

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

    std::cout << "opengl vendor:   " << glGetString(GL_VENDOR) << std::endl; 
    std::cout << "opengl renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "opengl version:  " << glGetString(GL_VERSION) << std::endl;
    std::cout << "shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "glew version:    " << glewGetString(GLEW_VERSION) << std::endl;
    
    //this is not working
    if(!initShaders())
    {
        std::cout << "init shaders error" << std::endl;
    }
    
    if(!initGL())
    {
        std::cout << "init error" << std::endl;
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

bool initShaders()
{
    program = glCreateProgram();
    
    compileShader(GL_VERTEX_SHADER, std::string(vertexSource));
    compileShader(GL_FRAGMENT_SHADER, std::string(fragmentSource));
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
        std::cout << "program linking error: " << log << std::endl; 
        delete[] log;
        glDeleteProgram(program);
        program = 0;
        return false;

    }
    return true;

}

bool compileShader(GLenum type, std::string source)
{
    GLsizei sourceLength = source.length();
    //assume that source contains shader source code
    GLuint shader = glCreateShader(type);
    /*
    * https://stackoverflow.com/questions/8024433/glsl-shader-compilation-issue-at-runtime/8024613#8024613
    * source should be lines of GLSL, not in one line
    * https://gist.github.com/kenpower/4327014
    * simple shaders to use
    * */
    glShaderSource(shader, 1, (const GLchar* const *)source.c_str(), &sourceLength);
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
        std::cout << "shader compile error: " << log << std::endl; 
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