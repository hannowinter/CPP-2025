#include "TestCommon.hpp"
#include "../src/Util.hpp"

TEST(UtilTest, overlapsTest)
{
	constexpr static sf::Rect<float> r11{ { 0.0f, 0.0f }, { 100.0f, 100.0f } };
	constexpr static sf::Rect<float> r12{ { 50.0f, 50.0f }, { 200.0f, 20.0f } };

	constexpr static sf::Rect<float> r21{ { 0.0f, 0.0f }, { 100.0f, 100.0f } };
	constexpr static sf::Rect<float> r22{ { 150.0f, 50.0f }, { 200.0f, 20.0f } };

	// try some different types
	constexpr static sf::Rect<double> r31{ { 0.0, 0.0 }, { 50.0, 200.0 } };
	constexpr static sf::Rect<double> r32{ { 20.0, 200.0 }, { 100.0, 100.0 } };

	constexpr static sf::Rect<int> r41{ { 0, 0 }, { 100, 100 } };
	constexpr static sf::Rect<int> r42{ { 20, 20 }, { 30, 40 } };

	EXPECT_TRUE(overlaps(r11, r12)); 
	EXPECT_FALSE(overlaps(r21, r22));
	EXPECT_FALSE(overlaps(r31, r32)); // edge case, they touch but don't overlap
	EXPECT_TRUE(overlaps(r41, r42)); // containment
}