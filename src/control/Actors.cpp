#include "Actors.hpp"

void ActorList::update(float delta, const Inputs& inputs)
{
	UpdateState update_state{
		.delta = delta,
		.inputs = inputs,
		.actors = *this
	};

	for (const auto& actor : m_actors)
		actor->update(update_state);

	m_actors.insert(
		m_actors.end(),
		std::make_move_iterator(m_actors_to_spawn.begin()), 
		std::make_move_iterator(m_actors_to_spawn.end())
	);
	m_actors_to_spawn.clear();

	m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(), [&](const auto& elem)
		{
			return std::find(m_actors_to_despawn.begin(), m_actors_to_despawn.end(), elem.get()) != m_actors_to_despawn.end();
		}), m_actors.end());
	m_actors_to_despawn.clear();
}

void ActorList::draw(Layer& layer)
{
	for (const auto& actor : m_actors)
		actor->draw(layer);
}

void ActorList::despawn(ActorControl* actor)
{
	m_actors_to_despawn.push_back(actor);
}