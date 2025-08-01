#ifndef SMOKEVIEW_HPP
#define SMOKEVIEW_HPP

#include "Layer.hpp"

// Class modeling smoke after alien or player has been hit
class SmokeView {

public:

    // Create view
    SmokeView();

    // Draw view
    void draw(Layer& layer, sf::Vector2f position);

private:

    // Smoke texture
    sf::Sprite m_smoke;
};

#endif //SMOKEVIEW_HPP