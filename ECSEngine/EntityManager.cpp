#include "EntityManager.h"

EntityManager::EntityManager()
{
}

void EntityManager::update()
{

	for (const auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}
	
	m_entitiesToAdd.clear();

	removeDeadEntities(m_entities);

	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}

	
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);
	
	return entity;
}

std::shared_ptr<Entity> EntityManager::getEntity(size_t id)
{
    for (const auto& e : m_entities)
    {
        if (e->id() == id) return e;
    }

    for (const auto& e : m_entitiesToAdd)
    {
        if (e->id() == id) return e;
    }

    return nullptr;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		if (!vec[i]->isActive())
		{
			vec.erase(vec.begin() + i);
		}
	}
}