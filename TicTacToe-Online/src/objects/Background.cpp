#include "Background.h"
#include "../engine/textures/TextureManager.h"
#include <SFML/Graphics/Sprite.hpp>

Background::Background(float width, float height) : GameObject(width, height)
{
	m_Sprite = new sf::Sprite(*TextureManager::getTexture("background.png"));
	m_Transform = m_Sprite;
	m_Drawable = m_Sprite;
	m_Sprite->setScale(width / m_Sprite->getGlobalBounds().width, height / m_Sprite->getGlobalBounds().height);
}