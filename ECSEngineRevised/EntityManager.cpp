#include "EntityManager.h"

EntityManager::EntityManager() {}

void EntityManager::update()
{

	for (const auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(std::move(e));
		m_entityMap[m_entities.back()->tag()].push_back(m_entities.back());
	}

	m_entitiesToAdd.clear();

	removeDeadEntities(m_entities);

	for (auto it = m_entityMap.begin(); it != m_entityMap.end();)
	{
		auto& vec = it->second;

		removeDeadEntities(it->second);
		if (it->second.empty())
			it = m_entityMap.erase(it);
		else
			++it;
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::make_shared<Entity>(m_totalEntities++, tag);

	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities() const
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag) const
{
	auto it = m_entityMap.find(tag);
	if (it != m_entityMap.end())
		return it->second;
	static EntityVec emptyVec;
	return emptyVec;
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	vec.erase(std::remove_if(vec.begin(), vec.end(), 
		[](const std::shared_ptr<Entity>& e) { return !e->isActive(); }), 
		vec.end());
}