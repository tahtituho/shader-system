#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Configuration.h"
#include "Music.h"
#include "Cosmonaut.h"
#include "Textures.h"
#include "Logger.h"

const char* VERSION = "1.4";
#define SYNC_PLAYER

void update(double time);
void render(double time);
void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void mainLoop();
bool initShaders(bool first);
std::string readShaderSource(std::string path);
bool compileShader(const GLenum type, const std::string source, bool first);
void logError(int error, const char* desc);
void cleanUp();
void writeToLogFile( const std::string &text, bool clean);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void musicPause(void* c, int flag);
void musicSetRow(void* c, int row);
int musicPlaying(void* c);

bool fullscreen;

GLFWwindow* window;
GLuint program;
GLint timeUniform;
GLint resolutionUniform;

std::string vertexPath;
GLuint vertexShader;
std::string fragmentPath;
GLuint fragmentShader;
unsigned int VBO, VAO, EBO;

DemoSystem::Configuration configurations;
DemoSystem::Music music;
DemoSystem::Cosmonaut cosmonaut;
DemoSystem::Textures textures;
DemoSystem::Logger logger;

int main(int argc, char* args[])
{
    
    std::string confFile = "configuration.json";
    if(argc > 1) {
        confFile = std::string(args[1]);
    }
    
    configurations.read(confFile);
    if (configurations.demo.log) {
        logger.write(DemoSystem::Logger::INFO, "shader system version "  + std::string(VERSION) + " by tähtituho 2019");
        logger.write(DemoSystem::Logger::INFO, "use configuration json file as parameter. Default is configuration.json");
    }

    vertexPath = configurations.shaders.vertex;
    fragmentPath = configurations.shaders.fragment;

    if(vertexPath.empty() || fragmentPath.empty())
    {
        if (configurations.demo.log) {
            logger.write(DemoSystem::Logger::ERR, "provide vertex and fragment shaders in configuration file");
        }
        return 1;
    }

    if (!glfwInit()) {
        if (configurations.demo.log) {
            logger.write(DemoSystem::Logger::ERR, "glfwInit failed");
        }
        return -1;
    }

    if(configurations.demo.log) {
        glfwSetErrorCallback(logError);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, configurations.shaders.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, configurations.shaders.minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (configurations.demo.release) {
        // Two ways to start as fullscreen, need to check which is more suitable
        // This is true fullscreen!
        window = glfwCreateWindow(configurations.screen.width, configurations.screen.height, (configurations.demo.group + " : " + configurations.demo.name).c_str(), glfwGetPrimaryMonitor(), NULL);
        // This is windowed fullscreen!
        /*const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        window = glfwCreateWindow(mode->width, mode->height, (configurations.demo.group + " : " + configurations.demo.name).c_str(), glfwGetPrimaryMonitor(), NULL);*/
    }
    else {
        window = glfwCreateWindow(configurations.screen.width, configurations.screen.height, (configurations.demo.group + " : " + configurations.demo.name).c_str(), NULL, NULL);
    }

    if(!window) {
        if (configurations.demo.log) {
            logger.write(DemoSystem::Logger::ERR, "window creation failed");
        }
        glfwTerminate();
        return -1;
    }

    // Setting window icon
    GLFWimage icons[1];
    unsigned error;
    GLFWimage image;
    std::vector<unsigned char> imageBuffer;
    unsigned int width;
    unsigned int height;
    // Using temp variables, for some reason casting didn't work as expected
    error = lodepng::decode(imageBuffer, width, height, configurations.demo.icon);
    if (error == 0) {
        image.height = height;
        image.width = width;
        image.pixels = &imageBuffer[0];
        icons[0] = image;

        glfwSetWindowIcon(window, 1, icons);
    }

    if (configurations.demo.release) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    glfwSetKeyCallback(window, handleKeyboard);
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);

    GLenum glewError = glewInit();
    if (GLEW_OK != glewError)
    {
        if (configurations.demo.log) {
            logger.write(DemoSystem::Logger::ERR, "glew error: " + std::string((const char*)glewGetErrorString(glewError)));
        }
        return false;
    }
    glViewport(0, 0, configurations.screen.width, configurations.screen.height);
    
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (configurations.demo.log) {
        logger.write(DemoSystem::Logger::INFO, "opengl vendor:   " + std::string((const char*)glGetString(GL_VENDOR)));
        logger.write(DemoSystem::Logger::INFO, "opengl renderer  " + std::string((const char*)glGetString(GL_RENDERER)));
        logger.write(DemoSystem::Logger::INFO, "opengl version:  " + std::string((const char*)glGetString(GL_VERSION)));
        logger.write(DemoSystem::Logger::INFO, "shading version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
        logger.write(DemoSystem::Logger::INFO, "context version  " + std::to_string(configurations.shaders.majorVersion) + "." + std::to_string(configurations.shaders.minorVersion));
        logger.write(DemoSystem::Logger::INFO, "bass version:    " + std::to_string(BASS_GetVersion()));
    }
    
    music.initialize(configurations.tune.frequency, configurations.tune.file);
    cosmonaut.initialize(configurations.tune.BPM, configurations.sync.RPB);
    if (!configurations.demo.release) {
        cosmonaut.connectPlayer(configurations.sync.host);
    }
    sync_cb functions;
    functions.is_playing = &musicPlaying;
    functions.pause = &musicPause;
    functions.set_row = &musicSetRow;
    cosmonaut.setFunctions(&functions);
    cosmonaut.setTracks(configurations.tracks);
    std::list<DemoSystem::Asset> t(configurations.assets);
    t.remove_if([](DemoSystem::Asset a) {
        return a.type != DemoSystem::Asset::AssetType::TEXTURE;
    });
    textures.setTextures(t);

    if(!initShaders(true))
    {
        if (configurations.demo.log) {
            logger.write(DemoSystem::Logger::ERR, "init shaders error");
        }
        return 1;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // VBO is basically the vertices of the model, VAO is a list of VBO's
    // We can use the index of the VAO to draw which VBO we want.
    // In our case, we will be drawing to triangles to cover the screen,
    // so we have room to show the shader. It should be possible to do 
    // this with quads, and not mess with the triangles.
    // Using EBO we can store the vertices in a separate variable,
    // and then just use these vertices in any order we want.
    float vertices[] = {
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    music.play();
    mainLoop();
    cleanUp();
    return 0;
}

void musicPause(void* rr, int flag) {
    if(flag == 1) {
        music.pause();
    }
    else {
        music.play();
    }
}

void musicSetRow(void* rr, int row) {
    double rowRate = *((double *)rr);
    music.seek(row / rowRate);
}

int musicPlaying(void* rr) {
    if(music.isPlaying()) {
        return 1;
    }
    else {
        return 0;
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void update(double time)
{
    cosmonaut.update(time * cosmonaut.getRowRate());
}

void render(double time)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    glUniform1f(timeUniform, (GLfloat)time);
    glUniform2f(resolutionUniform, (GLfloat)configurations.screen.width, (GLfloat)configurations.screen.height);
 
    for(std::list<DemoSystem::Cosmonaut::Gateway>::iterator it = cosmonaut.gateways.begin(); it != cosmonaut.gateways.end(); ++it) {
        if(it->type == DemoSystem::Track::FLOAT1) {
            glUniform1f(it->uniform, (GLfloat)it->value.x);
        }
        else if(it->type == DemoSystem::Track::FLOAT2) {
            glUniform2f(it->uniform, (GLfloat)it->value.x, (GLfloat)it->value.y);
        }
        else if(it->type == DemoSystem::Track::FLOAT3) {
            glUniform3f(it->uniform, (GLfloat)it->value.x, (GLfloat)it->value.y, (GLfloat)it->value.z);
        }
    }

    unsigned int textureIndex = 0;

    for(std::list<DemoSystem::Textures::Texture>::iterator it = textures.textures.begin(); it != textures.textures.end(); ++it) {
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, it->handle);
        glUniform1i(it->uniform, textureIndex);
        textureIndex++;
    }
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        switch(key)
        {
            case GLFW_KEY_F:
                if(fullscreen) {
                    glfwRestoreWindow(window);
                }
                else {
                    glfwMaximizeWindow(window);
                }
                fullscreen = !fullscreen;
                break;
            case GLFW_KEY_R:
                initShaders(false);
                break;
            case GLFW_KEY_S:
                if(configurations.demo.release == false) {
                    music.silence();
                }          
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }
}

void mainLoop()
{
    while(!glfwWindowShouldClose(window)) {
        double position = music.position();
        update(position);
        render(position);
        if(configurations.demo.release == true && music.hasMusicEnded() == true) {
           glfwSetWindowShouldClose(window, GLFW_TRUE); 
        }
    }
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
        if (configurations.demo.log) {
            logger.write(DemoSystem::Logger::ERR, "program linking error" + std::string(log));
        }
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

    //This should be done inside of cosmonaut
    //Find a way to fix this
    for(std::list<DemoSystem::Cosmonaut::Gateway>::iterator it = cosmonaut.gateways.begin(); it != cosmonaut.gateways.end(); ++it) {
        it->uniform = glGetUniformLocation(program, it->name.c_str());
    }

    for(std::list<DemoSystem::Textures::Texture>::iterator it = textures.textures.begin(); it != textures.textures.end(); ++it) {
        glGenTextures(1, &it->handle);
        glBindTexture(GL_TEXTURE_2D, it->handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, it->width, it->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &it->image[0]);
        it->uniform = glGetUniformLocation(program, it->name.c_str());
    }
    
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
            if (configurations.demo.log) {
                logger.write(DemoSystem::Logger::ERR, "vertex shader compile error: " + std::string(log));
            }
        }
        else
        {
            if (configurations.demo.log) {
                logger.write(DemoSystem::Logger::ERR, "fragment shader compile error: " + std::string(log));
            }
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

void logError(int error, const char* desc)
{
    logger.write(DemoSystem::Logger::ERR, "glfw error: " + std::to_string(error) + std::string(desc));
}

void cleanUp()
{
    glDeleteProgram(program);
    program = 0;

    glfwDestroyWindow(window);
    cosmonaut.cleanUp();
    music.cleanUp();
    glfwTerminate();
}