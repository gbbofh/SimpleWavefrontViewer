#include "node.hpp"
#include "scene.hpp"

void Scene::setRoot(Node* root)
{
        this->root = root;
        if(root) {

                root->onEnter();

        } else if(!root && this->root) {

                root->onLeave();
        }
}

void Scene::onStartup()
{

        if(this->root) {

                this->root->onEnter();
        }
}

void Scene::onShutdown()
{
        if(this->root) {

                this->root->onLeave();
        }
}

void Scene::onUpdate(double deltaTime)
{
        if(this->root) {

                this->onUpdate(deltaTime);
        }
}

