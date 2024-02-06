#include "PlayGameState.h"
#include "PauseGameState.h"
#include "GameOverGameState.h"
#include "../engine/rendering/Window.h"
#include "../objects/Background.h"
#include "../objects/Grid.h"
#include "../engine/core/GameManager.h"
#include "../engine/events/MethodCommand.h"
#include "../networking/ClientSocket.h"



PlayGameState::PlayGameState()
{
    Window* pWindow = GameManager::instance().getWindow();
    this->initPlayGameState(pWindow);
}

PlayGameState::PlayGameState(Window* pWindow) 
{
    this->initPlayGameState(pWindow);
}

void PlayGameState::initPlayGameState(Window* pWindow) {
   

    sf::Vector2f fWindowSize = (sf::Vector2f)pWindow->getSFMLObject()->getSize();
    this->spawnGameObject(new Background(fWindowSize.x, fWindowSize.y));

    float fGridSize;
    float fAspectRatio = fWindowSize.x / fWindowSize.y;
    if (fAspectRatio < 1) {
        fGridSize = fWindowSize.x * 0.8f;
    }
    else {
        fGridSize = fWindowSize.y * 0.8f;
    }
    sf::Vector2f fGridMargin;
    fGridMargin.x = (fWindowSize.x - fGridSize) * 0.5f;
    fGridMargin.y = (fWindowSize.y - fGridSize) * 0.5f;

    this->spawnGameObject(new Grid(fGridMargin.x, fGridMargin.y, fGridSize, fGridSize));
}

PlayGameState::~PlayGameState()
{
}

void PlayGameState::switchStateDefeat() {
    //should be wrapped into stateExit() method
    GameManager::instance().getEventsManager()->unsubscribe(GAME_OVER_DEFEAT, new MethodCommand<PlayGameState>(this, &PlayGameState::switchStateDefeat));
    GameManager::instance().getEventsManager()->unsubscribe(KEY_ESCAPE_PRESSED, new MethodCommand<PlayGameState>(this, &PlayGameState::switchStatePause));
	GameManager::instance().switchGameState(new GameOverGameState(true, this));
}

void PlayGameState::switchStatePause() {
    GameManager::instance().getEventsManager()->unsubscribe(GAME_OVER_DEFEAT, new MethodCommand<PlayGameState>(this, &PlayGameState::switchStateDefeat));
    GameManager::instance().getEventsManager()->unsubscribe(KEY_ESCAPE_PRESSED, new MethodCommand<PlayGameState>(this, &PlayGameState::switchStatePause));
    GameManager::instance().switchGameState(new PauseGameState(this));
}

void PlayGameState::update(float deltaTime) {
    GameState::update(deltaTime);
}

void PlayGameState::enter()
{
    GameManager::instance().getEventsManager()->subscribe(GAME_OVER_DEFEAT, new MethodCommand<PlayGameState>(this, &PlayGameState::switchStateDefeat));
    GameManager::instance().getEventsManager()->subscribe(KEY_ESCAPE_PRESSED, new MethodCommand<PlayGameState>(this, &PlayGameState::switchStatePause));
}

