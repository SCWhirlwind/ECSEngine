#pragma once
#include "Component.h"
#include "AssetManager.h"

class CellComponent : public Component
{

public:
	                                                  
	Vec2 cellPos;
	std::vector<std::shared_ptr<Entity>> entityBucket;

	CellComponent()
	{
	}

	CellComponent(const Vec2& pos) : cellPos(pos)
	{

	}

	~CellComponent()
	{
		entityBucket.clear();
	}
};