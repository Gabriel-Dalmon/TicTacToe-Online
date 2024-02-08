#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include "../events/EventsManager.h"
//class EventsManager;
class Window;
class GameState;

namespace sf
{
	class Mouse;
}

namespace SwitchGameStateFlags {
	const int DELETE_OLD_GAMESTATE = 1 << 0;  // 0001
	const int DELETE_GAME_OBJECTS = 1 << 1;   // 0010
	const int COPY_GAME_OBJECTS = 1 << 2;     // 0100
}

namespace SwitchGameStateFlag = SwitchGameStateFlags;

class GameManager
{
public:
	~GameManager() {};
	static GameManager& instance();

	void InitGameManager(Window* pWindow, GameState* pInitialGameState);
	void render();
	void update();
	GameState* getGameState();
	void switchGameState(GameState* pGameState, int iSwitchGameStateFlags);


	EventsManager* getEventsManager();
	Window* getWindow();
	sf::Vector2i getMousePosition();

private:
	GameManager() {};
	static GameManager* instance_;

	GameState* gameState;
	void updateDeltaTime();
	sf::Clock* clock = new sf::Clock();
	float deltaTime = 0;
	Window* window;
	sf::Mouse* mouse;
	
	EventsManager* eventsManager = new EventsManager();
	
};