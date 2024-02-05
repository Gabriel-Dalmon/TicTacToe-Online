#include "GameManager.h"
#include "GameState.h"
#include "../events/EventsManager.h"
#include "../rendering/Window.h"

#include "iostream"

GameManager* GameManager::instance_ = NULL;



void GameManager::InitGameManager(Window* pWindow, GameState* initialGameState) {
    this->window = pWindow;
    this->gameState = initialGameState;
}

GameManager& GameManager::instance() 
{    
    /*
    static GameManager* instanceA = new GameManager();
    return *instanceA;*/

    if (!instance_) { 
        instance_ = new GameManager(); 
    };
    return *instance_;
}

void GameManager::render()
{
    this->window->clearWindow();
    this->gameState->render(this->window);
    this->gameState->deleteGameObjectsAtEndOfUpdate();
    this->window->display();
}


void GameManager::update()
{
    this->updateDeltaTime();
    this->eventsManager->handleSFMLEvents();
    this->gameState->update(this->deltaTime);
}

void GameManager::updateDeltaTime() {
    this->deltaTime = this->clock->getElapsedTime().asSeconds();
    this->clock->restart();
}

Window* GameManager::getWindow()
{
    return this->window;
}

EventsManager* GameManager::getEventsManager() 
{
    return this->eventsManager;
}

GameState* GameManager::getGameState()
{
    return this->gameState;
}

void GameManager::switchGameState(GameState* gameState)
{
    std::cout << "Switching game state" << std::endl;
    this->gameState = gameState;
}

sf::Vector2i GameManager::getMousePosition()
{
    sf::Vector2i mousePosition = GameManager::mouse->getPosition(*GameManager::window->getSFMLObject());
    return mousePosition;
}