#include "PlayerBulletView.hpp"

#include "AssetLoader.hpp"

// Create PlayerBulletView
PlayerBulletView::PlayerBulletView() :
    PlayerProjectileView(AssetLoader::get().player_bullet)
{

}