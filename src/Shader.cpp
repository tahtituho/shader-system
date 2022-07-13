#include "Shader.h"

DemoSystem::Shader::Shader() {
}

DemoSystem::Shader::~Shader() {
}

void DemoSystem::Shader::setScreenSize(int width, int height)
{
    this->screenWidth = width;
    this->screenHeight = height;
}

void DemoSystem::Shader::registerSynchronizer(DemoSystem::Synchronizer *synchronizer)
{
    this->synchronizer = synchronizer;
}

void DemoSystem::Shader::registerTextures(std::list<Textures::Texture> *textures)
{
    this->textures = textures;
}

void DemoSystem::Shader::registerCamera(DemoSystem::Camera *camera)
{
    this->camera = camera;
}

void DemoSystem::Shader::registerLogger(Logger *logger)
{
    this->logger = logger;
}

void DemoSystem::Shader::addUniforms() 
{
    for (auto tv = this->synchronizer->trackVariables.begin(); tv != this->synchronizer->trackVariables.end(); tv++)
    {
        tv->uniform = glGetUniformLocation(this->program, tv->name.c_str());
    }

    for (auto bv = this->synchronizer->basicVariables.begin(); bv != this->synchronizer->basicVariables.end(); bv++)
    {
        bv->second.uniform = glGetUniformLocation(this->program, bv->first.c_str());
    }

    for (auto it = this->textures->begin(); it != this->textures->end(); it++)
    {
        glGenTextures(1, &it->handle);
        glBindTexture(GL_TEXTURE_2D, it->handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, it->width, it->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &it->image[0]);
        it->uniform = glGetUniformLocation(this->program, it->name.c_str());
    }
}

void DemoSystem::Shader::initShaders(std::string vertexSource, std::string fragmentSource)
{
    if (vertexSource == "") 
    {
        logger->write(DemoSystem::Logger::ERR, "vertex shader source is empty");
        return;
    }
    if (fragmentSource == "") 
    {
        logger->write(DemoSystem::Logger::ERR, "fragment shader source is empty");
        return;
    }
    logger->write(DemoSystem::Logger::INFO, "compile started...");
    if (this->program != 0)
    {
        glDeleteProgram(this->program);
    }
    this->program = glCreateProgram();

    this->compileShader(GL_VERTEX_SHADER, vertexSource);
    this->compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, fragmentShader);

    GLint linkStatus;
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            char *log = new char[logLength];
            glGetProgramInfoLog(this->program, logLength, NULL, log);
            logger->write(DemoSystem::Logger::ERR, std::string(log));
            delete[] log;
        }
        return;
    }

    logger->write(DemoSystem::Logger::INFO, "compile done");
    return;
}

void DemoSystem::Shader::initFrameBuffer()
{
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
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f};
    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void DemoSystem::Shader::compileShader(const GLenum type, std::string source)
{
    GLuint shader = glCreateShader(type);
    const GLchar *sourceChar = (const GLchar *)source.c_str();
    glShaderSource(shader, 1, &sourceChar, 0);
    glCompileShader(shader);

    GLint compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            char *log = new char[logLength];
            glGetShaderInfoLog(shader, logLength, NULL, log);
            logger->write(DemoSystem::Logger::ERR, std::string(log));
            delete[] log;
        }
        glDeleteShader(shader);
        return;
    }
    if (type == GL_VERTEX_SHADER)
    {
        vertexShader = shader;
    }
    else
    {
        fragmentShader = shader;
    }

    return;
}

void DemoSystem::Shader::render(double time)
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    BasicVariable *timeVariable = &this->synchronizer->basicVariables.at("time");
    glUniform1f(timeVariable->uniform, (GLfloat)time);

    BasicVariable *resolutionVariable = &this->synchronizer->basicVariables.at("resolution");
    glUniform2f(resolutionVariable->uniform, (GLfloat)this->screenWidth, (GLfloat)this->screenHeight);

    BasicVariable *positionVariable = &this->synchronizer->basicVariables.at("position");
    Vector3 cameraPosition = this->camera->getPosition();
    glUniform3f(positionVariable->uniform, (GLfloat)cameraPosition.x, (GLfloat)cameraPosition.y, (GLfloat)cameraPosition.z);

    BasicVariable *viewDirVariable = &this->synchronizer->basicVariables.at("viewDir");
    Vector3 viewDir = this->camera->getViewDir();
    glUniform3f(viewDirVariable->uniform, (GLfloat)viewDir.x, (GLfloat)viewDir.y, (GLfloat)viewDir.z);

    BasicVariable *userVariable = &this->synchronizer->basicVariables.at("user");
    glUniform3f(userVariable->uniform, (GLfloat)userVariable->value.x, (GLfloat)userVariable->value.y, (GLfloat)userVariable->value.z);

    for (auto tv = this->synchronizer->trackVariables.begin(); tv != this->synchronizer->trackVariables.end(); tv++)
    {
        if (tv->type == Variable::DataType::FLOAT1)
        {
            glUniform1f(tv->uniform, (GLfloat)tv->value.x);
        }
        else if (tv->type == Variable::DataType::FLOAT2)
        {
            glUniform2f(tv->uniform, (GLfloat)tv->value.x, (GLfloat)tv->value.y);
        }
        else if (tv->type == Variable::DataType::FLOAT3)
        {
            glUniform3f(tv->uniform, (GLfloat)tv->value.x, (GLfloat)tv->value.y, (GLfloat)tv->value.z);
        }
    }

    unsigned int textureIndex = 0;

    for (auto it = this->textures->begin(); it != this->textures->end(); it++)
    {
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, it->handle);
        glUniform1i(it->uniform, textureIndex);
        textureIndex++;
    }

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //this->logger->render();
    //glUseProgram(0);
    //glfwSwapBuffers(this->window);
    glfwPollEvents();
}

// Rendering function used for "quick-and-dirty" post-processing without renderbuffer
void DemoSystem::Shader::renderPost(double time) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(this->program);

    glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, (GLfloat)this->screenWidth, (GLfloat)this->screenHeight, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
	glUniform1i(0, 0);
    
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUseProgram(0);
}

void DemoSystem::Shader::cleanUp()
{
    glDeleteProgram(this->program);
}
