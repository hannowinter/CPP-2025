#include "PlayerProjectile.hpp"

#include "Constants.hpp"

// Create projectile
PlayerProjectile::PlayerProjectile(sf::Vector2f position) :
    m_position { position }
{

}

// Make projectile move up
void PlayerProjectile::move_up(float delta)
{
    m_position.y -= constants::player_bullet::MOVE_SPEED * delta;
}
