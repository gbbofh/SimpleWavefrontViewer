#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <stdexcept>

class EngineError : public std::runtime_error {

public:
        explicit EngineError(const std::string& what) :
                std::runtime_error(what) {}
        explicit EngineError(const char*  what) :
                std::runtime_error(what) {}
};

class Engine {

public:

        virtual ~Engine() {}

        virtual void onStart() {}
        virtual void onShutdown() {}
        virtual void onUpdate(double dt) {}

        void startUp(int width, int height, const char* title, bool fullscreen);
        void shutDown();

        void update();
        void setFullscreen(bool fullscreen);

        bool shouldRun();
        bool isFullscreen();

        double getTime();
        double getDeltaTime();

        void keyboardInput(int key, int scan, int action, int mod);
        void mouseButton(int button, int action, int mod);

        void setArgs(std::vector<std::string>& args);

//private:
protected:
        bool run;

        int win_x;
        int win_y;
        int win_w;
        int win_h;

        double mouse_x;
        double mouse_y;

        double prev_mouse_x;
        double prev_mouse_y;

        double delta_mouse_x;
        double delta_mouse_y;

        double cur_time;
        double prev_time;

        GLFWwindow* window;
        GLFWmonitor* monitor;

        std::vector<std::string> args;

        static void logError(int code, const char* message);
        static void glError(GLenum src, GLenum type,
                        GLenum id, GLenum sev, GLsizei len,
                        const GLchar* msg, const void* up);
        static void onKeyboardInput(GLFWwindow* wnd, int key,
                        int sc, int action, int mods);
        static void onMouseInput(GLFWwindow* wnd, double x, double y);
        static void onMouseButton(GLFWwindow* wnd, int button,
                        int action, int mods);
};

