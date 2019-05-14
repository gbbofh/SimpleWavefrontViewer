#include "node.hpp"

void Node::addChild(Node* node)
{
        this->children.push_back(node);
}

void Node::addChild(Node* node, int index)
{
        std::vector<Node*>::iterator it;

        it = this->children.begin();
        this->children.insert(it + index, node);
}

void Node::removeChild(int index)
{
        std::vector<Node*>::iterator it;

        it = this->children.begin();
        this->children.erase(it + index);
}

void Node::onEnter()
{
        for(Node* node : this->children) {

                node->onEnter();
        }
}

void Node::onLeave()
{
        for(Node* node : this->children) {

                node->onLeave();
        }
}

void Node::onUpdate(double deltaTime)
{
        for(Node* node : this->children) {

                node->onUpdate(deltaTime);
        }
}

