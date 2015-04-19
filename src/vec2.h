#pragma once

struct vec2
{
	int x;
	int y;
};
typedef struct vec2 vec2;

vec2 createVec2(int x, int y)
{
	vec2 vec = { x, y };
	return vec;
}