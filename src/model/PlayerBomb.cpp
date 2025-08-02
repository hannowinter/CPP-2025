#include "PlayerBomb.hpp"

#include "Constants.hpp"

// Create bomb projectile
PlayerBomb::PlayerBomb(sf::Vector2f position) :
    PlayerProjectile(position)
{

}

// Get hotbox of projectile
sf::FloatRect PlayerBomb::hitbox() const
{
    return {
        m_position,
        constants::player_bullet::BOMB_SIZE
    };
}
