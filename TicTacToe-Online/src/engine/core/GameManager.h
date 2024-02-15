#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include "../events/EventsManager.h"
#include "../networking/SocketThread.h"

//class EventsManager;
class Window;
class GameState;

namespace sf
{
	class Mouse;
}

class GameManager
{
public:
	~GameManager() {};
	static GameManager& instance();

	void InitGameManager(Window* pWindow, GameState* initialGameState, SocketThread* pSocketThread);
	void render();
	void update();
	GameState* getGameState();
	void switchGameState(GameState*);

	Socket* CreateSocket();
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
	SocketThread* m_socketThread;
	
};