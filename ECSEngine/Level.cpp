#include "Level.h"
#include <fstream>
#include <iostream>

Level::Level()
{
	Mapsize = Minimumsize;
}

Level::Level(Vec2 mapsize)
{
	if (CheckMapSize(mapsize))
	{
		Mapsize = mapsize;
	}
}

void Level::Resize(Vec2 newsize)
{
	if (CheckMapSize(newsize))
	{
		Mapsize = newsize;
	}
}

std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>>& Level::getLevelMap()
{
	return Levelmap;
}

void Level::SaveLevel()
{
	std::ofstream file;
	file.open("level.txt");

	if (file.is_open())
	{
		std::cout << "file exists!" << std::endl;
		file.close();
	}
	else
	{
		std::cout << "file doesnt exist... creating file" << std::endl;
		std::ofstream newFile("level.txt");
		if (newFile.is_open())
		{
			std::cout << "file is created!" << std::endl;
			newFile.close();
		}
		else
		{
			std::cout << "file could not be created!" << std::endl;
		}
	}
}

void Level::LoadLevel()
{
	std::ofstream file;
	file.open("level.txt");

	if (file.is_open())
	{
		std::cout << "file exists!" << std::endl;
		file.close();
	}
	else
	{
		std::cout << "file doesnt exist... creating file" << std::endl;
		std::ofstream newFile("level.txt");
		if (newFile.is_open())
		{
			std::cout << "file is created!" << std::endl;
			newFile.close();
		}
		else
		{
			std::cout << "file could not be created!" << std::endl;
		}
	}
}

bool Level::CheckMapSize(Vec2 size)
{
	if(size.x >= Minimumsize.x && size.y >= Minimumsize.y)
	{
		return true;
	}
	return false;
}

bool Level::CheckFile(std::string filename)
{
	std::ofstream file;
	file.open("level.txt");

	if (file.is_open())
	{
		std::cout << "file exists!" << std::endl;
		file.close();
		return true;
	}

	return false;
}
