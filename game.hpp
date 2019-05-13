#pragma once

#include "model.hpp"
#include "engine.hpp"
#include "shader.hpp"

class Game : public Engine {

public:
        Game() {}
        ~Game() {}

        void onStart();
        void onShutdown();
        void onUpdate(double dt);

        void keyboardInput(int key, int scan, int action, int mods);
        void mouseButton(int button, int action, int mods);

private:
        Model model;
        ShaderProgram shaderProgram;
};

