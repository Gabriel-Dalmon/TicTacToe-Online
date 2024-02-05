#include "GameObject.h"
#include <cstdlib>
#include <SFML/Graphics.hpp>

GameObject::GameObject(float width, float height)
{
	this->m_Size.x = width;
	this->m_Size.y = height;
}
bool GameObject::isPointInAABB(float x, float y)
{
	sf::Vector2f position = m_Transform->getPosition();
	if (x > position.x && x < position.x + m_Size.x && y > position.y && y < position.y + m_Size.y) {
		return true;
	}
	return false;
}
