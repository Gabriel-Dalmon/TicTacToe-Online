#include "Maths.h"
#include <cmath>

#define FLT_MAX          3.402823466e+38F

/// <param name="startPointX :"> The first point's X coordinate in the plan</param>
/// <param name="startPointY :"> The first point's Y coordinate in the plan</param>
/// <param name="endPointX :"> The second point's X coordinate in the plan</param>
/// <param name="endPointY :" > The second point's Y coordinate in the plan</param>
/// <returns>A sf::Vector2f with x and y coordinates</returns>
sf::Vector2f Maths::vect(float startPointX, float startPointY, float endPointX, float endPointY)
{
	sf::Vector2f vector = { endPointX - startPointX, endPointY - startPointY };
	return vector;
}

sf::Vector2f Maths::normalize(sf::Vector2f vector)
{
	float norm = getNorm(vector);
	sf::Vector2f normalizedVector = { vector.x / norm, vector.y / norm };
	return normalizedVector;
}

bool Maths::haveSameSign(int x, int y) {
	return ((x ^ y) >= 0);
}

sf::Vector2f Maths::getIntersectionBetweenSegments(sf::Vector2f vertexA, sf::Vector2f vertexB, sf::Vector2f vertexC, sf::Vector2f vertexD) {
	float tNumerator = (vertexA.x - vertexC.x) * (vertexC.y - vertexD.y) - (vertexA.y - vertexC.y) * (vertexC.x - vertexD.x);

	float uNumerator = (vertexA.x - vertexC.x) * (vertexA.y - vertexB.y) - (vertexA.y - vertexC.y) * (vertexA.x - vertexB.x);
	float denominator = (vertexA.x - vertexB.x) * (vertexC.y - vertexD.y) - (vertexA.y - vertexB.y) * (vertexC.x - vertexD.x);

	float absoluteDenominator = std::abs(denominator);

	if (Maths::haveSameSign(tNumerator, denominator)
		&& Maths::haveSameSign(tNumerator, denominator)
		&& (std::abs(tNumerator) <= absoluteDenominator)
		&& (std::abs(uNumerator) <= absoluteDenominator)
		)
	{
		float t = tNumerator / denominator;
		return { vertexA.x + t * (vertexB.x - vertexA.x), vertexA.y + t * (vertexB.y - vertexA.y) };
	}
	return { FLT_MAX , FLT_MAX };
}