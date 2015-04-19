#pragma once

#include "main.h"

struct sprite
{
	GLint texId;
};
typedef struct sprite Sprite;

void Sprite_update(int x, int y);
void Sprite_render();