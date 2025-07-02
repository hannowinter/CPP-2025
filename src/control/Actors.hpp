#ifndef ACTORS_H
#define ACTORS_H

#include "Inputs.hpp"
#include "../view/Layer.hpp"
#include <vector>
#include <memory>

class ActorList; // forward declaration

struct UpdateState
{
	float delta;
	const Inputs& inputs;
	ActorList& actors;
};

// Base class for all control classes representing some kind of actor (player, bullet, alien, ...)
class ActorControl
{
public:
	virtual ~ActorControl() = default;

	virtual void update(const UpdateState& state) = 0;
	virtual void draw(Layer& layer) = 0;
};

class ActorList
{
public:
	void update(float delta, const Inputs& inputs);
	void draw(Layer& layer);

	template <std::derived_from<ActorControl> A> requires
		std::is_constructible_v<A, sf::Vector2f>
	void spawn(sf::Vector2f position)
	{
		m_actors_to_spawn.push_back(std::make_unique<A>(position));
	}

	void despawn(ActorControl* actor);

private:
	std::vector<std::unique_ptr<ActorControl>> m_actors; // currently active actors

	std::vector<std::unique_ptr<ActorControl>> m_actors_to_spawn; // actors to be spawned
	std::vector<ActorControl*> m_actors_to_despawn; // actors to be despawned

	// The `update` method iterates through `m_actors` in order to update each actor.
	// During this process, we need to prevent any insertion or erasure of elements to or from `m_actors`,
	// otherwise its iterators may be invalidated, leading to undefined behavior.
	// The `spawn` and `despawn` methods may be called during this iteration process.
	// Instead of immediately inserting or erasing the specified actor, we put them into
	// separate lists `m_actors_to_spawn` and `m_actors_to_despawn` first to remember them,
	// and insert or erase them to or from `m_actors` after the iteration process is done.
};

#endif