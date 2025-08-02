#include "LaserView.hpp"

#include "AssetLoader.hpp"

// Create BombView
LaserView::LaserView() :
    PlayerProjectileView(AssetLoader::get().laser_bullet)
{

}