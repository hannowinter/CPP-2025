#include "PlayerProjectileView.hpp"

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
