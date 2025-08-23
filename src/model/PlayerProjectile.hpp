#ifndef PLAYERPROJECTILE_HPP
#define PLAYERPROJECTILE_HPP

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

// Abstract class for different types of player ammunition
class PlayerProjectile {
public:
    // Methods for all types of ammunition
    virtual ~PlayerProjectile() = default;

    // Create ammunition object
    PlayerProjectile(sf::Vector2f position);

    // Get hitbox of ammunition
    virtual sf::FloatRect hitbox() const = 0;

    // Make projectile move up
    virtual void move_up(float delta);

protected:

    // Position of ammunition
    sf::Vector2f m_position;
};

// Class modelling a bullet shot by the player
class PlayerBullet : public PlayerProjectile
{
public:
    // Create player bullet
    PlayerBullet(sf::Vector2f position);

    // Get hitbox of bullet
    sf::FloatRect hitbox() const override;
};

// Class modelling a laser shot by the player
class PlayerLaser : public PlayerProjectile 
{
public:

    // Create laser projectile
    PlayerLaser(sf::Vector2f position);

    // Get hotbox of projectile
    sf::FloatRect hitbox() const override;
};

// Class modelling a bomb shot by the player
class PlayerBomb : public PlayerProjectile 
{
public:

    // Create bomb projectile
    PlayerBomb(sf::Vector2f position);

    // Make bomb explode
    void explode();

    // Get hitbox
    sf::FloatRect hitbox() const override;

private:

    // Remember if bomb has exploded
    bool m_has_exploded = false;
};

#endif //PLAYERPROJECTILE_HPP