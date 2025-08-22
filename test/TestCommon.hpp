#ifndef TESTCOMMON_H
#define TESTCOMMON_H

#include <gtest/gtest.h>
#include "../src/model/Constants.hpp"

constexpr float EPS = 0.001f;

constexpr float FIXED_DELTA_30 = 1.0f / 30; // 30 fps
constexpr float FIXED_DELTA_60 = 1.0f / 60; // 60 fps
constexpr float FIXED_DELTA_120 = 1.0f / 120; // 120 fps

#endif