#include "AssetLoader.hpp"

AssetLoader::AssetLoader() :
	player{ "assets/player.png" },
	yellow{ "assets/yellow.png" },
	green{ "assets/green.png" },
	red{ "assets/red.png" } 
{

}

const AssetLoader& AssetLoader::get()
{
	if (!inst)
		inst = std::make_unique<AssetLoader>();
	return *inst;
}

std::unique_ptr<AssetLoader> AssetLoader::inst;