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

	void addScore(int s);
	void setScore(int s);

	void receiveCard(UnoCard c);
	void clearHand();

	//Accessors
	int getScore();
	std::string getName();

	std::vector<UnoCard>& getHand();


};

