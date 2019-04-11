#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Configuration.h"
#include "Music.h"
#include "Cosmonaut.h"
#include "Textures.h"

const char* VERSION = "1.3";
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
void CheckForGLError();
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

int main(int argc, char* args[])
{
    std::cout << "[INFO]: shader system version " << VERSION << " by tähtituho 2019" << std::endl;
    
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

    if (!glfwInit()) {
        std::cerr << "[ERROR]: glfwInit failed" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(logError);
  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, configurations.shaders.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, configurations.shaders.minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(configurations.screen.width, configurations.screen.height, (configurations.demo.group + " : " + configurations.demo.name).c_str(), NULL, NULL);

    if(!window) {
        std::cerr << "[ERROR]: window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, handleKeyboard);
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);

    GLenum glewError = glewInit();
    if (GLEW_OK != glewError)
    {
        std::cerr << "[ERROR]: glew error: " << glewGetErrorString(glewError) << std::endl;
        return false;
    }
    glViewport(0, 0, configurations.screen.width, configurations.screen.height);
    
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    std::cout << "[INFO]: opengl vendor:   " << glGetString(GL_VENDOR) << std::endl; 
    std::cout << "[INFO]: opengl renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "[INFO]: opengl version:  " << glGetString(GL_VERSION) << std::endl;
    std::cout << "[INFO]: shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "[INFO]: context version  " << configurations.shaders.majorVersion << "." << configurations.shaders.minorVersion << std::endl;
    std::cout << "[INFO]: bass version:    " << BASS_GetVersion() << std::endl;

    music.initialize(configurations.tune.frequency, configurations.tune.file);
    cosmonaut.initialize(configurations.tune.BPM, configurations.sync.RPB);
    cosmonaut.connectPlayer(configurations.sync.host);
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
        std::cerr << "[ERROR]: init shaders error" << std::endl;
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

void CheckForGLError()
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "ERROR: 	";
		if (error == GL_INVALID_ENUM)
			std::cout << "GL_INVALID_ENUM";
		if (error == GL_INVALID_VALUE)
			std::cout << "GL_INVALID_VALUE";
		if (error == GL_INVALID_OPERATION)
			std::cout << "GL_INVALID_OPERATION";
		if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION";
		if (error == GL_OUT_OF_MEMORY)
			std::cout << "GL_OUT_OF_MEMORY";
		if (error == GL_STACK_UNDERFLOW)
			std::cout << "GL_STACK_UNDERFLOW";
		if (error == GL_STACK_OVERFLOW)
			std::cout << "GL_STACK_OVERFLOW";
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

void logError(int error, const char* desc)
{
    std::cerr << "[ERROR]: error(" << error << "): " << desc << std::endl; 
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