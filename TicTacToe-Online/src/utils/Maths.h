#pragma once
#include <corecrt_math.h>
#include <SFML/System/Vector2.hpp>

namespace Maths
{
	sf::Vector2f vect(float startPointX, float startPointY, float endPointX, float endPointY);
	sf::Vector2f normalize(sf::Vector2f vector);
	inline float getNorm(sf::Vector2f vector) { return sqrtf(powf(vector.x, 2) + powf(vector.y, 2)); };

	inline float convertRadiansToDegrees(float radianAngle) { return radianAngle * 180 / 3.14159265358979323846; };

	bool haveSameSign(int x, int y);
	sf::Vector2f getIntersectionBetweenSegments(sf::Vector2f vertexA, sf::Vector2f vertexB, sf::Vector2f vertexC, sf::Vector2f vertexD);
	sf::Vector2f getAngle(sf::Vector2f vectorOne, sf::Vector2f vectorTwo);

}