#pragma once

#include <memory>
#include <bitset>
#include <string>
#include <vector>
#include <array>
#include "Component.h"

using ComponentID = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}


constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Entity
{
	friend class EntityManager;

private:

	bool m_active = true;
	std::string m_tag = "default";
	size_t m_id = 0;
	std::vector<std::unique_ptr<Component>> m_components;

	ComponentArray componentArray {};
	ComponentBitSet componentBitset {};

	Entity(const size_t id, const std::string& tag);

public:

	void destroy();
	size_t id() const;
	bool isActive() const;
	const std::string& tag() const;

	template <typename T> bool hasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto c = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
		T* ptr = c.get();
		m_components.emplace_back(std::move(c));

		componentArray[getComponentTypeID<T>()] = ptr;
		componentBitset[getComponentTypeID<T>()] = true;

		return *ptr;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	template<typename T> void removeComponent()
	{
		const ComponentID id = getComponentTypeID<T>();
		if (!componentBitset[id]) return;

		Component* const ptr = componentArray[id];

		m_components.erase(
			std::remove_if(m_components.begin(), m_components.end(),
				[ptr](const std::unique_ptr<Component>& c) { return c.get() == ptr; }),
			m_components.end());

		componentArray[id] = nullptr;
		componentBitset[id] = false;
	}
};