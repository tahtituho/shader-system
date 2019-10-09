#include "Shader.h"

DemoSystem::Shader::Shader() {

}

DemoSystem::Shader::Shader(std::string vertexFile, std::string fragmentFile, unsigned int width, unsigned int height) {
    this->vertexFile = vertexFile;
    this->fragmentFile = fragmentFile;
    this->width = width;
    this->height = height;
}

DemoSystem::Shader::~Shader() {
    glDeleteShader(this->vertex);
    glDeleteShader(this->fragment);
    glDeleteProgram(this->program);
}

void DemoSystem::Shader::initialize() {
    this->program = glCreateProgram();
    this->vertex = glCreateShader(GL_VERTEX_SHADER);
    this->fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    this->initShader();
}

std::string DemoSystem::Shader::readSource(std::string file) {
    std::ifstream ifs(file);
    return std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
}

DemoSystem::Logger::Message DemoSystem::Shader::initShader()
{
    DemoSystem::Logger::Message m;
    m = this->compileShader(GL_VERTEX_SHADER);
    if(m.failure == true) {
        return m;
    }

    m = this->compileShader(GL_FRAGMENT_SHADER);
    if(m.failure == true) {
        return m;
    }

    if(program != 0)
    {
        glDeleteProgram(program);
        program = glCreateProgram();
    }
    glAttachShader(program, this->vertex);
    glAttachShader(program, this->fragment);
   
    
    GLint linkStatus = 0;
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE)
    {
        GLint logLength = 0;
        char* log;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        log = new char[logLength];
        GLint infoLogStatus = 0;
        glGetProgramInfoLog(program, logLength, &infoLogStatus, log);
        m.failure = true;
        m.content = std::string(log);
        delete[] log;
        return m;
    }

    timeUniform = glGetUniformLocation(program, "time");
    resolutionUniform = glGetUniformLocation(program, "resolution");
    return m;
}

DemoSystem::Logger::Message DemoSystem::Shader::compileShader(GLenum type)
{
    DemoSystem::Logger::Message m;
    GLuint shader = glCreateShader(type);
    std::string source = type == GL_VERTEX_SHADER ? this->readSource(this->vertexFile) : this->readSource(this->fragmentFile);
    const GLchar* sourceChar = (const GLchar*)source.c_str();
    glShaderSource(shader, 1, &sourceChar, 0);
    delete sourceChar;
    glCompileShader(shader);

    GLint compileResult = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if(compileResult == GL_FALSE)
    {
        GLint logLength = 0;
        char* log;

        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logLength);
        log = new char[logLength];
        GLint infoLogStatus = 0;
        glGetShaderInfoLog(shader, logLength, &infoLogStatus, log);
        m.failure = true;
        m.content = std::string(log);
        delete[] log;
        
        glDeleteShader(shader);
        return m;
    }    
    if(type == GL_VERTEX_SHADER)
    {      
        this->vertex = shader;
    }
    else
    {
        this->fragment = shader;
    }
    return m;
}

void DemoSystem::Shader::render(double time) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(this->program);

    glUniform1f(this->timeUniform, (GLfloat)time);
    glUniform2f(this->resolutionUniform, (GLfloat)this->width, (GLfloat)this->height);
 
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUseProgram(0);
}

