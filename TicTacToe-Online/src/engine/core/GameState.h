#pragma once
#include <vector>
#include "../../objects/GameObject.h"

class Window;

class GameState {
public:
	GameState() {};
	~GameState() {};
	virtual void enter() {};
	//virtual void leave() = 0;
	virtual void render(Window* window);
	virtual void update(float deltaTime);
	void spawnGameObject(GameObject* gameObject);
	virtual void killGameObject(GameObject* gameObject);
	void deleteGameObjectsAtEndOfUpdate();

protected:
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> gameObjectsToDelete;
};


