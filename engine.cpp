#include "engine.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Engine::startUp(int width, int height, const char* title, bool fullscreen)
{
        int rv;

        this->run = false;

        glfwSetErrorCallback(this->logError);

        rv = glfwInit();
        if(rv != GLFW_TRUE) {

                throw EngineError("Failed to initialize glfw.");
        }

        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        this->monitor = glfwGetPrimaryMonitor();
        this->window = glfwCreateWindow(width, height, title,
                        fullscreen ? this->monitor : nullptr, nullptr);
        if(!this->window) {

                glfwTerminate();
                throw EngineError("Failed to create window.");
        }

        glfwSetWindowUserPointer(this->window, this);
        glfwSetKeyCallback(this->window, onKeyboardInput);
        glfwSetCursorPosCallback(this->window, onMouseInput);
        glfwSetMouseButtonCallback(this->window, onMouseButton);

        glfwMakeContextCurrent(this->window);

        rv = glewInit();
        if(rv != GLEW_OK) {

                glfwDestroyWindow(this->window);
                glfwTerminate();

                throw EngineError("Failed to initialize glew.");
        }

        glfwGetWindowPos(this->window, &this->win_x, &this->win_y);
        glfwGetWindowSize(this->window, &this->win_w, &this->win_h);

        this->run = true;

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        glDebugMessageCallback(Engine::glError, 0);

        onStart();
}

void Engine::shutDown()
{
        onShutdown();

        if(this->window) {

                glfwDestroyWindow(this->window);
        }

        glfwTerminate();
}

void Engine::setFullscreen(bool fullscreen)
{
        const GLFWvidmode* mode;

        if(isFullscreen() == fullscreen) return;

        if(fullscreen) {

                glfwGetWindowPos(this->window, &this->win_x, &this->win_y);
                glfwGetWindowSize(this->window, &this->win_w, &this->win_h);

                mode = glfwGetVideoMode(this->monitor);

                glfwSetWindowMonitor(this->window, this->monitor,
                                0, 0, mode->width, mode->height, 0);

        } else {

                glfwSetWindowMonitor(this->window, nullptr,
                                this->win_x, this->win_y,
                                this->win_w, this->win_h, 0);
        }

        glViewport(0, 0, this->win_w, this->win_h);
}

void Engine::update()
{
        this->prev_time = this->cur_time;
        this->cur_time = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        onUpdate(getDeltaTime());

        glfwSwapBuffers(this->window);
        glfwPollEvents();
}

bool Engine::isFullscreen()
{
        return glfwGetWindowMonitor(this->window);
}

bool Engine::shouldRun()
{
        return this->run && !glfwWindowShouldClose(this->window);
}

double Engine::getTime()
{
        return this->cur_time;
}

double Engine::getDeltaTime()
{
        return this->cur_time - this->prev_time;
}

void Engine::keyboardInput(int key, int scan, int action, int mod)
{
}

void Engine::mouseButton(int button, int action, int mod)
{
}

void Engine::setArgs(std::vector<std::string>& args)
{
        this->args = args;
}

void Engine::logError(int code, const char* message)
{
        std::cout << "error[" << code << "]:" << message << std::endl;
}

void GLAPIENTRY Engine::glError(GLenum src, GLenum type,
                                GLenum id, GLenum sev, GLsizei len,
                                const GLchar* msg, const void* up)
{
        const char* ew = (sev == GL_DEBUG_TYPE_ERROR) ? "error: " : "warn: ";
        std::cout << ew << msg << std::endl;
}

void Engine::onKeyboardInput(GLFWwindow* wnd, int key,
                int sc, int action, int mods)
{
        void* up = glfwGetWindowUserPointer(wnd);
        Engine* instance = reinterpret_cast<Engine*>(up);

        if(key == GLFW_KEY_F11 && action == GLFW_PRESS) {

                instance->setFullscreen(!instance->isFullscreen());
        }

        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

                glfwSetWindowShouldClose(wnd, GL_TRUE);
        }

        instance->keyboardInput(key, sc, action, mods);
}

void Engine::onMouseInput(GLFWwindow* wnd, double x, double y)
{
        void* up = glfwGetWindowUserPointer(wnd);
        Engine* instance = reinterpret_cast<Engine*>(up);

        instance->mouse_x = x;
        instance->mouse_y = y;
}

void Engine::onMouseButton(GLFWwindow* wnd, int button, int action, int mods)
{
        void* up = glfwGetWindowUserPointer(wnd);
        Engine* instance = reinterpret_cast<Engine*>(up);

        instance->mouseButton(button, action, mods);
}

