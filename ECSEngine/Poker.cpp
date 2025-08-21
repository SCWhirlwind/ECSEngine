#include "Poker.h"


std::shared_ptr<Entity>* Poker::sortHand(std::shared_ptr<Entity> hand[])
{
	for (int i = 0; i < 5 - 1; i++)
	{
		for (int j = i + 1; j < 5; j++)
		{
			if (hand[i]->getComponent<CardComponent>().rank > hand[j]->getComponent<CardComponent>().rank)
			{
				std::swap(hand[i], hand[j]);
			}
		}
	}
	return hand;
}

std::pair<std::string, int> Poker::checkHand(std::shared_ptr<Entity> hand[])
{
	if (isFiveOfAKind(hand))
	{
		return std::make_pair("Five of a kind", 80);
	}
	else if (isRoyalFlush(hand))
	{
		return std::make_pair("Royal flush", 60);
	}
	else if (isFourOfAKind(hand))
	{
		return std::make_pair("Four of a kind", 50);

	}
	else if (isStraightFlush(hand))
	{
		return std::make_pair("Straight flush", 40);
	}
	else if (isStraight(hand))
	{
		return std::make_pair("Straight", 30);
	}
	else if (isFullHouse(hand))
	{
		return std::make_pair("Fullhouse", 25);
	}
	else if (isFlush(hand))
	{
		return std::make_pair("Flush", 20);
	}
	else if (isThreeOfAKind(hand))
	{
		return std::make_pair("Three of a kind", 15);
	}
	else if (isTwoPair(hand))
	{
		return std::make_pair("Two Pair", 10);
	}
	else if (isOnePair(hand))
	{
		return std::make_pair("One Pair", 5);
	}
	else if (isHighCard(hand))
	{
		return std::make_pair("High Card", 1);
	}
	return std::make_pair("Error", -1);
}

int Poker::checkHighCard(std::shared_ptr<Entity> hand[], int handtype)
{
	switch (handtype)
	{
		case 1:
		{
			return hand[4]->getComponent<CardComponent>().rank;
		}
		case 5:
		{
			for (int i = 0; i < 4; i++)
			{
				if (hand[i]->getComponent<CardComponent>().rank == hand[i + 1]->getComponent<CardComponent>().rank)
				{
					return hand[i]->getComponent<CardComponent>().rank;
				}
			}
			break;
		}
		case 10:
		{
			for (int i = 4; i > 0; i--)
			{
				if (hand[i]->getComponent<CardComponent>().rank == hand[i - 1]->getComponent<CardComponent>().rank)
				{
					return hand[i]->getComponent<CardComponent>().rank;
				}
			}
			break;
		}
		case 15:
		{
			for (int i = 0; i < 4; i++)
			{
				if (hand[i]->getComponent<CardComponent>().rank == hand[i + 1]->getComponent<CardComponent>().rank)
				{
					return hand[i]->getComponent<CardComponent>().rank;
				}
			}
			break;
		}
		case 20:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		case 25:
		{
				if (hand[0]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank &&
					hand[1]->getComponent<CardComponent>().rank == hand[2]->getComponent<CardComponent>().rank)
				{
					return hand[0]->getComponent<CardComponent>().rank;
				}
				else
				{
					return hand[4]->getComponent<CardComponent>().rank;
				}
		}
		case 30:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		case 40:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		case 50:
		{
			if (hand[0]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank)
			{
				return hand[3]->getComponent<CardComponent>().rank;
			}
			else
			{
				return hand[4]->getComponent<CardComponent>().rank;
			}
		}
		case 60:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		case 80:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		default:
		{
			break;
		}
	}
	return -1;
}



bool Poker::isFiveOfAKind(std::shared_ptr<Entity> hand[])
{
	if (hand[0]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank &&
		hand[1]->getComponent<CardComponent>().rank == hand[2]->getComponent<CardComponent>().rank &&
		hand[2]->getComponent<CardComponent>().rank == hand[3]->getComponent<CardComponent>().rank && 
		hand[3]->getComponent<CardComponent>().rank == hand[4]->getComponent<CardComponent>().rank)
	{
		return true;
	}
	return false;
}

bool Poker::isRoyalFlush(std::shared_ptr<Entity> hand[])
{
	if (hand[0]->getComponent<CardComponent>().rank == CardComponent::Ten &&
		hand[1]->getComponent<CardComponent>().rank == CardComponent::Jack &&
		hand[2]->getComponent<CardComponent>().rank == CardComponent::Queen &&
		hand[3]->getComponent<CardComponent>().rank == CardComponent::King &&
		hand[4]->getComponent<CardComponent>().rank == CardComponent::Ace &&
		
		isFlush(hand))
	{
		return true;
	}
	return false;
}

bool Poker::isFourOfAKind(std::shared_ptr<Entity> hand[])
{
	if (hand[1]->getComponent<CardComponent>().rank ==
		hand[2]->getComponent<CardComponent>().rank &&
		hand[2]->getComponent<CardComponent>().rank ==
		hand[3]->getComponent<CardComponent>().rank)
	{
		if (hand[0]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank ||
			hand[3]->getComponent<CardComponent>().rank == hand[4]->getComponent<CardComponent>().rank)
		{
			return true;
		}
	}
	return false;
}

bool Poker::isStraightFlush(std::shared_ptr<Entity> hand[])
{
	if (isStraight(hand) && isFlush(hand))
	{
		return true;
	}
	return false;
}

bool Poker::isStraight(std::shared_ptr<Entity> hand[])
{
	for (int i = 0; i < 4; i++)
	{
		if (hand[i]->getComponent<CardComponent>().rank != hand[i+1]->getComponent<CardComponent>().rank - 1)
		{
			return false;
		}
	}
	
	return true;
}

bool Poker::isFullHouse(std::shared_ptr<Entity> hand[])
{
	if (isThreeOfAKind(hand))
	{
		if (hand[2]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank)
		{
			if (hand[3]->getComponent<CardComponent>().rank == hand[4]->getComponent<CardComponent>().rank)
			{
				return true;
			}
		}
		else
		{
			if (hand[0]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank)
			{
				return true;
			}
		}
	}
	return false;
}

bool Poker::isFlush(std::shared_ptr<Entity> hand[])
{
	if (hand[0]->getComponent<CardComponent>().suit == hand[1]->getComponent<CardComponent>().suit &&
		hand[1]->getComponent<CardComponent>().suit == hand[2]->getComponent<CardComponent>().suit &&
		hand[2]->getComponent<CardComponent>().suit == hand[3]->getComponent<CardComponent>().suit &&
		hand[3]->getComponent<CardComponent>().suit == hand[4]->getComponent<CardComponent>().suit)
	{
		return true;
	}
	return false;
}

bool Poker::isThreeOfAKind(std::shared_ptr<Entity> hand[])
{
	if (!isFourOfAKind(hand))
	{
		if (hand[2]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank)
		{
			if (hand[2]->getComponent<CardComponent>().rank == hand[0]->getComponent<CardComponent>().rank)
			{
				return true;
			}
		}
		if (hand[2]->getComponent<CardComponent>().rank == hand[3]->getComponent<CardComponent>().rank)
		{
			if (hand[2]->getComponent<CardComponent>().rank == hand[4]->getComponent<CardComponent>().rank)
			{
				return true;
			}
		}
		if (hand[2]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank)
		{
			if (hand[2]->getComponent<CardComponent>().rank == hand[3]->getComponent<CardComponent>().rank)
			{
				return true;
			}
		}
	}
	return false;
}

bool Poker::isTwoPair(std::shared_ptr<Entity> hand[])
{
	if (!isThreeOfAKind(hand))
	{
		if (hand[0]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank &&
			hand[2]->getComponent<CardComponent>().rank == hand[3]->getComponent<CardComponent>().rank ||
			hand[1]->getComponent<CardComponent>().rank == hand[2]->getComponent<CardComponent>().rank &&
			hand[3]->getComponent<CardComponent>().rank == hand[4]->getComponent<CardComponent>().rank || 
			hand[0]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank &&
			hand[3]->getComponent<CardComponent>().rank == hand[4]->getComponent<CardComponent>().rank)
		{
			return true;
		}
	}
	return false;
}

bool Poker::isOnePair(std::shared_ptr<Entity> hand[])
{
	if (!isTwoPair(hand) && !isThreeOfAKind(hand))
	{
		if (hand[0]->getComponent<CardComponent>().rank == hand[1]->getComponent<CardComponent>().rank ||
			hand[1]->getComponent<CardComponent>().rank == hand[2]->getComponent<CardComponent>().rank ||
			hand[2]->getComponent<CardComponent>().rank == hand[3]->getComponent<CardComponent>().rank ||
			hand[3]->getComponent<CardComponent>().rank == hand[4]->getComponent<CardComponent>().rank)
		{
			return true;
		}
	}
	return false;
}

bool Poker::isHighCard(std::shared_ptr<Entity> hand[])
{
	if (!isFiveOfAKind(hand) &&
		!isRoyalFlush(hand) &&
		!isFourOfAKind(hand) &&
		!isStraightFlush(hand) &&
		!isStraight(hand) &&
		!isFullHouse(hand) &&
		!isFlush(hand) &&
		!isThreeOfAKind(hand) &&
		!isTwoPair(hand) && 
		!isOnePair(hand))
	{
		return true;
	}
	return false;
}