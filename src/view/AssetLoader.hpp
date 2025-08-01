#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <SFML/Graphics.hpp>
#include <array>

// Struct to hold all relevant textures (Singleton)
struct AssetLoader
{
	const sf::Texture player;
	const sf::Texture player_hit;
	const std::array<sf::Texture, 2> yellow;
	const std::array<sf::Texture, 2> green;
	const std::array<sf::Texture, 2> red;
	const sf::Texture alien_bullet;
	const sf::Texture smoke;
	const sf::Texture bomb;
	const sf::Texture laser;
	const sf::Texture explosion;

	AssetLoader();

	static const AssetLoader& get();

private:
	static std::unique_ptr<AssetLoader> inst;
};

#endif