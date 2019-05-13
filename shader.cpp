#include "shader.hpp"

#include <fstream>

std::string Shader::getPath()
{
        return this->path;
}

std::string Shader::getError()
{

        int status;
        GLchar buf[4096];

        glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE) {

                glGetShaderInfoLog(this->shaderID, 4096, nullptr, buf);
        }

        return std::string(buf);
}

std::string Shader::getSource()
{
        return this->source;
}

void Shader::loadSource(const char* path)
{
        std::string line;
        std::ifstream shaderFile;

        shaderFile.open(path);

        while(std::getline(shaderFile, line)) {

                this->source += line + '\n';
        }

        shaderFile.close();

        this->path = std::string(path);
}

void Shader::setSource(const char* source)
{
        this->source = std::string(source);
}

/**
 * Now that it isn't 1 AM -- I realized that this did not need to be a virtual
 * method. I could have just called glCreateShader in the constructor. But I am
 * stupid. So I will fix it later, because I've invested so much time into this
 * already.
 */
bool VertexShader::compile()
{
        int status;
        const char* csource;

        csource = this->source.c_str();

        this->shaderID = glCreateShader(GL_VERTEX_SHADER);
        if(this->shaderID < 0) {

                return false;
        }

        glShaderSource(this->shaderID, 1, &csource, nullptr);
        glCompileShader(this->shaderID);

        glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &status);

        return status == GL_TRUE;
}

bool FragmentShader::compile()
{
        int status;
        const char* csource;

        csource = this->source.c_str();

        this->shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if(this->shaderID < 0) {

                return false;
        }

        glShaderSource(this->shaderID, 1, &csource, nullptr);
        glCompileShader(this->shaderID);

        glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &status);

        return status == GL_TRUE;
}

ShaderProgram::~ShaderProgram()
{
        /**
         * I'm not sure that this is the correct way to go about this, as the
         * shaders are not created by the shader program class. They probably
         * should be. I think I will restructure this such that the
         * ShaderProgram class will have a "loadShader" function, which will
         * call the corresponding function within a new instance of the Shader
         * superclass. That will make this the correct way to delete the list.
         */
        /*std::vector<Shader*>::iterator it;
        for(it = this->shaders.begin(); it != this->shaders.end(); it++) {

                delete *it;
        }
        this->shaders.clear();
        */

        glDeleteProgram(this->programID);
}

void ShaderProgram::activate()
{
        glUseProgram(this->programID);
}

void ShaderProgram::addShader(Shader* shader)
{
        this->shaders.push_back(shader);
}

bool ShaderProgram::linkProgram()
{
        int status;
        bool success;

        this->programID = glCreateProgram();

        for(Shader* shader : this->shaders) {

                success = shader->compile();
                if(!success) return false;

                glAttachShader(this->programID, shader->shaderID);
        }

        glLinkProgram(this->programID);


        glGetProgramiv(this->programID, GL_LINK_STATUS, &status);
        return status == GL_TRUE;
}

void ShaderProgram::setUniform(const char* name, int i)
{
        GLuint loc;

        glUseProgram(this->programID);

        loc = glGetUniformLocation(this->programID, name);
        glUniform1i(loc, i);

        glUseProgram(0);
}

void ShaderProgram::setUniform(const char* name, float f)
{
        GLuint loc;

        glUseProgram(this->programID);

        loc = glGetUniformLocation(this->programID, name);
        glUniform1f(loc, f);

        glUseProgram(0);
}

void ShaderProgram::setUniform(const char* name, glm::vec2 vec)
{
        GLuint loc;

        glUseProgram(this->programID);

        loc = glGetUniformLocation(this->programID, name);
        glUniform2fv(loc, 1, &vec[0]);

        glUseProgram(0);
}

void ShaderProgram::setUniform(const char* name, glm::vec3 vec)
{
        GLuint loc;

        glUseProgram(this->programID);

        loc = glGetUniformLocation(this->programID, name);
        glUniform3fv(loc, 1, &vec[0]);

        glUseProgram(0);
}

void ShaderProgram::setUniform(const char* name, glm::vec4 vec)
{
        GLuint loc;

        glUseProgram(this->programID);

        loc = glGetUniformLocation(this->programID, name);
        glUniform4fv(loc, 1, &vec[0]);

        glUseProgram(0);
}

void ShaderProgram::setUniform(const char* name, glm::mat4 mat)
{
        GLuint loc;

        glUseProgram(this->programID);

        loc = glGetUniformLocation(this->programID, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);

        glUseProgram(0);
}

std::string ShaderProgram::getError()
{

        int status;
        GLchar buf[4096];

        glGetProgramiv(this->programID, GL_LINK_STATUS, &status);
        if(status == GL_FALSE) {

                glGetProgramInfoLog(this->programID, 4096, nullptr, buf);
        }

        return std::string(buf);
}

