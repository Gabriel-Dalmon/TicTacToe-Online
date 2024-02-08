#include "GameManager.h"
#include "GameState.h"
#include "../events/EventsManager.h"
#include "../rendering/Window.h"

#include "iostream"

GameManager* GameManager::instance_ = NULL;



void GameManager::InitGameManager(Window* pWindow, GameState* pInitialGameState) {
    this->window = pWindow;
    this->gameState = pInitialGameState;
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

void GameManager::switchGameState(GameState* gameState, SwitchGameStateFlags eSwitchGameStateFlags = (SwitchGameStateFlag)0)
{
    if (eSwitchGameStateFlags & COPY_GAME_OBJECTS) {
        gameState->copyGameObjects(m_GameState);
        eSwitchGameStateFlags &= ~DELETE_GAME_OBJECTS;
    }
    if (eSwitchGameStateFlags & DELETE_GAME_OBJECTS) {
        m_GameState->deleteAllGameObjects();
    }
    if (eSwitchGameStateFlags & DELETE_OLD_GAMESTATE) {
		delete m_GameState;
	}

    std::cout << "Switching game state" << std::endl;
    this->gameState = gameState;
}

sf::Vector2i GameManager::getMousePosition()
{
    sf::Vector2i mousePosition = GameManager::mouse->getPosition(*GameManager::window->getSFMLObject());
    return mousePosition;
}