#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <SFML/Graphics.hpp>

struct AssetLoader
{
	const sf::Texture player;
	const sf::Texture yellow;
	const sf::Texture green;
	const sf::Texture red;

	AssetLoader();

	static const AssetLoader& get();

private:
	static std::unique_ptr<AssetLoader> inst;
};

#endif