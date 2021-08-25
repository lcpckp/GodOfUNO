#include "UnoGame.h"
#include<chrono>
#include<thread>
#include<iostream>
#include<vector>

const int COMPUTER_DELAY_MS = 250;

UnoGame::UnoGame()
{
	//Random Seed
	srand(time(NULL));

	font.loadFromFile("arialbd.ttf");

	createGameGlobals();
	setupBeginGame();
}


void UnoGame::setupBeginGame()
{
	draw = setupDeck(true); //true indicates deck will be shuffled
	discard.clear();

	playerTurn = false;
	colorPick = false;

	players.at(0).getHand()->clear();
	players.at(1).getHand()->clear();

	//Initial Deal (7 cards)
	UnoCard* currCard;
	for (int d = 1; d <= 7; d++)
	{
		currCard = &draw.back();
		players.at(0).receiveCard(*currCard);
		draw.pop_back();

		currCard = &draw.back();
		players.at(1).receiveCard(*currCard);
		draw.pop_back();
	}

	//Initial card flip draw 4 protection
	if (draw.back().getUtility() == unoutils::Draw4)
	{
		UnoCard temp = draw.front();
		draw.front() = draw.back();
		draw.pop_back();
	}

	playCard(draw.back());

	//if first card is Wild, player needs to select color.
	if (draw.back().getColor() == unoutils::Wild)
	{
		playerTurn = true;
	}

	draw.pop_back();
}

//High Level Accessors

//First card that can be played will be
void UnoGame::runBasicComputerMove()
{
	std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(COMPUTER_DELAY_MS)); //be nice to humans, sort of

	//Get the computer hand
	std::vector<UnoCard>* compHand = players.at(1).getHand();
	//Set up pointer to current card
	UnoCard* compCard;

	//switch indicates if a card is played this turn
	bool cardPlayed = false;

	//if no color pick is required
	if (!colorPickRequired())
	{
		//loop through the computer hand
		for (int c = 0; c < compHand->size(); c++)
		{
			//set the card that we want to look at
			compCard = &compHand->at(c);

			//if it's a matching color, play it, remove from hand, and break;
			if (compCard->getColor() == discard.back().getColor())
			{
				playCard(*compCard);
				compHand->erase(compHand->begin() + c);
				cardPlayed = true;
				c = 100; // testing
				break;
			}
			//if it's the same number card, do the same
			else if (compCard->getValue() == discard.back().getValue() && compCard->getValue() <= 9)
			{
				playCard(*compCard);
				compHand->erase(compHand->begin() + c);
				cardPlayed = true;
				c = 100; // testing
				break;
			}
			//if the utility matches, same thing as above
			else if ((compCard->getValue() == discard.back().getValue() && compCard->getValue() == 20) && compCard->getUtility() == getDiscardUtility())
			{
				playCard(*compCard);
				compHand->erase(compHand->begin() + c);
				cardPlayed = true;
				c = 100; // testing
				break;
			}
		}

		//if a color, number, or utility hasn't been played yet,
		if (!cardPlayed)
		{
			//cycle through our hand again
			for (int c = 0; c < compHand->size(); c++)
			{
				compCard = &compHand->at(c);

				//looking for wild cards to play.
				if (compCard->getColor() == unoutils::Wild)
				{
					playCard(*compCard);
					compHand->erase(compHand->begin() + c);
					cardPlayed = true;
					c = 100; //testing
					break;
				}
			}
		}

		//if we still haven't played anything, we must draw.
		if (!cardPlayed)
			drawCard(1);
	}
	else //colorPick IS Required
	{
		bool colorSelected = false;
		for (int c = 0; c < compHand->size(); c++)
		{
			compCard = &compHand->at(c);
			if (compCard->getColor() != unoutils::Wild)
			{
				colorSelected = true;
				discard.back().setColor(compCard->getColor());
				noPickRequired();
				if (getDiscardUtility() != unoutils::Draw4)
				{
					playerTurn = !playerTurn;
				}
				break;
			}
		}
		if (!colorSelected)
		{
			discard.back().setColor(std::rand() % 4);
		}
	}
}

bool UnoGame::isPlayerTurn()
{
	return playerTurn;
}

int UnoGame::getDiscardColor()
{
	return discard.back().getColor();
}

int UnoGame::getDiscardUtility()
{
	return discard.back().getUtility();
}

bool UnoGame::colorPickRequired()
{
	return discard.back().getColor() == unoutils::Wild;
}

void UnoGame::noPickRequired()
{
	colorPick = false;
}

void UnoGame::drawCard(int playerNum)
{
	if (!colorPickRequired())
	{
		UnoCard* currCard = &draw.back();
		if (currCard->getColor() == unoutils::Wild)
		{
			playCard(*currCard);
			draw.pop_back();
		}
		else if (currCard->getColor() == discard.back().getColor())
		{
			playCard(*currCard);
			draw.pop_back();
		}
		else if (currCard->getValue() <= 9 && (currCard->getValue() == discard.back().getValue()))
		{
			playCard(*currCard);
			draw.pop_back();
		}
		else if (currCard->getUtility() == discard.back().getUtility() && discard.back().getUtility() != unoutils::None)
		{
			playCard(*currCard);
			draw.pop_back();
		}
		else
		{
			if (playerNum == 0)
			{
				currCard->setVisibility(true);
			}
			players.at(playerNum).getHand()->push_back(*currCard);
			draw.pop_back();
			playerTurn = !playerTurn;
		}
	}

	else //colorPick IS Required
	{
		std::cout << "weird part of code" << std::endl;
	}
	

	
}

std::vector<UnoCard>* UnoGame::getHandofPlayer(int p)
{
	return players.at(p).getHand();
}

std::vector<UnoCard>* UnoGame::getButtons()
{
	return &buttons;
}



std::vector<UnoCard> UnoGame::setupDeck(bool shuffle)
{
	std::vector<UnoCard> deck;

	//Cycle Colors (including 'wild' color)
	for (int colorNum = unoutils::Red; colorNum <= unoutils::Wild; colorNum++)
	{
		//non-wilds (first 4 loops)
		if (colorNum < 4)
		{
			//0-9s
			for (int valueNum = 0; valueNum <= 9; valueNum++)
			{
				//push the card
				deck.push_back(*new UnoCard(static_cast<unoutils::COLOR>(colorNum), static_cast<unoutils::UTILITY>(0), valueNum, &font));
				//push a second card if non-zero
				if (valueNum != 0)
					deck.push_back(*new UnoCard(static_cast<unoutils::COLOR>(colorNum), static_cast<unoutils::UTILITY>(0), valueNum, &font));

			}
			//Twice for each utility
			for (int u = 0; u <= 1; u++)
			{
				deck.push_back(*new UnoCard(colorNum, unoutils::Skip, 20, &font));
				deck.push_back(*new UnoCard(colorNum, unoutils::Reverse, 20, &font));
				deck.push_back(*new UnoCard(colorNum, unoutils::Draw2, 20, &font));
			}
		}
		//Otherwise, has to be Wild
		else
		{
			//Loop 4 times, add a Wild and Draw4 each time
			for (int w = 0; w <= 3; w++)
			{
				deck.push_back(*new UnoCard(colorNum, unoutils::Draw4, 50, &font));
				deck.push_back(*new UnoCard(colorNum, unoutils::None, 50, &font));
			}
		}
	}

	if (shuffle)
	{
		return shuffleDeck(deck);
	}
	return deck;
}

std::vector<UnoCard> UnoGame::shuffleDeck(std::vector<UnoCard> d)
{
	int j;
	for (int i = 0; i < d.size(); i++)
	{
		UnoCard tempCard = d.at(i);
		j = std::rand() % d.size();
		d.at(i) = d.at(j);
		d.at(j) = tempCard;
	}
	return d;
}

void UnoGame::reshuffle()
{
	if (over())
	{
		setupBeginGame();
	}
	else //Game is NOT over
	{
		//Keep one card for the game to continue
		UnoCard* keepPlaying = &discard.back();
		discard.pop_back();
		//std::cout << "keep playing is: " << keepPlaying->getReadableCard() << std::endl;

		//set up a fresh deck and insert the discard and any remaining draw cards
		std::vector<UnoCard> newDeck;
		newDeck.reserve(draw.size() + discard.size());
		newDeck.insert(newDeck.end(), draw.begin(), draw.end());
		newDeck.insert(newDeck.end(), discard.begin(), discard.end());

		//Reset wilds that were played onto discard pile
		for (int c = 0; c < newDeck.size(); c++)
		{
			//Wilds always have a value of 50
			if (newDeck.at(c).getValue() == 50)
			{
				newDeck.at(c).setColor(unoutils::Wild);
			}
			newDeck.at(c).setVisibility(false);
		}

		//shuffle the new deck and reset draw pile to that new deck
		draw = shuffleDeck(newDeck);

		//clear the discard pile, put the one card back there to continue the game
		discard.clear();
		discard.push_back(*keepPlaying);
	}
}

void UnoGame::createGameGlobals()
{
	//TODO add more players
	players.push_back(*new Player("Alec"));
	players.push_back(*new Player("Jordan (BOT)"));

	//Set up buttons
	buttons.push_back(*new UnoCard(unoutils::Red, unoutils::None, -1, &font));
	buttons.push_back(*new UnoCard(unoutils::Yellow, unoutils::None, -1, &font));
	buttons.push_back(*new UnoCard(unoutils::Blue, unoutils::None, -1, &font));
	buttons.push_back(*new UnoCard(unoutils::Green, unoutils::None, -1, &font));
}

std::vector<Player>* UnoGame::getPlayers()
{
	return &players;
}

std::vector<UnoCard>* UnoGame::getDrawPile()
{
	return &draw;
}

std::vector<UnoCard>* UnoGame::getDiscardPile()
{
	return &discard;
}

void UnoGame::playCard(UnoCard& c)
{
	//std::cout << "Card played: " << c.getReadableColor() << " " << c.getReadableUtility() << " " << c.getReadableValue() << " by player " << (int)(!playerTurn) + 1 << std::endl;
	c.setVisibility(true);
	discard.push_back(c);

	if (c.getColor() == unoutils::Wild && c.getUtility() == unoutils::None)
	{
		colorPick = true;
	}
	else if (c.getUtility() != unoutils::None)
	{
		if (c.getUtility() == unoutils::Draw2)
		{
			for (int d = 0; d < 2; d++)
			{
				players.at(playerTurn).receiveCard(draw.back());
				draw.pop_back();
			}
		}
		if (c.getUtility() == unoutils::Draw4)
		{
			colorPick = true;
			for (int d = 0; d < 4; d++)
			{
				players.at(playerTurn).receiveCard(draw.back());
				draw.pop_back();
			}
		}
		
	}
	else
	{
		playerTurn = !playerTurn;
	}
}

//Click Handling

bool UnoGame::drawClicked(int x, int y)
{
	if (draw.back().getBounds().contains(x, y))
	{
		drawCard(0);
		return true;
	}
	return false;
}

bool UnoGame::handCardClicked(int x, int y)
{
	if (!colorPickRequired())
	{
		std::vector<UnoCard>* currHand = players.at(0).getHand();
		//For each card in human's hand:
		for (int c = 0; c < currHand->size(); c++)
		{
			UnoCard* currCard = &currHand->at(c);
			if (currCard->getBounds().contains(x, y))
			{
				if (currCard->getColor() == unoutils::Wild)
				{
					playCard(*currCard);
					currHand->erase(currHand->begin() + c);
					return true;
				}
				else if (currCard->getColor() == discard.back().getColor())
				{
					playCard(*currCard);
					currHand->erase(currHand->begin() + c);
					return true;
				}
				else if (currCard->getValue() <= 9 && currHand->at(c).getValue() == discard.back().getValue())
				{
					playCard(*currCard);
					currHand->erase(currHand->begin() + c);
					return true;
				}
				else if (currCard->getUtility() == discard.back().getUtility() && discard.back().getUtility() != unoutils::None)
				{
					playCard(*currCard);
					currHand->erase(currHand->begin() + c);
					return true;
				}
			}
		}
	}
	return false;
}

bool UnoGame::buttonsClicked(int x, int y, bool incrementTurn)
{
	for (int b = 0; b < 4; b++)
	{
		if (buttons.at(b).getBounds().contains(x, y))
		{
			if (incrementTurn)
				playerTurn = !playerTurn;
			colorPick = false;
			discard.back().setColor(buttons.at(b).getColor());
			return true;
		}
	}
	return false;
}

int UnoGame::over()
{
	if (players.at(0).getHand()->empty())
	{
		return 1;
	}
	else if (players.at(1).getHand()->empty())
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

void UnoGame::tallyScore()
{
	//over() returns 1 or 2 for player's 0 or 1 win state, so subtract 1 to get the index of the winner
	int winnerIndex = over() - 1;
	std::vector<UnoCard>* losingHand = players.at(!winnerIndex).getHand();
	int score = 0;

	//Loop through losing player's hand, get total score
	for (int i = 0; i < losingHand->size(); i++)
	{
		score += losingHand->at(i).getValue();
	}

	players.at(winnerIndex).addScore(score);
	latestScore = score;
}

int UnoGame::getLatestScore()
{
	return latestScore;
}