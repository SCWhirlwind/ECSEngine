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

int Poker::checkHand(std::shared_ptr<Entity> hand[])
{
	if (isFiveOfAKind(hand))
	{
		std::cout << "Five of a kind" << std::endl;
		return 10;
	}
	else if (isRoyalFlush(hand))
	{
		std::cout << "Royal flush" << std::endl;
		return 9;
	}
	else if (isFourOfAKind(hand))
	{
		std::cout << "Four of a kind" << std::endl;
		return 8;
	}
	else if (isStraightFlush(hand))
	{
		std::cout << "Straight flush" << std::endl;
		return 7;
	}
	else if (isStraight(hand))
	{
		std::cout << "Straight" << std::endl;
		return 6;
	}
	else if (isFullHouse(hand))
	{
		std::cout << "Fullhouse" << std::endl;
		return 5;
	}
	else if (isFlush(hand))
	{
		std::cout << "Flush" << std::endl;
		return 4;
	}
	else if (isThreeOfAKind(hand))
	{
		std::cout << "Three of a kind" << std::endl;
		return 3;
	}
	else if (isTwoPair(hand))
	{
		std::cout << "Two Pair" << std::endl;
		return 2;
	}
	else if (isOnePair(hand))
	{
		std::cout << "One Pair" << std::endl;
		return 1;
	}
	else if (isHighCard(hand))
	{
		std::cout << "High Card" << std::endl;
		return 0;
	}
	std::cout << "Error" << std::endl;
	return -1;
}

int Poker::checkHighCard(std::shared_ptr<Entity> hand[], int handtype)
{
	switch (handtype)
	{
		case 0:
		{
			return hand[4]->getComponent<CardComponent>().rank;
		}
		case 1:
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
		case 2:
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
		case 3:
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
		case 4:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		case 5:
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
		case 6:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		case 7:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		case 8:
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
		case 9:
		{
			return hand[4]->getComponent<CardComponent>().rank;
			break;
		}
		case 10:
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

