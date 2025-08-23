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



// ==============================================================



// Create new bullet at position of shooting
PlayerBullet::PlayerBullet(sf::Vector2f position) :
	PlayerProjectile(position)
{

}

// Get hitbox of bullet
sf::FloatRect PlayerBullet::hitbox() const
{
	return {
		m_position,
		constants::player_bullet::BULLET_SIZE
	};
}



// ==============================================================



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



// ==============================================================



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
