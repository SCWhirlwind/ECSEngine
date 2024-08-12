#pragma once
#include "Component.h"
#include <string>

class CardComponent : public Component
{
public:

	enum Rank { Seven = 0, Eight, Nine, Ten, Jack, Queen, King, Ace };
	enum Suit { Red = 0, Black, White };
	
	CardComponent::Rank rank;
	CardComponent::Suit suit;

	std::shared_ptr<Entity> erank;
	std::shared_ptr<Entity> esuit;

	CardComponent()
	{
	}

	CardComponent(CardComponent::Rank r, CardComponent::Suit s) : rank(r), suit(s)
	{
	}

	~CardComponent()
	{
		erank = nullptr;
		esuit = nullptr;
	}
};