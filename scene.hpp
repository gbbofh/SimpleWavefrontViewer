#pragma once

#include <vector>

class Node;

class Scene {

public:
        void setRoot(Node* root);

        void onStartup();
        void onShutdown();
        void onUpdate(double deltaTime);

private:
        Node* root;
};

