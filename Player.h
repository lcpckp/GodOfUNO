#pragma once
#include<iostream>
#include<vector>
#include "UnoCard.h"

class Player
{
private:
	int score;
	std::string name;
	std::vector<UnoCard> hand;
public:
	Player();
	Player(std::string n);
	void receiveCard(UnoCard& c);
	std::vector<UnoCard>* getHand();
	void addScore(int s);
	int getScore();
	std::string getName();
};

