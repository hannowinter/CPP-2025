#include "Controls.hpp"

void ControlList::update(float delta, const Inputs& inputs)
{
	UpdateState update_state{
		.delta = delta,
		.inputs = inputs,
		.controls = *this
	};

	for (const auto& control : m_controls)
		control->update(update_state);

	m_controls.insert(
		m_controls.end(),
		std::make_move_iterator(m_controls_to_add.begin()),
		std::make_move_iterator(m_controls_to_add.end())
	);
	m_controls_to_add.clear();

	m_controls.erase(std::remove_if(m_controls.begin(), m_controls.end(), [&](const auto& elem)
		{
			return std::find(
				m_controls_to_remove.begin(),
				m_controls_to_remove.end(),
				elem.get()
			) != m_controls_to_remove.end();
		}), m_controls.end());
	m_controls_to_remove.clear();
}

void ControlList::draw(Layer& layer)
{
	for (const auto& control : m_controls)
		control->draw(layer);
}

void ControlList::remove(Control* control)
{
	m_controls_to_remove.push_back(control);
}

typename ControlList::control_list_t::iterator ControlList::begin()
{
	return m_controls.begin();
}

typename ControlList::control_list_t::iterator ControlList::end()
{
	return m_controls.end();
}

typename ControlList::control_list_t::const_iterator ControlList::begin() const
{
	return m_controls.begin();
}

typename ControlList::control_list_t::const_iterator ControlList::end() const
{
	return m_controls.end();
}