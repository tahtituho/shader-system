#include "Shader.h"

DemoSystem::Shader::Shader() {

}

DemoSystem::Shader::~Shader() {
    glDeleteShader(this->vertex);
    glDeleteShader(this->fragment);
    glDeleteProgram(this->program);
}

void DemoSystem::Shader::initialize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;

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

void DemoSystem::Shader::initializeUniform(std::string variable, std::string track) {
    DemoSystem::Shader::UniformVariable uv;
    uv.uniform = glGetUniformLocation(this->program, variable.c_str());
    uv.track = track;
    this->uniforms.push_back(uv);
}

void DemoSystem::Shader::initializeUniforms(std::list<DemoSystem::Common::TrackVariableBond> trackVariableBonds) {
    for(DemoSystem::Common::TrackVariableBond trackVariable : trackVariableBonds) {
        this->initializeUniform(trackVariable.variable, trackVariable.track);
    }
}
void DemoSystem::Shader::setSources(std::string vertexSource, std::string fragmentSource) {
    this->vertexSource = vertexSource;
    this->fragmentSource = fragmentSource;
}

void DemoSystem::Shader::setCosmonaut(DemoSystem::Cosmonaut* cosmonaut) {
    this->cosmonaut = cosmonaut;
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

    return m;
}

DemoSystem::Logger::Message DemoSystem::Shader::compileShader(GLenum type)
{
    DemoSystem::Logger::Message m;
    GLuint shader = glCreateShader(type);
    std::string source = type == GL_VERTEX_SHADER ? this->vertexSource : this->fragmentSource;
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

    for(const DemoSystem::Shader::UniformVariable uv : this->uniforms) {
        if(uv.track == "time") {
            glUniform1f(uv.uniform, (GLfloat)time);
            continue;
        }
        else if(uv.track == "resolution") {
            glUniform2f(uv.uniform, (GLfloat)this->width, (GLfloat)this->height);
            continue;
        }
        DemoSystem::Common::Gateway gateway = this->cosmonaut->getTrack(uv.track);
        switch(gateway.type) {
            case DemoSystem::Common::Track::FLOAT1:
                glUniform1f(uv.uniform, (GLfloat)gateway.value.x);
                break;
            case DemoSystem::Common::Track::FLOAT2:
                glUniform2f(uv.uniform, (GLfloat)gateway.value.x, (GLfloat)gateway.value.y);
                break;
            case DemoSystem::Common::Track::FLOAT3:
                glUniform3f(uv.uniform, (GLfloat)gateway.value.x, (GLfloat)gateway.value.y, (GLfloat)gateway.value.z);
                break;
        }
    }
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUseProgram(0);
}

