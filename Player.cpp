#include "Player.h"

Player::Player()
{
	score = 0; name = "Bot";
}

Player::Player(std::string n)
{
	score = 0; name = n;
}

void Player::receiveCard(UnoCard c)
{
	hand.push_back(c);
}

std::vector<UnoCard>& Player::getHand()
{
	return hand;
}

void Player::addScore(int s)
{
	score += s;
}

std::string Player::getName()
{
	return name;
}

int Player::getScore()
{
	return score;
}

void Player::clearHand()
{
	hand.clear();
}