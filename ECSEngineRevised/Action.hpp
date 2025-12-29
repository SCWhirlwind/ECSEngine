#pragma once
#include <string>
class Action
{
public:
	Action() = default;
	explicit Action(const std::string& name, const std::string& type) : m_name(name), m_type(type) {};

	const std::string& getType() const { return m_type; }
	const std::string& getName() const { return m_name; }

	bool operator==(const Action& other) const {
		return m_name == other.m_name && m_type == other.m_type;
	}

	bool operator!=(const Action& other) const {
		return !(*this == other);
	}

private:
	std::string m_name;
	std::string m_type;
};

