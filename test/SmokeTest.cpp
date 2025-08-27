#include "TestCommon.hpp"
#include "../src/control/SmokeControl.hpp"

// Tests whether the smoke disappears after its lifetime.
TEST(SmokeControlTest, lifetimeTest)
{
	ControlList controls;
	SmokeControl& smoke = controls.request_add<SmokeControl>(sf::Vector2f{});
	controls.execute_requests();

	// check if smoke gets despawned after some time
	constexpr size_t LIFETIME_UPDATES =
		constants::alien::EFFECT_DURATION / FIXED_DELTA_60 +
		2; // add a bit of leniency

	for (size_t i = 0; i < LIFETIME_UPDATES; i++)
	{
		smoke.update_lifetime(FIXED_DELTA_60, controls);
		controls.execute_requests();
	}

	EXPECT_EQ(controls.count<SmokeControl>(), 0);
}