#include "PlayerBomb.hpp"

#include "Constants.hpp"

// Create bomb projectile
PlayerBomb::PlayerBomb(sf::Vector2f position) :
    PlayerProjectile(position)
{

}

// Make bomb explode
void PlayerBomb::explode()
{
    m_has_exploded = true;
}


// Get hotbox of projectile
sf::FloatRect PlayerBomb::hitbox() const
{
    // Bomb has not yet exploded
    if (!m_has_exploded)
        return {
            m_position,
            constants::player_bullet::BOMB_SIZE
        };
    else // Bomb has exploded
        return {
            m_position,
            constants::player_bullet::EXPLOSION_SIZE
        };
}
