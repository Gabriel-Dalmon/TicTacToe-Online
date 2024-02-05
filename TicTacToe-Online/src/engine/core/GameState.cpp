#include "GameState.h"
#include "../rendering/Window.h"
#include <iostream>

void GameState::render(Window* window) {
    for (int i = 0; i < this->gameObjects.size(); i++)
    {
        window->drawOnWindow(this->gameObjects[i]->getDrawable());
    }
}

void GameState::update(float deltaTime) {
    for (int i = 0; i < this->gameObjects.size(); i++)
    {
        gameObjects[i]->update(deltaTime);
    }
}

void GameState::spawnGameObject(GameObject* gameObject)
{
	this->gameObjects.push_back(gameObject);
}

void GameState::killGameObject(GameObject* gameObject)
{
    this->gameObjects.erase(std::remove(this->gameObjects.begin(), this->gameObjects.end(), gameObject), this->gameObjects.end());
    this->gameObjectsToDelete.push_back(gameObject);
}

void GameState::deleteGameObjectsAtEndOfUpdate() {
    
    for (GameObject* gameObject : this->gameObjectsToDelete) {
        this->gameObjectsToDelete.erase(std::remove(this->gameObjectsToDelete.begin(), this->gameObjectsToDelete.end(), gameObject), this->gameObjectsToDelete.end());
        delete gameObject;
    }
}