#pragma once
#include "GameObject.h"


namespace sf
{
	class Sprite;
}

typedef enum AssetSymbols {
	CIRCLE = 0,
	CROSS,
	SYMBOLS_COUNT
} AssetSymbol;

class Symbol : public GameObject
{
public:
	Symbol(AssetSymbol symbol, float x, float y, float width, float height);
	~Symbol() { delete m_Sprite; };

private:
	sf::Sprite* m_Sprite;
};