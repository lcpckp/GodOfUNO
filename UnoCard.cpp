#include "UnoCard.h"
#include "GameEngine.h"
#include<iostream>
#include<cmath>
#include<SFML/Graphics.hpp>



UnoCard::UnoCard()
{
    cardColor = sf::Color::Black;
    cardValue = 50;
    skip = true;
    reverse = false;
    drawCards = 4;

    faceDown = false;
}

UnoCard::UnoCard(sf::Color c, int v, bool s, bool r, int dc, sf::Font& f)
{
    cardColor = c;
    cardValue = v;
    skip = s;
    reverse = r;
    drawCards = dc;

    faceDown = (cardValue >= 0);
    
    //Front of Card
    baseCard.setSize(sf::Vector2f(GameEngine::CARD_WIDTH, GameEngine::CARD_HEIGHT));
    baseCard.setPosition(GameEngine::WINDOW_WIDTH / 2, GameEngine::WINDOW_HEIGHT / 2);
    baseCard.setOutlineColor(sf::Color::White);
    baseCard.setFillColor(cardColor);
    baseCard.setOutlineThickness(1);

    /*Rarity based on how many cards you draw
    switch(dc)
    {
    case 0:
    case 1:
        baseCardBack.setOutlineColor(sf::Color::White);
        break;
    case 2:
        baseCardBack.setOutlineColor(sf::Color::Green);
        break;
    case 3:
        baseCardBack.setOutlineColor(sf::Color::Blue);
        break;
    case 4:
        baseCardBack.setOutlineColor(sf::Color::Magenta);
    default:
        baseCardBack.setOutlineColor(sf::Color::Yellow);
    } */

    if (cardColor == sf::Color::Yellow || cardColor == sf::Color::Green)
    {
        cardIcon.setFillColor(sf::Color::Black);
    }
    else
    {
        cardIcon.setFillColor(sf::Color::White);
    }

    cardIcon.setFont(f);
    cardIcon.setCharacterSize(30);
    cardIcon.setString(getCardIcon());

    sf::FloatRect localTextBounds = cardIcon.getLocalBounds(); //gets bounds of text itself
    cardIcon.setOrigin(localTextBounds.left + (localTextBounds.width / 2.f), localTextBounds.top + (localTextBounds.height / 2.f)); //set origin to center of text

    //Back of Card
    baseCardBack.setSize(sf::Vector2f(GameEngine::CARD_WIDTH, GameEngine::CARD_HEIGHT));
    baseCardBack.setPosition(GameEngine::WINDOW_WIDTH / 2, GameEngine::WINDOW_HEIGHT / 2);
    baseCardBack.setOutlineColor(sf::Color::White);
    baseCardBack.setFillColor(sf::Color::Black);
    baseCardBack.setOutlineThickness(1);

    cardIconBack.setFont(f);
    cardIconBack.setCharacterSize(50);
    cardIconBack.setFillColor(sf::Color::White);
    cardIconBack.setString("U");

    localTextBounds = cardIconBack.getLocalBounds();
    cardIconBack.setOrigin(localTextBounds.left + (localTextBounds.width / 2.f), localTextBounds.top + localTextBounds.height / 2.f); //set origin to center of text
    
}

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

sf::Color UnoCard::getColor()
{
    return cardColor;
}

void UnoCard::setColor(sf::Color c)
{
    cardColor = c;
    baseCard.setFillColor(c);
    if (cardColor == sf::Color::Yellow || cardColor == sf::Color::Green)
    {
        cardIcon.setFillColor(sf::Color::Black);
    }
    else
    {
        cardIcon.setFillColor(sf::Color::White);
    }
}

void UnoCard::setValue(int v)
{
    cardValue = v;
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

std::string UnoCard::getColorString()
{

    //{ sf::Color(180, 0, 0), sf::Color(220, 170, 0) , sf::Color(0, 180, 0), sf::Color(0, 0, 180) };

    if (cardColor == sf::Color(180, 0, 0))
    {
        return "Red";
    }
    else if (cardColor == sf::Color(220, 170, 0))
    {
        return "Yellow";
    }
    else if (cardColor == sf::Color(0, 180, 0))
    {
        return "Green";
    }
    else if (cardColor == sf::Color(0, 0, 180))
    {
        return "Blue";
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

std::string UnoCard::getCardIcon()
{
    std::string i = "";
    if (skip && !drawCards)
        i += "( \\ )";
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

sf::FloatRect UnoCard::getBounds()
{
    return baseCard.getGlobalBounds();
}

void UnoCard::setFaceDown(bool v)
{
    faceDown = v;
}

bool UnoCard::isFaceDown()
{
    return faceDown;
}

void UnoCard::updatePosition()
{
    //sets position to approach the target position

    sf::Vector2f distanceVector = getTargetPosition() - baseCard.getPosition();
    float length = std::sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
    if (length < 2)
    {
        baseCard.setPosition(getTargetPosition());
        baseCardBack.setPosition(getTargetPosition());
    }
    else
    {
        baseCard.setPosition(baseCard.getPosition() + distanceVector * 0.005f);
        baseCardBack.setPosition(baseCardBack.getPosition() + distanceVector * 0.005f);
    }

    cardIcon.setPosition(baseCard.getPosition() + baseCard.getSize() / 2.f);
    //sf::Vector2f currPos = baseCard.getGlobalBounds()
    


    //baseCard.setPosition(getTargetPosition());
    cardIcon.setPosition(baseCard.getPosition() + baseCard.getSize() / 2.f);

    //baseCardBack.setPosition(getTargetPosition());
    cardIconBack.setPosition(baseCardBack.getPosition() + baseCardBack.getSize() / 2.f);
}

sf::Vector2f UnoCard::getTargetPosition()
{
    return targetPosition;
}

void UnoCard::setTargetPosition(int x, int y)
{
    targetPosition = sf::Vector2f(x, y);
    updatePosition();
}

void UnoCard::setCardIdentity(sf::Color c, int v, bool s, bool r, int dc)
{
    cardColor = c; cardValue = v; skip = s; reverse = r; drawCards = dc;

}