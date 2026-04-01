#include "pch.h"
#include "Entity.h"

Entity::Entity(entt::registry& aRegistry) : myRegistry(aRegistry)
{
	myHandle = aRegistry.create();
}