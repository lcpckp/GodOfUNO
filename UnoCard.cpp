#include "UnoCard.h"

#include<SFML/Graphics.hpp>
#include<iostream>

UnoCard::UnoCard()
{
    sf::Color::Black;
    cardColor = unoutils::Wild;
    cardUtility = unoutils::Draw4;
    value = 50;
    visible = false; setVisibility(false); //process visual look for visibility
}

UnoCard::UnoCard(int c, int u, int v, sf::Font* f)
{
    cardColor = static_cast<unoutils::COLOR>(c);
    cardUtility = static_cast<unoutils::UTILITY>(u);
    value = v;
    if (value >= 0)
    {
        setVisibility(false);
    }
    else
    {
        setVisibility(true);
    }
        

    baseCard.setSize(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT));
    baseCard.setPosition(0, 0);
    baseCard.setOutlineColor(sf::Color::White);
    baseCard.setOutlineThickness(1);

    cardName.setFont(*f);

    sf::FloatRect localTextBounds = cardName.getLocalBounds();
    cardName.setOrigin(localTextBounds.left + (localTextBounds.width / 2.f), localTextBounds.top + localTextBounds.height / 2.f); //set origin to center of text
}

void UnoCard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(baseCard);
    target.draw(cardName);
}

int UnoCard::getColor()
{
    return cardColor;
}

void UnoCard::setColor(int c)
{
    cardColor = static_cast<unoutils::COLOR>(c);
    baseCard.setFillColor(this->getSFColor());
}

int UnoCard::getValue()
{
    return value;
}

int UnoCard::getUtility()
{
    return cardUtility;
}

std::string UnoCard::getReadableColor()
{
    switch (cardColor)
    {
    case 0:
        return "red";
    case 1:
        return "yellow";
    case 2:
        return "blue";
    case 3:
        return "green";
    case 4:
        return "wild";
    default:
        return "";
    }
}

std::string UnoCard::getReadableValue()
{
    switch (value)
    {
    case 0:
        return "0";
    case 1:
        return "1";
    case 2:
        return "2";
    case 3:
        return "3";
    case 4:
        return "4";
    case 5:
        return "5";
    case 6:
        return "6";
    case 7:
        return "7";
    case 8:
        return "8";
    case 9:
        return "9";
    case 20:
        return "20";
    case 50:
        return "50";
    default:
        return "";
    }
}

std::string UnoCard::getReadableUtility()
{
    switch (cardUtility)
    {
    case 0:
        return "";
    case 1:
        return "skip";
    case 2:
        return "reverse";
    case 3:
        return "draw two";
    case 4:
        return "draw four";
    default:
        return "";
    }
}

std::string UnoCard::getReadableCard()
{
    return getReadableColor() + " " + getReadableUtility() + "(" + getReadableValue() + ")";
}

std::string UnoCard::getCardIcon()
{
    if (cardUtility != unoutils::None)
    {
        switch (cardUtility)
        {
        case 1:
            return "(/)";
        case 2:
            return "<>";
        case 3:
            return "+2";
        case 4:
            return "+4";
        }
    }
    else if(value == 50)
    {
        return "W";
    }
    else if (value == -1)
    {
        return "";
    }
    else
    {
        return getReadableValue();
    }
    return "";
}

sf::RectangleShape UnoCard::getBaseCard()
{
    return baseCard;
}


sf::FloatRect UnoCard::getBounds()
{
    return baseCard.getGlobalBounds();
}

void UnoCard::setVisibility(bool v)
{
    visible = v;
    if (visible)
    {
        baseCard.setFillColor(this->getSFColor());
        cardName.setCharacterSize(35);
        cardName.setString(this->getCardIcon());
        if (cardColor == unoutils::Yellow || cardColor == unoutils::Green)
        {
            cardName.setFillColor(sf::Color::Black);
        }
        else
        {
            cardName.setFillColor(sf::Color::White);
        }
    }
    else
    {
        cardName.setCharacterSize(50);
        cardName.setString("U");
        baseCard.setFillColor(sf::Color::Black);
        cardName.setFillColor(sf::Color::White);
    }
}

bool UnoCard::isVisible()
{
    return visible;
}

sf::Color UnoCard::getSFColor()
{
    switch (cardColor)
    {
    case 0:
        return sf::Color::Red;
    case 1:
        return sf::Color::Yellow;
    case 2:
        return sf::Color::Blue;
    case 3:
        return sf::Color::Green;
    case 4:
        return sf::Color::Black;
    default:
        return sf::Color::Black;
    }
}

void UnoCard::updatePosition(int x, int y)
{
    baseCard.setPosition(x, y);
    cardName.setPosition(baseCard.getPosition() + baseCard.getSize() / 2.f);
    position = sf::Vector2i(x, y);
}

sf::Vector2i UnoCard::getPos()
{
    return position;
}
