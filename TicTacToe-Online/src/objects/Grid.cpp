#include "Grid.h"
#include "Symbol.h"

#include "../engine/core/GameManager.h"
#include "../engine/textures/TextureManager.h"
#include "../engine/events/EventsManager.h"
#include "../engine/core/GameState.h"
#include "../engine/events/MethodCommand.h"



#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

Grid::Grid(float x, float y, float width, float height) : GameObject(width, height)
{
	m_Sprite = new sf::Sprite(*TextureManager::getTexture("grid.png"));
	m_Transform = m_Sprite;
	m_Drawable = m_Sprite;
	m_Sprite->setPosition(x, y);
	m_Sprite->setScale(width / m_Sprite->getGlobalBounds().width, height / m_Sprite->getGlobalBounds().height);

	GameManager::instance().getEventsManager()->subscribe(EventName::MOUSE_LEFT_PRESSED, new MethodCommand<Grid>(this, &Grid::onMouseClick));
}

void Grid::onMouseClick()
{
	sf::Vector2i mousePosition = GameManager::instance().getMousePosition();
	if (!this->isPointInAABB((sf::Vector2f)mousePosition)) {
		return;
	}

	sf::Vector2f gridPosition = this->getPosition();
	float cellSize = this->getWidth() / 3;

	// can be replaced with euclydian division (mousePos - gridPos / gridSize), which probably computes faster
	int column = (mousePosition.x - gridPosition.x) / (this->getWidth() / 3);
	int row = (mousePosition.y - gridPosition.y) / (this->getHeight() / 3);
	if (m_GridState[column * m_GridSize + row] != 0) {
		GameManager::instance().getEventsManager()->trigger(EventName::CELL_ALREADY_TAKEN);
		return;
	}
	m_GridState[column * m_GridSize + row] = m_CurrentPlayer;
	Symbol* symbol = new Symbol(m_CurrentPlayer == 1 ? AssetSymbol::CROSS : AssetSymbol::CIRCLE, gridPosition.x + column * cellSize, gridPosition.y + row * cellSize, cellSize, cellSize);
	GameManager::instance().getGameState()->spawnGameObject(symbol);
	m_CurrentPlayer = -m_CurrentPlayer;
	WinDirection winDirection = this->isMoveWinning();
	if (winDirection != WinDirection::NOT_WINNING) {
		GameManager::instance().getEventsManager()->trigger(EventName::GAME_OVER_VICTORY);
		std::cout << "GAME OVER \n";
		return;
	}
	GameManager::instance().getEventsManager()->trigger(EventName::MOVE_PLAYED);
}

// Specific for 3x3 grid in 1D array and win condition being to align 3 symbols.
WinDirection Grid::isMoveWinning()
{
	int lDiagSum = m_GridState[0] + m_GridState[4] + m_GridState[8];
	if (lDiagSum == 3 || lDiagSum == -3) {
		return LEFT_DIAGONAL;
	}

	int rDiagSum = m_GridState[2] + m_GridState[4] + m_GridState[6];
	if (rDiagSum == 3 || rDiagSum == -3) {
		return RIGHT_DIAGONAL;
	}

	int rowSum, columnSum, rowOffset;
	for (int i = 0; i < 3; i++) {
		rowOffset = i * m_GridSize;
		rowSum = m_GridState[rowOffset] + m_GridState[rowOffset + 1] + m_GridState[rowOffset + 2];
		columnSum = m_GridState[i] + m_GridState[3 + i] + m_GridState[6 + i];
		if (rowSum == 3 || rowSum == -3) {
			return (WinDirection)i;
		}
		else if (columnSum == 3 || columnSum == -3) {
			return (WinDirection)(3 + i);
		}
	}
	return WinDirection::NOT_WINNING;
}

