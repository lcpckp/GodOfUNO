#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>

class UnoCard : public sf::Drawable
{
private:

	//Define Card
	int drawCards; //+2 or +4
	bool skip; //true for skips and drawCards
	bool reverse; //treat the same as skip for now
	int cardValue; //0-9 face cards, 20 for utility cards, 50 for wilds
	sf::Color cardColor; //suit of the card
	bool faceDown;

	//Draw function
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	//Store drawable information in card
	sf::RectangleShape baseCard;
	sf::RectangleShape baseCardBack;
	sf::Text cardIcon;
	sf::Text cardIconBack;
	sf::Vector2f targetPosition;
	float moveSpeed;

public:
	UnoCard();
	UnoCard(sf::Color c, int v, bool s, bool r, int dc, sf::Font& f);

	sf::FloatRect getBounds();
	void setFaceDown(bool v);
	bool isFaceDown();

	//Set properties of card
	void setColor(sf::Color c);
	void setValue(int v);
	void setSkip(bool s);
	void setReverse(bool r);
	void setDrawCards(int c);
	void setCardIdentity(sf::Color c, int v, bool s, bool r, int dc);

	//Get properties of card
	sf::Color getColor();
	int getValue();
	bool getSkip();
	bool getReverse();
	int getDrawCards();
	std::string getCardIcon();

	//Moves towards target baseCard position
	void updatePosition();

	//Sets final position of card
	void setTargetPosition(int x, int y);
	sf::Vector2f getTargetPosition();

	//Logging purposes
	std::string getColorString();
	std::string getValueString();
	std::string getUtilityString();
	std::string getCardName();
};

