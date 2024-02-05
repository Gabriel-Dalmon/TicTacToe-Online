#pragma once
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <string>

namespace sf
{
	class Texture;
	class Sprite;
}

class TextureManager
{
public:
	static sf::Texture* getTexture(const std::string& fileName);
	static void destroyTexture();

private:
	static std::unordered_map<std::string, sf::Texture*> textures;
};

