#include "Symbol.h"
#include "../engine/textures/TextureManager.h"
#include <SFML/Graphics/Sprite.hpp>

Symbol::Symbol(AssetSymbol symbol, float x, float y, float width, float height) : GameObject(width, height)
{
	m_Sprite = new sf::Sprite(*TextureManager::getTexture(symbol == AssetSymbol::CROSS ? "cross.png" : "circle.png"));
	m_Transform = m_Sprite;
	m_Drawable = m_Sprite;
	m_Sprite->setPosition(x, y);
	m_Sprite->setScale(width / m_Sprite->getGlobalBounds().width, height / m_Sprite->getGlobalBounds().height);
}	
