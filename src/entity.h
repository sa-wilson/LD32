#pragma once

#include "main.h"
#include "sprite.h"
#include "vec2.h"

struct entity
{
	Sprite* sprite;
	vec2 pos;
	vec2 vel;
	vec2 accel;
	vec2 dAccel;
};
typedef struct entity Entity;

enum entityError
{
	ENTITY_OK,
	ENTITY_NULLPTR
};
typedef enum entityError EntityError;

Entity* createEntity();
EntityError Entity_Init(Entity* entity, Sprite* sprite, vec2 pos);
void Entity_Update(Entity* entity);
void Entity_Render(Entity* entity);

void Entity_setPos(int x, int y);
void Entity_setVel(int x, int y);
void Entity_setAccel(int x, int y);
