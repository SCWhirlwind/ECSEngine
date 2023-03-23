#pragma once
#include <string>
class Action
{
public:
	Action() = default;
	Action(const std::string& name, const std::string& type);

	const std::string& getType() const;
	const std::string& getName() const;

private:
	std::string m_name;
	std::string m_type;
};

