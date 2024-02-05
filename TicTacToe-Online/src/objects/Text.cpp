#include "Text.h"
#include <SFML/Graphics.hpp>

Text::Text(const char* text, sf::Vector2f position, const char* font, int size, sf::Color color) : GameObject(size, size)
{
	m_Font = new sf::Font();
	m_Font->loadFromFile(font);

	m_String = new sf::String(text);
	m_Text = new sf::Text(*m_String, *m_Font, (unsigned int)size);

	m_Drawable = m_Text;
	m_Transform = m_Text;

	m_Text->setPosition(position);
	m_Text->setFillColor(color);

}

Text::~Text()
{
	delete m_Font;
	delete m_String;
	delete m_Text;
}

void Text::updateText(std::string text)
{
	m_String = new sf::String(text);
	m_Text->setString(*m_String);
}

void Text::setTextStyle(std::vector<sf::Text::Style> styles)
{
	for (int i = 0; i < styles.size(); i++)
		m_Text->setStyle(styles[i]);
}