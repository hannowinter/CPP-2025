#include "BombView.hpp"

#include "AssetLoader.hpp"

// Create BombView
BombView::BombView() :
    PlayerProjectileView(AssetLoader::get().bomb)
{

}