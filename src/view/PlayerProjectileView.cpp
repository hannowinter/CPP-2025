#include "PlayerProjectileView.hpp"
#include "AssetLoader.hpp"

PlayerProjectileView::PlayerProjectileView(const sf::Texture& sprite) :
    m_sprite { sprite }
{

}

void PlayerProjectileView::draw(Layer& layer, const PlayerProjectile& projectile)
{
    // Set position / size of texture and add to layer
    m_sprite.setPosition(projectile.hitbox().position);
    layer.add_to_layer(m_sprite);
}



// ==========================================================



// Creates PlayerBulletView.
PlayerBulletView::PlayerBulletView() :
    PlayerProjectileView(AssetLoader::get().player_bullet)
{

}



// ==========================================================



// Creates BombView.
LaserView::LaserView() :
    PlayerProjectileView(AssetLoader::get().laser_bullet)
{

}



// ==========================================================



// Creates BombView.
BombView::BombView() :
    PlayerProjectileView(AssetLoader::get().bomb)
{

}

// Makes bomb explode.
void BombView::explode()
{
    // Set explosion texture and resize bounding rectangle
    m_sprite.setTexture(AssetLoader::get().explosion, true);
    m_sprite.setScale({ 2.0f, 2.0f });
}
