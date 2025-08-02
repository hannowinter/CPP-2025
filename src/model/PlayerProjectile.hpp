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

    // Check if constant PlayerProjectile is of specified type and cast
    template <std::derived_from<PlayerProjectile> C>
    const C* is() const
    {
        return dynamic_cast<const C*>(this);
    }

    // Check if non-constant PlayerProjectile is of specified type and cast
    template <std::derived_from<PlayerProjectile> C>
    C* is()
    {
        return dynamic_cast<C*>(this);
    }

protected:

    // Position of ammunition
    sf::Vector2f m_position;
};

#endif //PLAYERPROJECTILE_HPP