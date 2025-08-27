#include "TestCommon.hpp"
#include "../src/control/UpgradeControl.hpp"
#include "../src/control/PlayerControl.hpp"
#include "../src/control/GameControl.hpp"

constexpr size_t ROUNDS = 20;

// Mock GameControl that creates PlayerControl at specified position
class MockGameControl : public GameControl
{
public:
    MockGameControl(sf::Vector2f player_position) : m_player_position(player_position) {}

    void add_children(ControlList& controls) override
    {
        controls.request_add<GameControl>(*this);
        controls.request_add<PlayerControl>(m_player_position);
        controls.request_add<UpgradeControl>();
    }

private:
    sf::Vector2f m_player_position;
};

struct UpgradeTest : public testing::Test
{
protected:
    UpgradeTest() :
        player{ sf::Vector2f{} }
    {
        random.seed(0); // make it deterministic
    }

    ControlList controls;
    PlayerControl player;
    UpgradeControl upgrade;
    std::mt19937 random;
};

// Test that item spawns on the far side when player is positioned on left side of screen
TEST_F(UpgradeTest, spawnsOnFarSidePlayerLeft)
{
    constexpr sf::Vector2f PLAYER_LEFT_POS{ 0.0f, 0.0f };

    player = PlayerControl{ PLAYER_LEFT_POS };

    for (size_t i = 0; i < ROUNDS; i++)
    {
        sf::Vector2f spawn_pos = upgrade.determine_random_position(player, random);
        upgrade.spawn_at(spawn_pos, constants::upgrades::Weapon::BOMB);

        sf::FloatRect hitbox = upgrade.hitbox();

        // Item should spawn on right side
        float player_right_edge = PLAYER_LEFT_POS.x + constants::player::SIZE.x;

        EXPECT_GT(hitbox.position.x, player_right_edge);
    }
}

// Test that item spawns on the far side when player is positioned on right side of screen
TEST_F(UpgradeTest, spawnsOnFarSidePlayerRight)
{
    constexpr sf::Vector2f PLAYER_RIGHT_POS{ constants::VIEW_WIDTH, 0.0f };

    player = PlayerControl{ PLAYER_RIGHT_POS };

    for (size_t i = 0; i < ROUNDS; i++)
    {
        sf::Vector2f spawn_pos = upgrade.determine_random_position(player, random);
        upgrade.spawn_at(spawn_pos, constants::upgrades::Weapon::BOMB);

        sf::FloatRect hitbox = upgrade.hitbox();

        // Debug output
        /*std::cout << "Player at x=" << PLAYER_RIGHT_POS.x << std::endl;
        std::cout << "Player width=" << constants::player::SIZE.x << std::endl;
        std::cout << "Player spans from " << PLAYER_RIGHT_POS.x << " to " << (PLAYER_RIGHT_POS.x + constants::player::SIZE.x) << std::endl;
        std::cout << "Item spawned at x=" << hitbox.position.x << std::endl;
        std::cout << "Item width=" << hitbox.size.x << std::endl;
        std::cout << "Item spans from " << hitbox.position.x << " to " << (hitbox.position.x + hitbox.size.x) << std::endl;
        std::cout << "Expected: item right edge < player left edge" << std::endl;
        std::cout << "Expected: " << (hitbox.position.x + hitbox.size.x) << " < " << PLAYER_RIGHT_POS.x << std::endl;
        std::cout << "Result: " << ((hitbox.position.x + hitbox.size.x) < PLAYER_RIGHT_POS.x) << std::endl;*/

        // Item should spawn on left side
        EXPECT_LT(hitbox.position.x + hitbox.size.x, PLAYER_RIGHT_POS.x);
    }
}

// Test that item always spawns within screen boundaries
TEST_F(UpgradeTest, alwaysSpawnsWithinBounds)
{
    std::vector<sf::Vector2f> test_positions = {
        { 0.0f, 0.0f },                             // extreme left
        { 50.0f, 0.0f },                            // left side
        { constants::VIEW_WIDTH / 2.0f, 0.0f },     // center
        { constants::VIEW_WIDTH - 50.0f, 0.0f },    // right side
        { constants::VIEW_WIDTH, 0.0f }             // extreme right
    };

    for (const auto& position : test_positions)
    {
        for (size_t i = 0; i < ROUNDS; i++)
        {
            player = PlayerControl{ position };

            sf::Vector2f spawn_pos = upgrade.determine_random_position(player, random);
            upgrade.spawn_at(spawn_pos, constants::upgrades::Weapon::BOMB);

            sf::FloatRect hitbox = upgrade.hitbox();

            // Item should be within boundaries
            EXPECT_GE(hitbox.position.x, constants::PADDING);
            EXPECT_LE(hitbox.position.x + hitbox.size.x, constants::VIEW_WIDTH - constants::PADDING);
        }
    }
}