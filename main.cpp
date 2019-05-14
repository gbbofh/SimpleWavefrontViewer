#include <vector>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game.hpp"
#include "engine.hpp"

int main(int argc, char* argv[])
{
        //Engine engine;
        Game engine;

        std::vector<std::string> args;

        /*
         * Parse command-line arguments into a vector for easy processing
         * by the actual application.
         */
        for(int i = 1; i < argc; i++) {

                args.push_back(std::string(argv[i]));
        }

        engine.setArgs(args);

        try {
                /*
                 * Create a window to draw to
                 */
                engine.startUp(800, 600, "Title!", false);

                while(engine.shouldRun()) {

                        engine.update();
                }

                engine.shutDown();

        } catch(EngineError& e) {

                std::cout << "error:" << e.what() << std::endl;
        }
}

