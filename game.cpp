#include "game.hpp"
#include "model.hpp"
#include "shader.hpp"

#include <iostream>

// Temporary
#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Game::onStart()
{
        Shader* frag;
        Shader* vert;

        frag = new FragmentShader();
        frag->loadSource("resources/frag_default.glsl");

        vert = new VertexShader();
        vert->loadSource("resources/vert_default.glsl");

        this->shaderProgram.addShader(frag);
        this->shaderProgram.addShader(vert);
        this->shaderProgram.linkProgram();
        this->shaderProgram.activate();

        if(!this->args.empty()) {

                this->model = Model(this->args[0]);

        } else {

                this->model = Model("resources/rex.obj");
        }

        glClearColor(0.25, 0.50, 0.75, 1.0);
}

void Game::onShutdown()
{
}

void Game::onUpdate(double dt)
{
        static float x;
        static float y;
        static float z;

        x = cosf(getTime()) * 7.0f;
        y = 3.0f;
        z = sinf(getTime()) * 7.0f;

        glm::mat4 mvp = glm::perspective(glm::radians(45.0f),
                        (float)this->win_w / (float)this->win_h,
                        0.1f, 100.0f) * glm::lookAt(glm::vec3(x, y, z),
                                glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        this->shaderProgram.setUniform("mvp", mvp);
        this->model.draw(this->shaderProgram);
}

void Game::keyboardInput(int key, int scan, int action, int mods)
{
        Engine::keyboardInput(key, scan, action, mods);
}

void Game::mouseButton(int button, int action, int mods)
{
        Engine::mouseButton(button, action, mods);
}

