#pragma once
#include<memory>
#include "UnoCard.h"
#include "Resources.h"

class Player
{
private:

	int score;
	std::string name;
	std::vector<std::unique_ptr<UnoCard>> hand;

public:
	Player();
	Player(std::string n);
	~Player() = default;
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;


	void addScore(int s);
	void setScore(int s); //unused

	void receiveCard(std::unique_ptr<UnoCard> c);
	std::unique_ptr<UnoCard> giveCard(int c);
	void clearHand();

	//Accessors
	int getScore();
	std::string getName();

	std::vector<std::unique_ptr<UnoCard>>& getHand();
	int getHandValue();
	void fanOutHand(float x, float y);
};

