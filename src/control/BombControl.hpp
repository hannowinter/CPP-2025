#ifndef BOMBCONTROL_HPP
#define BOMBCONTROL_HPP

#include "PlayerProjectileControl.hpp"

class BombControl : public PlayerProjectileControl {
public:

    // Create BombControl
    BombControl(sf::Vector2f position);
};

#endif //BOMBCONTROL_HPP