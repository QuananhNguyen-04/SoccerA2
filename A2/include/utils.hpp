#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <algorithm>
#include <stdio.h>

void normalize_velocity(float& vx, float& vy, float max_speed);
void apply_friction(float& vx, float& vy, float df);
float clamp(float value, float min, float max);
float distance(float x, float y, float x1, float y1);
#endif