#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

/*
namespace sf
{
	class Transformable;
	class Drawable;
}*/

class GameObject
{
public:
	// CONSTRUCTORS / DESTRUCTORS
	GameObject(float width, float height);
	~GameObject(){};
	//
	virtual void update(float deltaTime) {};

	// GETTERS/SETTERS
	inline sf::Vector2f getPosition() { return m_Transform->getPosition(); };
	inline void setPosition(float x, float y) { m_Transform->setPosition(x, y); };
	inline float getOrientation() { return m_Transform->getRotation(); };
	inline void setOrientation(float angle) { m_Transform->setRotation(angle); };

	inline float getWidth() { return this->m_Size.x; };
	inline float getHeight() { return this->m_Size.y; };

	bool isPointInAABB(float x, float y);
	inline bool isPointInAABB(sf::Vector2f point) { return isPointInAABB(point.x, point.y); };

	inline sf::Drawable* getDrawable() { return this->m_Drawable; };
	inline sf::Transformable* getLocalTransform() { return m_Transform; };

protected:
	sf::Drawable* m_Drawable;
	sf::Transformable* m_Transform;
	sf::Vector2f m_Size;
};