#pragma once
#include "GameObject.h"
#include <vector>

namespace sf {
	class Sprite;
}

typedef enum WinDirections {
	ROW_0 = 0,
	ROW_1,
	ROW_2,
	COLUMN_0,
	COLUMN_1,
	COLUMN_2,
	LEFT_DIAGONAL,
	RIGHT_DIAGONAL,
	NOT_WINNING,
	WIN_DIRECTIONS_COUNT
} WinDirection;

class Grid : public GameObject
{
public:
	Grid(float x, float y, float width, float height);
	~Grid() {};
	void onMouseClick();
	void playMove(int index);
	WinDirection isMoveWinning();


private:
	int m_GridState[9] = {0,0,0, 0,0,0, 0,0,0};
	int m_GridSize = 3;
	int m_CurrentPlayer = 1;
	sf::Sprite* m_Sprite;

	int index = 0;
	int column;
	int row;
};

