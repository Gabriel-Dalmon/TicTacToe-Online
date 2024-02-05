#include "GameOverGameState.h"
#include "../objects/Text.h"

GameOverGameState::GameOverGameState(bool isDefeat, GameState* pausedGameState) : PauseGameState(pausedGameState)
{
	switch (isDefeat)
	{
	case true:
		this->spawnGameObject(new Text("Defeat", { 250, 200 }, "data/assets/graphics/fonts/Roboto-Regular.ttf", 80, sf::Color::White));
		break;
	case false:
		this->spawnGameObject(new Text("Win", { 250, 200 }, "data/assets/graphics/fonts/Roboto-Regular.ttf", 80, sf::Color::White));
		break;
	}
}
