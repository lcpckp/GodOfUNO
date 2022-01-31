#include "UnoCard.h"
#include "GameEngine.h"
#include<iostream>
#include<cmath>
#include<SFML/Graphics.hpp>


//Constructors

UnoCard::UnoCard()
{
    //not used
    cardColor = sf::Color::Black;
    cardValue = 50;

    skip = true;
    reverse = false;
    drawCards = 4;

    faceDown = false;

    moveSpeed = 20.f;
    setTargetPosition(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 2);
}

UnoCard::UnoCard(sf::Color c, int v, bool s, bool r, int dc, int diff, sf::Font& f)
{
    cardColor = c;
    cardValue = v;
    skip = s;
    reverse = r;
    drawCards = dc;
    faceDown = (cardValue >= 0);

    moveSpeed = 4.f;

    setTargetPosition(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 2);

    //Front of Card
    baseCard.setSize(sf::Vector2f(Resources::CARD_WIDTH, Resources::CARD_HEIGHT));
    baseCard.setPosition(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 2);
    baseCard.setOutlineColor(sf::Color::White);
    baseCard.setFillColor(cardColor);
    baseCard.setOutlineThickness(2);
    
    cardIcon.setFont(f);
    cardIcon.setCharacterSize(Resources::FONT_SIZE_NORMAL);
    cardIcon.setString(getCardIcon());

    //set origin to center of text - DO NOT DELETE (stylistic change)
    //sf::FloatRect localTextBounds = cardIcon.getLocalBounds(); //gets bounds of text itself
    //cardIcon.setOrigin(localTextBounds.left + (localTextBounds.width / 2.f), localTextBounds.top + (localTextBounds.height / 2.f)); 

    //Back of Card
    baseCardBack.setSize(sf::Vector2f(Resources::CARD_WIDTH, Resources::CARD_HEIGHT));
    baseCardBack.setPosition(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 2);
    baseCardBack.setOutlineColor(sf::Color::White);
    baseCardBack.setFillColor(sf::Color::Black);
    baseCardBack.setOutlineThickness(2);

    //Rarity based on how many cards you draw (difficulty 5+ only)
    if (diff >= 5)
    {
        switch (dc)
        {
        case 0:
        case 1:
            baseCardBack.setOutlineColor(sf::Color::White);
            break;
        case 2:
            baseCardBack.setOutlineColor(Resources::availableCardColors[2]);
            break;
        case 3:
            baseCardBack.setOutlineColor(Resources::availableCardColors[3]);
            break;
        case 4:
            baseCardBack.setOutlineColor(Resources::availableCardColors[4]);
            break;
        default:
            baseCardBack.setOutlineColor(Resources::availableCardColors[5]);
        }
    }

    cardIconBack.setFont(f);
    cardIconBack.setCharacterSize(12);
    cardIconBack.setFillColor(sf::Color::White);
    cardIconBack.setString("");

    //set origin to center of text - DO NOT DELETE (stylistic change)
    sf::FloatRect localTextBounds = cardIconBack.getLocalBounds();
    cardIconBack.setOrigin(localTextBounds.left + (localTextBounds.width / 2.f), localTextBounds.top + localTextBounds.height / 2.f); 
    
}

//Implemented Draw method

void UnoCard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (faceDown)
    {
        target.draw(baseCardBack);
        target.draw(cardIconBack);
    }
    else
    {
        target.draw(baseCard);
        target.draw(cardIcon);
    }
}

//Setters

void UnoCard::setColor(sf::Color c)
{
    cardColor = c;
    baseCard.setFillColor(c);
}

void UnoCard::setValue(int v)
{
    cardValue = v;
    cardIcon.setString(getCardIcon());
}

void UnoCard::setFaceDown(bool v)
{
    faceDown = v;
}

void UnoCard::setCardIdentity(sf::Color c, int v, bool s, bool r, int dc)
{
    cardColor = c; cardValue = v; skip = s; reverse = r; drawCards = dc;

}

//Accessors

sf::Color UnoCard::getColor()
{
    return cardColor;
}

int UnoCard::getValue()
{
    return cardValue;
}

bool UnoCard::getSkip()
{
    return skip;
}

bool UnoCard::getReverse()
{
    return reverse;
}

int UnoCard::getDrawCards()
{
    return drawCards;
}

std::string UnoCard::getCardIcon()
{
    std::string i = "";
    if (skip && !drawCards)
        i += "( / )";
    else if (reverse)
        i += "</>";
    else if (drawCards)
        i += "+" + std::to_string(drawCards);
    else if (cardValue <= 9 && cardValue >= 0)
        i += std::to_string(cardValue);
    else if (cardValue == 50)
        i += "W";
    return i;
}

bool UnoCard::isFaceDown()
{
    return faceDown;
}

//ToString

std::string UnoCard::getColorString()
{

    //{ sf::Color(180, 0, 0), sf::Color(220, 170, 0) , sf::Color(0, 180, 0), sf::Color(0, 0, 180) };

    if (cardColor == Resources::availableCardColors[0])
    {
        return "Red";
    }
    else if (cardColor == Resources::availableCardColors[1])
    {
        return "Yellow";
    }
    else if (cardColor == Resources::availableCardColors[2])
    {
        return "Green";
    }
    else if (cardColor == Resources::availableCardColors[3])
    {
        return "Blue";
    }
    else if (cardColor == Resources::availableCardColors[4])
    {
        return "Purple";
    }
    else if (cardColor == Resources::availableCardColors[5])
    {
        return "Orange";
    }
    else if (cardColor == sf::Color::Black)
    {
        return "Wild";
    }
    else
    {
        return "Mystery";
    }
}

std::string UnoCard::getValueString()
{
    return std::to_string(cardValue);
}

std::string UnoCard::getUtilityString()
{
    std::string s = "";
    if (skip && !drawCards)
        s += "Skip";
    else if (reverse)
        s += "Reverse";
    else
    {
        s += "Draw +" + std::to_string(drawCards);
    }
    return s;
}

std::string UnoCard::getCardName()
{
    std::string s = getColorString();
    
    if (skip || reverse)
    {
        s += " " + getUtilityString();
    }
    else
    {
        if(cardValue <= 9)
            s += " " + getValueString();
    }

    return s;
}

//Position Info

sf::FloatRect UnoCard::getBounds()
{
    return baseCard.getGlobalBounds();
}

sf::RectangleShape UnoCard::getBaseCard()
{
    return baseCard;
}


void UnoCard::updatePosition(sf::Time dt)
{
    //sets position to approach the target position

    sf::Vector2f distanceVector = getTargetPosition() - baseCard.getPosition();
    float length = std::sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
    if (length < 1)
    {
        baseCard.setPosition(getTargetPosition());
        baseCardBack.setPosition(getTargetPosition());
    }
    else
    {
        baseCard.setPosition(baseCard.getPosition() + ((distanceVector * moveSpeed) * (float)dt.asSeconds()));
        baseCardBack.setPosition(baseCardBack.getPosition() + ((distanceVector * moveSpeed) * (float)dt.asSeconds()));
    }

    //NEW WAY with ORIGIN at TOP LEFT of TEXT
    cardIcon.setPosition(baseCard.getPosition().x + 8, baseCard.getPosition().y);
    //cardIconBack.setPosition(baseCardBack.getPosition().x + 5, baseCardBack.getPosition().y);

    //OLD WAY with ORIGIN at CENTER of TEXT
    //cardIcon.setPosition(baseCard.getPosition() + baseCard.getSize() / 2.f);
    cardIconBack.setPosition(baseCardBack.getPosition() + baseCardBack.getSize() / 2.f);
}

sf::Vector2f UnoCard::getTargetPosition()
{
    return targetPosition;
}

void UnoCard::setTargetPosition(int x, int y)
{
    targetPosition = sf::Vector2f(x, y);
}

