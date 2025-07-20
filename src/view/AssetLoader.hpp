#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <SFML/Graphics.hpp>
#include <array>

struct AssetLoader
{
	const sf::Texture player;
	const std::array<sf::Texture, 2> yellow;
	const std::array<sf::Texture, 2> green;
	const std::array<sf::Texture, 2> red;
	const sf::Texture alien_bullet;

	AssetLoader();

	static const AssetLoader& get();

private:
	static std::unique_ptr<AssetLoader> inst;
};

#endif