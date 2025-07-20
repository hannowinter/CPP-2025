#include "AssetLoader.hpp"

AssetLoader::AssetLoader() :
	player{ "assets/player.png" },
	yellow{ sf::Texture{ "assets/yellow_0.png" }, sf::Texture{ "assets/yellow_1.png" } },
	green{ sf::Texture{ "assets/green_0.png" }, sf::Texture{ "assets/green_1.png" } },
	red{ sf::Texture{ "assets/red_0.png" }, sf::Texture{ "assets/red_1.png" } },
	alien_bullet{ "assets/bullet_alien.png" }
{

}

const AssetLoader& AssetLoader::get()
{
	if (!inst)
		inst = std::make_unique<AssetLoader>();
	return *inst;
}

std::unique_ptr<AssetLoader> AssetLoader::inst;