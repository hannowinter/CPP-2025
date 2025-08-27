#ifndef SMOKEVIEW_HPP
#define SMOKEVIEW_HPP

#include "Layer.hpp"

// Class modeling smoke after alien or player has been hit
class SmokeView {

public:

    // Creates view.
    SmokeView();

    // Draws view.
    void draw(Layer& layer, sf::Vector2f position);

private:

    // Smokes texture.
    sf::Sprite m_smoke;
};

#endif //SMOKEVIEW_HPP