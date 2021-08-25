#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>

namespace unoutils
{
	enum COLOR { Red, Yellow, Blue, Green, Wild };
	enum UTILITY { None, Skip, Reverse, Draw2, Draw4 };
}


class UnoCard : public sf::Drawable
{
private:
	enum unoutils::COLOR cardColor;

	enum unoutils::UTILITY cardUtility;

	static const int CARD_WIDTH = 50;
	static const int CARD_HEIGHT = 80;
	int value;
	sf::RectangleShape baseCard;
	sf::Text cardName;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool visible;
	sf::Vector2i position;

public:
	UnoCard();
	UnoCard(int c, int u, int v, sf::Font* f);
	int getColor();
	void setColor(int c);
	int getValue();
	int getUtility();
	sf::RectangleShape getBaseCard();
	std::string getReadableColor();
	std::string getReadableValue();
	std::string getReadableUtility();
	std::string getReadableCard();
	std::string getCardIcon();
	sf::Color getSFColor();
	void setBounds(sf::FloatRect b);
	sf::FloatRect getBounds();
	void setVisibility(bool v);
	bool isVisible();
	void updatePosition(int x, int y);

	sf::Vector2i getPos();
};

