#pragma once

#include <vector>

class Node {

public:
        void addChild(Node* node);
        void addChild(Node* node, int index);

        void removeChild(int index);

        void onEnter();
        void onLeave();
        void onUpdate(double deltaTime);

private:
        Node* parent;
        std::vector<Node*> children;
};

