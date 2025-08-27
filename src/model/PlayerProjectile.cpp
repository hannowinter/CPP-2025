#include "PlayerProjectile.hpp"

#include "Constants.hpp"

// Creates projectile.
PlayerProjectile::PlayerProjectile(sf::Vector2f position) :
    m_position { position }
{

}

// Makes projectile move up.
void PlayerProjectile::move_up(float delta)
{
    m_position.y -= constants::player_projectile::MOVE_SPEED * delta;
}



// ==============================================================



// Creates new bullet at position.
PlayerBullet::PlayerBullet(sf::Vector2f position) :
	PlayerProjectile(position)
{

}

// Gets hitbox of bullet.
sf::FloatRect PlayerBullet::hitbox() const
{
	return {
		m_position,
		constants::player_projectile::BULLET_SIZE
	};
}



// ==============================================================



// Creates laser projectile.
PlayerLaser::PlayerLaser(sf::Vector2f position) :
    PlayerProjectile(position)
{

}

// Gets hitbox of projectile.
sf::FloatRect PlayerLaser::hitbox() const
{
    return {
        m_position,
         constants::player_projectile::LASER_SIZE
    };
}



// ==============================================================



// Creates bomb projectile.
PlayerBomb::PlayerBomb(sf::Vector2f position) :
    PlayerProjectile(position)
{

}

// Makes bomb explode.
void PlayerBomb::explode()
{
    m_has_exploded = true;
}


// Gets hotbox of projectile.
sf::FloatRect PlayerBomb::hitbox() const
{
    // Bomb has not yet exploded
    if (!m_has_exploded)
        return {
            m_position,
            constants::player_projectile::BOMB_SIZE
    };
    else // Bomb has exploded
        return {
            m_position,
            constants::player_projectile::EXPLOSION_SIZE
    };
}
