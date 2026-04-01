#pragma once
#include "entt/entt.hpp"

class Entity
{
public:
	Entity(entt::registry& aRegistry);
	~Entity() = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... aArgs)
	{
		if (HasComponent<T>())
		{
			LOG_WARNING("Trying to add a component that already exists on entity.");
			return GetComponent<T>();
		}

		return myRegistry.emplace<T>(myHandle, std::forward<Args>(aArgs)...);
	}

	template<typename T>
	void RemoveComponent()
	{
		if (!HasComponent<T>())
		{
			LOG_ERROR("Trying to remove a component the entity doesn't have.");
			ASSERT(false);
		}

		myRegistry.remove<T>(myHandle);
	}

	template<typename T>
	T& GetComponent()
	{
		if (!HasComponent<T>())
		{
			LOG_ERROR("Trying to fetch a component the entity doesn't have.");
			ASSERT(false);
		}

		return myRegistry.get<T>(myHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return myRegistry.any_of<T>(myHandle);
	}

private:
	entt::entity myHandle;
	entt::registry& myRegistry;
};