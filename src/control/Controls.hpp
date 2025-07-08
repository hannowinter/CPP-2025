#ifndef CONTROLS_H
#define CONTROLS_H

#include "Inputs.hpp"
#include "../view/Layer.hpp"
#include <vector>
#include <memory>

class ControlList; // forward declaration

struct UpdateState
{
	float delta;
	const Inputs& inputs;
	ControlList& controls;
};

// Base class for all control classes
class Control
{
public:
	virtual ~Control() = default;

	virtual void update(const UpdateState& state) = 0;
	virtual void draw(Layer& layer) = 0;

	template <std::derived_from<Control> C>
	const C* is() const
	{
		return dynamic_cast<const C*>(this);
	}

	template <std::derived_from<Control> C>
	C* is()
	{
		return dynamic_cast<C*>(this);
	}
};

class ControlList
{
	using control_list_t = std::vector<std::unique_ptr<Control>>;

public:
	void update(float delta, const Inputs& inputs);
	void draw(Layer& layer);

	template <std::derived_from<Control> C, typename... ArgTs> requires
		std::is_constructible_v<C, ArgTs...>
	C& add(ArgTs&&... args)
	{
		return dynamic_cast<C&>(
			*m_controls_to_add.emplace_back(std::make_unique<C>(std::forward<ArgTs>(args)...))
		);
	}

	void remove(Control* control);

	template <std::derived_from<Control> C>
	C* get(size_t nth = 0)
	{
		for (const auto& control : m_controls)
		{
			if (C* c = control->is<C>())
			{
				if (nth == 0)
					return c;
				else
					nth--;
			}
		}
		return nullptr;
	}

	typename control_list_t::iterator begin();
	typename control_list_t::iterator end();
	typename control_list_t::const_iterator begin() const;
	typename control_list_t::const_iterator end() const;

private:
	control_list_t m_controls; // currently active controls

	std::vector<std::unique_ptr<Control>> m_controls_to_add; // controls to be added
	std::vector<Control*> m_controls_to_remove; // controls to be removed

	// The `update` method iterates through `m_controls` in order to update each control.
	// During this process, we need to prevent any insertion or erasure of elements to or from `m_controls`,
	// otherwise its iterators may be invalidated, leading to undefined behavior.
	// The `add` and `remove` methods may be called during this iteration process.
	// Instead of immediately inserting or erasing the specified control, we first put them into
	// separate lists `m_controls_to_add` and `m_controls_to_remove` to remember them,
	// and only after the iteration process is done, we insert or erase them to or from `m_controls`.
};

#endif