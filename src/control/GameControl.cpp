#include "GameControl.hpp"

#include "AlienControl.hpp"
#include "PlayerControl.hpp"
#include "AudioPlayer.hpp"
#include "SmokeControl.hpp"
#include "UpgradeControl.hpp"
#include "../model/Constants.hpp"

// Create GameController
GameControl::GameControl() :
    m_random{ static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count() 
    ) }
{

}

// Move to next level
void GameControl::increment_level()
{
    m_state.level++;
    m_state.lives = constants::game::INITIAL_LIVES;
    m_state.over = false;
    m_state.level_won = false;

    m_background.reset_direction(random());
}

// Reset game to beginning
void GameControl::reset_game()
{
    m_state.level = 10;
    m_state.lives = constants::game::INITIAL_LIVES;
    m_state.score = 0;
    m_state.over = false;
    m_state.level_won = false;

    m_background.reset_direction(random());
}

// Add child controllers
void GameControl::add_children(ControlList& controls)
{
    // Add PlayerController to list of controllers
    controls.request_add<PlayerControl>(sf::Vector2f{
        (constants::VIEW_WIDTH - constants::player::SIZE.x) / 2.0f,
        constants::VIEW_HEIGHT - constants::player::SIZE.y - constants::PADDING
    });

    // Add AlienGridController to list of controllers
    controls.request_add<AlienGridControl>();

    // Add UpgradeControl to list of controllers
    controls.request_add<UpgradeControl>();
}


// Initialize this controller
void GameControl::init(const ControlList& controls)
{
    m_background.reset_direction(random());
}

// Execute relevant updates
void GameControl::update(const UpdateState& state)
{
    m_background.update(state.delta, intensity());

    // Check if game is over (no lives left or aliens reached bottom)
    AlienGridControl* grid = state.controls.get<AlienGridControl>();
    if (grid != nullptr && (m_state.lives == 0 || grid->get_bottom() >= constants::VIEW_HEIGHT - 100.0f) && !m_gameover_shown)
    {
        // Play sound
        AudioPlayer::get().game_over.play();

        // Get PlayerController
        PlayerControl* player_control = state.controls.get<PlayerControl>();

        // Create smoke at player position
        state.controls.request_add<SmokeControl>(player_control->get().hitbox().position);
        player_control->hide();

        m_state.over = true;
        m_gameover_shown = true;
    }

    // Check if game should be restarted
    if (m_state.over && state.inputs.held_keys.contains(sf::Keyboard::Key::Space))
    {
        // Remove all controllers from list
        for (const auto& control : state.controls)
        {
            if (!control->is<GameControl>())
                state.controls.request_remove(control.get());
        }

        reset_game();
        add_children(state.controls);

        m_gameover_shown = false;
    }

    // Check if all alien have been shot and player can advance to next level
    if (state.controls.count<AlienControl>() == 0 && !m_victory_shown)
    {
        // Play sound
        AudioPlayer::get().level_won.play();

        m_state.level_won = true;
        m_victory_shown = true;
    }

    // Check if game should continue
    if (m_state.level_won && state.inputs.held_keys.contains(sf::Keyboard::Key::Space) && m_state.level < constants::game::MAX_LEVEL)
    {
        // Remove all controllers from list
        for (const auto& control : state.controls)
        {
            if (!control->is<GameControl>())
                state.controls.request_remove(control.get());
        }

        increment_level();
        add_children(state.controls);

        m_victory_shown = false;
    }
    else if (m_state.level_won && state.inputs.held_keys.contains(sf::Keyboard::Key::Space) && m_state.level == constants::game::MAX_LEVEL)
    {
        // Remove all controllers from list
        for (const auto& control : state.controls)
        {
            if (!control->is<GameControl>())
                state.controls.request_remove(control.get());
        }

        reset_game();
        add_children(state.controls);

        m_victory_shown = false;
    }
}

// Draw HUD
void GameControl::draw(LayerManager& layers)
{
    m_hud.draw(layers.get(LayerID::HUD), m_state);
    m_background.draw(layers.get(LayerID::BACKGROUND));
}

float GameControl::intensity() const
{
    // Calculate intensity
    // "ratio == 0.0" initially, "ratio == 1.0" in level 10
    float ratio = (float)(m_state.level - 1) / (constants::game::MAX_LEVEL - 1);
    ratio = std::pow(ratio, 2.0f); // make the intensity increase more gentle initially and steeper towards the end
    return std::lerp(
        constants::alien_grid::MIN_INTENSITY,
        constants::alien_grid::MAX_INTENSITY,
        ratio
    );
}

// Get pseudo-random number generator
std::mt19937& GameControl::random()
{
    return m_random;
}

// Get current state
GameState& GameControl::state()
{
    return m_state;
}

// Get current state
const GameState& GameControl::state() const
{
    return m_state;
}