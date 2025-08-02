#include "UpgradeView.hpp"

#include "AssetLoader.hpp"

// Create UpgradeView
// Initialize with random texture
UpgradeView::UpgradeView() :
    m_sprite { AssetLoader::get().smoke }
{

}

// Draw upgrade to screen
void UpgradeView::draw(Layer& layer, constants::player::Weapon weapon, sf::Vector2f position)
{
    // Select correct texture and reset bounding rectangle
    if (weapon == constants::player::Weapon::LASER)
        m_sprite.setTexture(AssetLoader::get().laser_upgrade, true);
    else // weapon == constants::player::Weapon::BOMB
        m_sprite.setTexture(AssetLoader::get().bomb, true);

    m_sprite.setPosition(position);
    layer.add_to_layer(m_sprite);
}