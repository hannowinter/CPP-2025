#ifndef UPGRADEVIEW_HPP
#define UPGRADEVIEW_HPP

#include "Layer.hpp"
#include "../model/Constants.hpp"

class UpgradeView {
public:

    // Creates UpgradeView.
    UpgradeView();

    // Draws upgrade to screen.
    void draw(Layer& layer, constants::upgrades::Weapon weapon, sf::Vector2f position);

private:
    
    // Sprites to be drawn.
    sf::Sprite m_sprite;
};

#endif //UPGRADEVIEW_HPP