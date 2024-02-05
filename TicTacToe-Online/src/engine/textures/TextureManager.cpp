#include "TextureManager.h"
#include <SFML/Graphics.hpp>

std::unordered_map<std::string, sf::Texture*> TextureManager::textures;

sf::Texture* TextureManager::getTexture(const std::string& fileName)
{
	auto it = textures.find(fileName);
	if (it != textures.end())
	{
		return it->second;
	}

	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile("data/assets/graphics/images/" + fileName);
	textures[fileName] = texture;

	return texture;
}

void TextureManager::destroyTexture()
{
	for (auto texturePointer : TextureManager::textures) {
		delete texturePointer.second;
	}
}
