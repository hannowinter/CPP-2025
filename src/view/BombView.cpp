#include "BombView.hpp"

#include "AssetLoader.hpp"

// Create BombView
BombView::BombView() :
    PlayerProjectileView(AssetLoader::get().bomb)
{

}

// Make bomb explode
void BombView::explode()
{
    // Set explosion texture and resize bounding rectangle
    m_sprite.setTexture(AssetLoader::get().explosion, true);
    m_sprite.setScale({2.0f, 2.0f});
}
