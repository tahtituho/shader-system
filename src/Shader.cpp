#include "Shader.h"

DemoSystem::Shader::Shader() {
}

DemoSystem::Shader::~Shader() {
    this->cleanShader();
}

void DemoSystem::Shader::initialize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;

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
}

void DemoSystem::Shader::initializeUniform(std::string variable, std::string track) {
    UniformVariable* uv = new UniformVariable();
    uv->track = track;
    uv->name = variable;
    uv->uniform = -1;
    this->uniforms.push_back(uv);
}

void DemoSystem::Shader::initializeUniforms(std::list<DemoSystem::Common::TrackVariableBond> trackVariableBonds) {
    for(DemoSystem::Common::TrackVariableBond trackVariable : trackVariableBonds) {
        this->initializeUniform(trackVariable.variable, trackVariable.track);
    }
}

void DemoSystem::Shader::initializeTexture(std::string texture) {
    UniformTexture* ut = new UniformTexture();
    ut->name = texture;
    ut->uniform = -1;
    this->textures.push_back(ut);
}

void DemoSystem::Shader::initializeTextures(std::list<std::string> textures) {
    for(std::string texture : textures) {
        this->initializeTexture(texture);
    }
}

void DemoSystem::Shader::setSources(std::string vertexSource, std::string fragmentSource) {
    this->vertexSource = vertexSource;
    this->fragmentSource = fragmentSource;
}

void DemoSystem::Shader::setCosmonaut(DemoSystem::Cosmonaut* cosmonaut) {
    this->cosmonaut = cosmonaut;
}

void DemoSystem::Shader::setTextureManager(DemoSystem::TextureManager* textureManager) {
    this->textureManager = textureManager;
}

DemoSystem::Logger::Message DemoSystem::Shader::initShader()
{
    DemoSystem::Logger::Message m;
   
    this->program = glCreateProgram();
    this->vertex = glCreateShader(GL_VERTEX_SHADER);
    this->fragment = glCreateShader(GL_FRAGMENT_SHADER);
    
    m = this->compileShader(GL_VERTEX_SHADER);
    if(m.failure == true) {
        return m;
    }

    m = this->compileShader(GL_FRAGMENT_SHADER);
    if(m.failure == true) {
        return m;
    }

    glAttachShader(program, this->vertex);
    glAttachShader(program, this->fragment);
   
    GLint linkStatus = 0;
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE)
    {
        GLint logLength = 1024;
        char* log;
        log = new char[logLength];
        GLint infoLogStatus = 0;
        glGetProgramInfoLog(program, logLength, &infoLogStatus, log);
        m.failure = true;
        m.content = std::string(log);
        delete[] log;
        return m;
    }
    this->refreshUniforms();  
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
        GLint logLength = 1024;
        char* log;
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

void DemoSystem::Shader::cleanShader() {
    glDetachShader(this->program, this->vertex);
    glDetachShader(this->program, this->fragment);
    glDeleteShader(this->vertex);
    glDeleteShader(this->fragment);
    glDeleteProgram(this->program);
}

void DemoSystem::Shader::render(double time) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(this->program);

    for(const DemoSystem::Shader::UniformVariable* uv : this->uniforms) {
        if(uv->track == "time") {
            glUniform1f(uv->uniform, (GLfloat)time);
            continue;
        }
        else if(uv->track == "resolution") {
            glUniform2f(uv->uniform, (GLfloat)this->width, (GLfloat)this->height);
            continue;
        }
        DemoSystem::Common::Gateway gateway = this->cosmonaut->getTrack(uv->track);
        switch(gateway.type) {
            case DemoSystem::Common::Track::FLOAT1:
                glUniform1f(uv->uniform, (GLfloat)gateway.value.x);
                break;
            case DemoSystem::Common::Track::FLOAT2:
                glUniform2f(uv->uniform, (GLfloat)gateway.value.x, (GLfloat)gateway.value.y);
                break;
            case DemoSystem::Common::Track::FLOAT3:
                glUniform3f(uv->uniform, (GLfloat)gateway.value.x, (GLfloat)gateway.value.y, (GLfloat)gateway.value.z);
                break;
        }
    }
    int index = 0;
    for(const DemoSystem::Shader::UniformTexture* ut : this->textures) {
        DemoSystem::Texture* t = this->textureManager->getTexture(ut->name);
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, t->handle);
        glUniform1i(ut->uniform, index);
        index++;
    }
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glUseProgram(0);
}

void DemoSystem::Shader::refreshUniforms() {
    for(UniformVariable* uv : this->uniforms) {
        uv->uniform = glGetUniformLocation(this->program, uv->name.c_str());
    }
    for(UniformTexture* ut : this->textures) {
        this->textureManager->initializeTexture(ut->name);
        ut->uniform = glGetUniformLocation(this->program, ut->name.c_str());
    }
}