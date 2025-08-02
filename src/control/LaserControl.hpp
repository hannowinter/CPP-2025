#ifndef LASERCONTROL_HPP
#define LASERCONTROL_HPP

#include "PlayerProjectileControl.hpp"

class LaserControl : public PlayerProjectileControl {
public:

    // Create LaserControl
    LaserControl(sf::Vector2f position);
};

#endif //LASERCONTROL_HPP