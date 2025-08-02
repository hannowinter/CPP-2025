#include "PlayerLaser.hpp"

#include "Constants.hpp"

// Create laser projectile
PlayerLaser::PlayerLaser(sf::Vector2f position) :
    PlayerProjectile(position)
{

}

// Get hitbox of projectile
sf::FloatRect PlayerLaser::hitbox() const
{
    return {
        m_position,
         constants::player_bullet::LASER_SIZE
    };
}
