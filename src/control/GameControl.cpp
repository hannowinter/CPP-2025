#include "GameControl.hpp"

#include "PlayerControl.hpp"
#include "AlienControl.hpp"
#include "../model/Constants.hpp"

GameControl::GameControl(ControlList& controls) :
    m_alien_mode{ Alien::SHIFT_RIGHT },
    m_alien_grid_origin{ constants::PADDING, constants::PADDING }
{
    controls.add<PlayerControl>(sf::Vector2f{
       (constants::VIEW_WIDTH - constants::PLAYER_SIZE.x) / 2.0f,
       constants::VIEW_HEIGHT - constants::PLAYER_SIZE.y - constants::PADDING
        });

    for (size_t y = 0; y < constants::ALIEN_COUNT_Y; y++)
    {
        for (size_t x = 0; x < constants::ALIEN_COUNT_X; x++)
        {
            AlienControl& alien = controls.add<AlienControl>(m_alien_grid_origin, x, y);
        }
    }
}

void GameControl::update(const UpdateState& state)
{
    size_t alien_min_col = SIZE_MAX;
    size_t alien_max_col = 0;
    for (const auto& control : state.controls)
    {
        if (AlienControl* alien = control->is<AlienControl>())
        {
            alien_min_col = std::min(alien->get().column(), alien_min_col);
            alien_max_col = std::max(alien->get().column(), alien_max_col);
        }
    }

    float alien_grid_left = alien_min_col * (constants::ALIEN_SIZE.x + constants::ALIEN_SPACING.x);
    float alien_grid_right = alien_max_col * (constants::ALIEN_SIZE.x + constants::ALIEN_SPACING.x) + constants::ALIEN_SIZE.x;

    switch (m_alien_mode)
    {
    case Alien::SHIFT_RIGHT:
        m_alien_grid_origin.x += constants::ALIEN_MOVE_SPEED_X * state.delta;
        if (m_alien_grid_origin.x + alien_grid_right > constants::VIEW_WIDTH - constants::PADDING)
        {
            m_alien_grid_origin.x = constants::VIEW_WIDTH - constants::PADDING - alien_grid_right;
            m_alien_prev_mode = m_alien_mode;
            m_alien_mode = Alien::DESCEND;
        }
        break;
    case Alien::SHIFT_LEFT:
        m_alien_grid_origin.x -= constants::ALIEN_MOVE_SPEED_X * state.delta;
        if (m_alien_grid_origin.x + alien_grid_left < constants::PADDING)
        {
            m_alien_grid_origin.x = constants::PADDING - alien_grid_left;
            m_alien_prev_mode = m_alien_mode;
            m_alien_mode = Alien::DESCEND;
        }
        break;
    case Alien::DESCEND:
        m_alien_grid_origin.y += constants::ALIEN_MOVE_SPEED_Y * state.delta;
        m_alien_descend_timer += state.delta;
        if (m_alien_descend_timer >= constants::ALIEN_DESCEND_DURATION)
        {
            m_alien_descend_timer = 0.0f;
            Alien::Mode new_prev_mode = m_alien_mode;
            if (m_alien_prev_mode == Alien::SHIFT_LEFT)
                m_alien_mode = Alien::SHIFT_RIGHT;
            if (m_alien_prev_mode == Alien::SHIFT_RIGHT)
                m_alien_mode = Alien::SHIFT_LEFT;
            m_alien_prev_mode = new_prev_mode;
        }
        break;
    }
}

sf::Vector2f GameControl::alien_grid_origin() const
{
    return m_alien_grid_origin;
}

void GameControl::draw(Layer& layer)
{

}