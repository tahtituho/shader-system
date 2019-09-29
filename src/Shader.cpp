#include "Shader.h"

DemoSystem::Shader::Shader() {

}

DemoSystem::Shader::Shader(std::string vertexFile, std::string fragmentFile) {
    this->vertexFile = vertexFile;
    this->fragmentFile = fragmentFile;

    this->program = glCreateProgram();
    this->vertex = glCreateShader(GL_VERTEX_SHADER);
    this->fragment = glCreateShader(GL_FRAGMENT_SHADER);
}

DemoSystem::Shader::~Shader() {
    glDeleteShader(this->vertex);
    glDeleteShader(this->fragment);
    glDeleteProgram(this->program);
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
    }

    return m;
}

DemoSystem::Logger::Message DemoSystem::Shader::compileShader(GLenum type)
{
    DemoSystem::Logger::Message m;
    GLuint shader = type == GL_VERTEX_SHADER ? this->vertex : this->fragment;
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
        program = 0;
    }    

    return m;
}

