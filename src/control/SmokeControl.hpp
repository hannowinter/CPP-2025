#ifndef SMOKECONTROL_HPP
#define SMOKECONTROL_HPP

#include "Control.hpp"
#include "../view/SmokeView.hpp"

class SmokeControl : public Control {

public:
    // Creates control at position.
    SmokeControl(sf::Vector2f position);

    // Abstract methods of parent class
    void init(const ControlList& controls) override;
    void update(const UpdateState& state) override;
    void draw(LayerManager& layers) override;

private:

    // View
    SmokeView m_smoke_view;

    // Position of smoke
    sf::Vector2f m_position;

    // Remaining lifetime
    float m_lifetime;
};

#endif //SMOKECONTROL_HPP