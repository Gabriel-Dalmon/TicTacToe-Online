#include "Window.h"
#include "../../engine/core/GameManager.h"
#include "../events/EventsManager.h"
#include "../events/MethodCommand.h"

Window::Window(int width, int height, const char* title) : window(sf::VideoMode(width, height), title) {
	GameManager::instance().getEventsManager()->subscribe(CLOSE_WINDOW_BUTTON, new MethodCommand<Window>(this, &Window::closeWindow));
}

Window::~Window()
{
}

void Window::closeWindow() {
	this->window.close();
}

void Window::clearWindow() {
	this->window.clear();
}

void Window::drawOnWindow(sf::Drawable* drawable) {
	this->window.draw(*drawable);
}

void Window::display() {
	this->window.display();
}