#pragma once
#include <unordered_map>
#include "Entity.h"
#include "Vec2.h"

class Level
{
public:

	Level();
	Level(Vec2 mapsize);

	void Resize(Vec2 newsize);

	std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>>& getLevelMap();

	void SaveLevel();
	void LoadLevel();

private:

	bool CheckMapSize(Vec2 size);

	bool CheckFile(std::string  filename);

	Vec2 Mapsize;
	Vec2 Minimumsize = Vec2(20, 16);
	std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>> Levelmap;
};

