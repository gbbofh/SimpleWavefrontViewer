#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

class Shader;
class VertexShader;
class FragmentShader;

class ShaderProgram;

class Mesh;

class Shader {

        friend class ShaderProgram;

public:
        virtual ~Shader() {}

        std::string getPath();
        std::string getError();
        std::string getSource();

        void loadSource(const char* path);
        void setSource(const char* source);

protected:

        virtual bool compile() = 0;

        GLuint shaderID;
        std::string path;
        std::string source;
};

class VertexShader : public Shader {

public:
        bool compile();
};

class FragmentShader : public Shader {

public:
        bool compile();
};

class ShaderProgram {

        friend class Mesh;

public:
        ~ShaderProgram();

        void activate();
        bool linkProgram();
        void addShader(Shader* shader);

        void setUniform(const char* name, int i);
        void setUniform(const char* name, float f);
        void setUniform(const char* name, glm::vec2 vec);
        void setUniform(const char* name, glm::vec3 vec);
        void setUniform(const char* name, glm::vec4 vec);
        void setUniform(const char* name, glm::mat4 mat);

        std::string getError();

        /**
         * TEMPORARY -- to avoid changing the mesh interface
         */

        int getProgramID() {return this->programID;}

private:
        GLuint programID;
        std::vector<Shader*> shaders;

        enum class ProgramErrorType {

                ERROR_LINK,
                ERROR_COMPILE,

        } errorType;
};

