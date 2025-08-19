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

#endif //PLAYERPROJECTILE_HPP