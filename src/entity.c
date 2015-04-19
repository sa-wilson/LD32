#include "entity.h"

Entity* createEntity()
{
	Entity* entity = NULL;

	entity = (Entity*) malloc(sizeof(Entity));

	if (!entity) return NULL;

	entity->sprite = NULL;
	entity->pos = createVec2(0, 0);
	entity->vel = createVec2(0, 0);
	entity->accel = createVec2(0, 0);
	entity->dAccel = createVec2(0, 0);

	return entity;
}

EntityError Entity_Init(Entity* entity, Sprite* sprite, vec2 pos)
{
	if (!entity)
		return ENTITY_NULLPTR;

	entity->sprite = sprite;
	entity->pos = pos;

	return ENTITY_OK;
}

void Entity_Update(Entity* entity)
{
	entity->pos.x += entity->vel.x;
	entity->pos.y += entity->vel.y;

}

void Entity_Render(Entity* entity)
{

}