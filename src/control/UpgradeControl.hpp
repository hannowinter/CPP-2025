#ifndef UPGRADECONTROL_HPP
#define UPGRADECONTROL_HPP

#include "Controls.hpp"

class UpgradeControl : public Control {
public:

    // Abstract methods of parent class
    void init(const ControlList& controls) override;
    void update(const UpdateState& state) override;
    void draw(LayerManager& layers) override;

private:
};

#endif //UPGRADECONTROL_HPP