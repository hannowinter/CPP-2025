#include <SFML/Graphics.hpp>

namespace constants 
{
    constexpr int VIEW_WIDTH = 600;
    constexpr int VIEW_HEIGHT = 600;
    constexpr sf::FloatRect VIEW_RECT = { { 0.0f, 0.0f }, { VIEW_WIDTH, VIEW_HEIGHT } };
    constexpr unsigned int FRAME_RATE = 60;

    constexpr float PADDING = 10.0f;

    constexpr float PLAYER_MOVE_SPEED = 300.0f;
    constexpr sf::Vector2f PLAYER_SIZE = { 60.0f, 30.0f };

    constexpr float BULLET_MOVE_SPEED = 800.0f;
    constexpr sf::Vector2f BULLET_SIZE = { 5.0f, 10.0f };
}