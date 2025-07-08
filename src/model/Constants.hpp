#include <SFML/Graphics.hpp>

namespace constants 
{
    constexpr int VIEW_WIDTH = 960;
    constexpr int VIEW_HEIGHT = 720;
    constexpr sf::FloatRect VIEW_RECT = { { 0.0f, 0.0f }, { VIEW_WIDTH, VIEW_HEIGHT } };
    constexpr unsigned int FRAME_RATE = 60;

    constexpr float PADDING = 10.0f; // minimum distance of the player to the view's border

    constexpr float PLAYER_MOVE_SPEED = 300.0f;
    constexpr sf::Vector2f PLAYER_SIZE = { 60.0f, 30.0f };

    constexpr float BULLET_MOVE_SPEED = 800.0f;
    constexpr sf::Vector2f BULLET_SIZE = { 5.0f, 10.0f };

    constexpr float ALIEN_MOVE_SPEED_X = 30.0f;
    constexpr float ALIEN_MOVE_SPEED_Y = 15.0f;
    constexpr sf::Vector2f ALIEN_SIZE = { 40.0f, 32.0f };
    constexpr sf::Vector2f ALIEN_SPACING = { 20.0f, 20.0f };
    constexpr float ALIEN_DESCEND_DURATION = 0.5f;
    constexpr size_t ALIEN_COUNT_X = 10;
    constexpr size_t ALIEN_COUNT_Y = 6;
}