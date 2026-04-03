#include "pch.h"
#include "Entity.h"

Entity::Entity(entt::registry& aRegistry, const std::string& aName) : myRegistry(aRegistry), myName(aName)
{
	myHandle = aRegistry.create();
}

const std::string& Entity::GetName()
{
	return myName;
}

const entt::entity& Entity::GetHandle()
{
	return myHandle;
}
