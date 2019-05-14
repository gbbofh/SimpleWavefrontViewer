#include "engine.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @brief               Initialize GLFW, GLEW, and create a window to draw to.
 * @param width         Default width of the window
 * @param height        Default height of the window
 * @param title         String displayed in the titlebar
 * @param fullscreen    If true, window is created as fullscreen
 */
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

        /*
         * Set the user pointer to be a reference to this class instance.
         * This done to get around the requirement that the callback functions
         * be static.
         */
        glfwSetWindowUserPointer(this->window, this);

        glfwSetKeyCallback(this->window,
                        Engine::onKeyboardInput);
        glfwSetCursorPosCallback(this->window,
                        Engine::onMouseInput);
        glfwSetMouseButtonCallback(this->window,
                        Engine::onMouseButton);
        glfwSetFramebufferSizeCallback(this->window,
                        Engine::onFramebufferResize);
        glfwSetWindowPosCallback(this->window,
                        Engine::onWindowMoved);
        glfwSetWindowSizeCallback(this->window,
                        Engine::onWindowResized);

        /*
         * Lock the aspect ratio and limit the size.
         */
        glfwSetWindowAspectRatio(this->window, width, height);
        glfwSetWindowSizeLimits(this->window, width / 4, height / 4,
                        GLFW_DONT_CARE, GLFW_DONT_CARE);

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

/**
 * @brief Destroy the window instance, and terminate glfw.
 */
void Engine::shutDown()
{
        onShutdown();

        if(this->window) {

                glfwDestroyWindow(this->window);
        }

        glfwTerminate();
}

/**
 * @brief               toggle fullscreen mode for the glfw window
 * @param fullscreen    whether or not the window should be fullscreen
 */
void Engine::setFullscreen(bool fullscreen)
{
        int width;
        int height;
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


        glfwGetFramebufferSize(this->window, &width, &height);

        glViewport(0, 0, width, height);
}

/**
 * @brief       performs an update cycle
 */
void Engine::update()
{
        /* Update timestep information */
        this->prev_time = this->cur_time;
        this->cur_time = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Call the update callback */
        onUpdate(getDeltaTime());

        glfwSwapBuffers(this->window);
        glfwPollEvents();
}

/**
 * @brief       checks to see if the window is in fullscreen mode or not
 * @return      returns true if the windows monitor pointer is null
 */
bool Engine::isFullscreen()
{
        return glfwGetWindowMonitor(this->window) != nullptr;
}

/**
 * @brief       checks to see if we should still be running the main loop
 * @return      returns true if this->run is true, and the window isn't closing
 */
bool Engine::shouldRun()
{
        return this->run && !glfwWindowShouldClose(this->window);
}

/**
 * @brief       gets the current timestep information
 * @return      returns the time at which the current frame began
 */
double Engine::getTime()
{
        return this->cur_time;
}

/**
 * @brief       gets the change in time between the previous frame and this one
 * @return      returns how much time has elapsed between two frames
 */
double Engine::getDeltaTime()
{
        return this->cur_time - this->prev_time;
}

/**
 * @brief               callback for keyboard input
 * @param key           the key which was pressed
 * @param scan          the scancode for the key which was pressed
 * @param action        whether the key is held, pressed, or released
 * @param mod           any modifying keys (alt, ctrl, shift) which are pressed
 */
void Engine::keyboardInput(int key, int scan, int action, int mod)
{
}

/**
 * @brief               callback for mouse input
 * @param button        the mouse which was pressed
 * @param action        whether the key is held, pressed, or released
 * @param mod           any modifying keys (alt, ctrl, shift) which are pressed
 */
void Engine::mouseButton(int button, int action, int mod)
{
}

/**
 * @brief       sets the argument vector
 * @param args  the arguments supplied to the application
 */
void Engine::setArgs(std::vector<std::string>& args)
{
        this->args = args;
}

/**
 * @brief               log an error from glfw to stderr
 * @param code          the error number
 * @param message       the message to log
 */
void Engine::logError(int code, const char* message)
{
        std::cerr << "error[" << code << "]:" << message << std::endl;
}

/**
 * @brief       log an error from OpenGL to stderr
 * @param src   source of the error
 * @param id    id of the message (?)
 * @param type  type of message (error or warning)
 * @param sev   how severe the message is
 * @param len   length of the message
 * @param msg   message string
 * @param up    user generated data
 */
void GLAPIENTRY Engine::glError(GLenum src, GLenum type,
                                GLenum id, GLenum sev, GLsizei len,
                                const GLchar* msg, const void* up)
{
        const char* ew = (type == GL_DEBUG_TYPE_ERROR) ? "error[" : "warn[";
        std::cerr << ew << "severity=" << sev << " ";
        std::cerr << "type=" << type << " ";
        std::cerr << "]: " << msg << std::endl;
}

/**
 * @brief               handles user keyboard input
 * @param wnd           the window which received the input
 * @param key           the key which was pressed
 * @param sc            the scancode corresponding to key
 * @param action        whether the key was pressed, held or released
 * @param mods          any keyboard modifiers which were pressed
 */
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

/**
 * @brief       handles user mouse movement
 * @param wnd   the window which received the input
 * @param x     the x position of the mouse
 * @param y     the y position of the mouse
 */
void Engine::onMouseInput(GLFWwindow* wnd, double x, double y)
{
        void* up = glfwGetWindowUserPointer(wnd);
        Engine* instance = reinterpret_cast<Engine*>(up);

        instance->mouse_x = x;
        instance->mouse_y = y;
}

/**
 * @brief               handles button presses on the mouse
 * @param button        the button which was pressed
 * @param action        whether the button was pressed, held or released
 * @param mods          any modifier keys
 */
void Engine::onMouseButton(GLFWwindow* wnd, int button, int action, int mods)
{
        void* up = glfwGetWindowUserPointer(wnd);
        Engine* instance = reinterpret_cast<Engine*>(up);

        instance->mouseButton(button, action, mods);
}

/**
 * @brief               resizes the viewport in relation to the window
 * @param wnd           the window being resized
 * @param width         the width of the framebuffer after resize
 * @param height        the height of the framebuffer after resize
 */
void Engine::onFramebufferResize(GLFWwindow* wnd, int width, int height)
{
        glViewport(0, 0, width, height);
}

/**
 * @brief       stores the current window positions when it is moved
 * @param wnd   the window being moved
 * @param x     the x position of the window
 * @param y     the y position of the window
 */
void Engine::onWindowMoved(GLFWwindow* wnd, int x, int y)
{
        void* up = glfwGetWindowUserPointer(wnd);
        Engine* instance = reinterpret_cast<Engine*>(up);

        instance->win_x = x;
        instance->win_y = y;
}

/**
 * @brief               resizes the viewport in relation to the window
 * @param wnd           the window being resized
 * @param width         the width of the window after resize
 * @param height        the height of the window after resize
 */
void Engine::onWindowResized(GLFWwindow* wnd, int width, int height)
{
        void* up = glfwGetWindowUserPointer(wnd);
        Engine* instance = reinterpret_cast<Engine*>(up);

        instance->win_w = width;
        instance->win_h = height;
}

