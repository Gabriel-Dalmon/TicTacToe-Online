#include "PauseGameState.h"
#include "../engine/events/EventsManager.h"
#include "../engine/events/MethodCommand.h"
#include "../engine/core/GameManager.h"

PauseGameState::PauseGameState(GameState* pausedGameState)
{
	this->pauseGameState = pausedGameState;
	GameManager::instance().getEventsManager()->subscribe(KEY_ESCAPE_PRESSED, new MethodCommand<PauseGameState>(this, &PauseGameState::unpauseState));

}

void PauseGameState::render(Window* window)
{
	pauseGameState->render(window);
	GameState::render(window);
}

void PauseGameState::unpauseState() {
	GameManager::instance().getEventsManager()->unsubscribe(KEY_ESCAPE_PRESSED, new MethodCommand<PauseGameState>(this, &PauseGameState::unpauseState));
	GameManager::instance().switchGameState(this->pauseGameState);
	GameManager::instance().getGameState()->enter();
}