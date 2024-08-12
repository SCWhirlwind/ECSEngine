#pragma once
#include "GameManager.h"
#include "ComponentHeaders.h"

class Poker
{
public:

	std::shared_ptr<Entity>* sortHand(std::shared_ptr<Entity> hand[]);

	int checkHand(std::shared_ptr<Entity> hand[]);
	int checkHighCard(std::shared_ptr<Entity> hand[], int handtype);

private:
	
	bool isFiveOfAKind(std::shared_ptr<Entity> hand[]);
	bool isRoyalFlush(std::shared_ptr<Entity> hand[]);
	bool isFourOfAKind(std::shared_ptr<Entity> hand[]);
	bool isStraightFlush(std::shared_ptr<Entity> hand[]);
	bool isStraight(std::shared_ptr<Entity> hand[]);
	bool isFullHouse(std::shared_ptr<Entity> hand[]);
	bool isFlush(std::shared_ptr<Entity> hand[]);
	bool isThreeOfAKind(std::shared_ptr<Entity> hand[]);
	bool isTwoPair(std::shared_ptr<Entity> hand[]);
	bool isOnePair(std::shared_ptr<Entity> hand[]);
	bool isHighCard(std::shared_ptr<Entity> hand[]);
};

