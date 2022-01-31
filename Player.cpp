#include "Player.h"

Player::Player()
{
	score = 0; name = "Bot";
}

Player::Player(std::string n)
{
	score = 0; name = n; hand.clear();
}

void Player::receiveCard(std::unique_ptr<UnoCard> card)
{
	hand.push_back(std::move(card));
}

std::unique_ptr<UnoCard> Player::giveCard(int c)
{
	std::unique_ptr<UnoCard> card = std::move(hand.at(c));
	hand.erase(hand.begin() + c);
	return std::move(card);
}

std::vector<std::unique_ptr<UnoCard>>& Player::getHand()
{
	return hand;
}

int Player::getHandValue()
{
	int handValue = 0;

	//Loop through losing player's hand, get total score
	for (int i = 0; i < hand.size(); i++)
	{
		handValue += hand.at(i)->getValue();
	}

	return handValue;
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

void Player::fanOutHand(float x, float y)
{
	//offset for hands
	float offset = Resources::CARD_WIDTH + Resources::CARD_SPACING;
	x = x - (offset * hand.size() / 2);
	//Fan out cards in hand
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setTargetPosition(x, y);
		x += offset;
	}
}