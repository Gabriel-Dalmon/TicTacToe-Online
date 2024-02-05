#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>


/// <summary>
/// Class Wrapping the sf::RenderWindow into a custom Class
/// </summary>
class Window {
public:
	Window(int width, int height, const char* title = "New Project");
	~Window();
	void closeWindow();
	void clearWindow();
	void drawOnWindow(sf::Drawable* drawable);
	void display();
	inline sf::RenderWindow* getSFMLObject() { return &window; };

private:
	sf::RenderWindow window;
};