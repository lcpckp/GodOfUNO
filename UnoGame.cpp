#include "UnoGame.h"
#include<chrono>
#include<thread>
#include<iostream>
#include<vector>



UnoGame::UnoGame()
{
	//Random Seed
	srand(time(NULL));

	font.loadFromFile("arialbd.ttf");

	//Runs once
	createGameGlobals();

	//Runs each game
	setupBeginGame();
}


void UnoGame::setupBeginGame()
{
	drawPile.clear();

	for (int c = 0; c < REAL_CARDS.size(); c++)
	{
		drawPile.push_back(REAL_CARDS.at(c));
		std::cout << REAL_CARDS.at(c).getCardName() << std::endl;
	}

	std::cout << "Deck Created" << std::endl;

	drawPile = shuffleDeck(drawPile);

	std::cout << "Deck Shuffled" << std::endl;

	discardPile.clear();

	playerTurn = false;
	colorPick = false;

	players.at(0).clearHand();
	players.at(1).clearHand();

	//Number of cards in hand
	const int NUM_CARDS_IN_STARTING_HAND = 7;
	
	for (int d = 1; d <= NUM_CARDS_IN_STARTING_HAND; d++)
	{
		UnoCard currCard = drawPile.back();
		currCard.setFaceDown(false);
		//player "0" human receives currCard
		players.at(0).receiveCard(currCard);
		drawPile.pop_back();

		currCard = drawPile.back();
		//player "1" computer receives currCard
		players.at(1).receiveCard(currCard);
		drawPile.pop_back();
	}

	//Initial card flip draw >=+3 protection
	while (drawPile.back().getDrawCards() > 2)
	{
		drawPile = shuffleDeck(drawPile);
	}

	playCard(drawPile.back());

	//if first card is Wild, player needs to select color.
	if (drawPile.back().getColor() == sf::Color::Black)
	{
		playerTurn = true;
	}

	drawPile.pop_back();
}

//High Level Accessors

//First card that can be played will be
void UnoGame::runBasicComputerMove()
{
	if (clock.getElapsedTime().asMilliseconds() < COMPUTER_DELAY_MS)
	{
		return;
	}

	//Get the computer hand
	std::vector<UnoCard>& compHand = players.at(1).getHand();
	//Set up reference to current card

	//switch indicates if a card is played this turn
	bool cardPlayed = false;

	//if no color pick is required
	if (!colorPickRequired())
	{
		//loop through the computer hand
		for (int c = 0; c < compHand.size(); c++)
		{
			//set the card that we want to look at
			UnoCard compCard = compHand.at(c);

			if (isLegalCard(compCard))
			{
				playCard(compCard);
				compHand.erase(compHand.begin() + c);
				cardPlayed = true;
				break;
			}
		}

		//if a color, number, or utility hasn't been played yet,
		if (!cardPlayed)
		{
			//cycle through our hand again
			for (int c = 0; c < compHand.size(); c++)
			{
				UnoCard compCard = compHand.at(c);

				//looking for wild cards to play.
				if (compCard.getColor() == sf::Color::Black)
				{
					playCard(compCard);
					compHand.erase(compHand.begin() + c);
					cardPlayed = true;
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
		for (int c = 0; c < compHand.size(); c++)
		{
			UnoCard compCard = compHand.at(c);
			if (compCard.getColor() != sf::Color::Black)
			{
				colorSelected = true;
				discardPile.back().setColor(compCard.getColor());
				noPickRequired();
				if (!discardPile.back().getSkip())
				{
					playerTurn = !playerTurn;
				}
				break;
			}
		}
		if (!colorSelected)
		{
			discardPile.back().setColor(availableCardColors.at(std::rand() % 4));
		}
		clock.restart();
	}
}

bool UnoGame::isPlayerTurn()
{
	return playerTurn;
}

sf::Color UnoGame::getDiscardColor()
{
	return discardPile.back().getColor();
}

bool UnoGame::colorPickRequired()
{
	return discardPile.back().getColor() == sf::Color::Black;
}

void UnoGame::noPickRequired()
{
	colorPick = false;
}

void UnoGame::drawCard(int playerNum)
{
	clock.restart();
	if (!colorPickRequired())
	{
		UnoCard currCard = drawPile.back();
		if (isLegalCard(currCard))
		{
			playCard(currCard);
			drawPile.pop_back();
		}
		else
		{
			if (playerNum == 0)
			{
				currCard.setFaceDown(false);
			}
			players.at(playerNum).getHand().push_back(currCard);
			drawPile.pop_back();
			playerTurn = !playerTurn;
		}
	}

	else //colorPick IS Required
	{
		std::cout << "weird part of code" << std::endl;
	}
	

	
}

std::vector<UnoCard>& UnoGame::getHandofPlayer(int p)
{
	return players.at(p).getHand();
}

std::vector<UnoCard>& UnoGame::getButtons()
{
	return buttons;
}



std::vector<UnoCard> UnoGame::setupDeck()
{
	std::vector<UnoCard> deck;
	sf::Color defaultColor = sf::Color::Black;

	for (int c = 0; c < NUM_COLORS; c++)
	{
		//Zero Card
		UnoCard zeroCard(availableCardColors[c], 0, false, false, 0, font);
		deck.push_back(zeroCard);
		//Number Cards (2 of each)
		for (int v = 1; v <= 9; v++)
		{
			UnoCard numCard1(availableCardColors[c], v, false, false, 0, font);
			deck.push_back(numCard1);
			UnoCard numCard2(availableCardColors[c], v, false, false, 0, font);
			deck.push_back(numCard2);
		}

		//Utility Cards (2 of each)
		for (int u = 1; u <= 2; u++)
		{
			//Skip
			UnoCard skipCard(availableCardColors[c], 20, true, false, 0, font);
			deck.push_back(skipCard);
			//Reverse
			UnoCard reverseCard(availableCardColors[c], 20, false, true, 0, font);
			deck.push_back(reverseCard);
			//Draw 2
			UnoCard draw2Card(availableCardColors[c], 20, true, false, 2, font);
			deck.push_back(draw2Card);
			//Draw 3
			UnoCard draw3Card(availableCardColors[c], 20, true, false, 3, font);
			deck.push_back(draw3Card);
		}

		//Wild Draw 4
		UnoCard drawWildCard(sf::Color::Black, 50, true, false, 4, font);
		deck.push_back(drawWildCard);
		//Wild
		UnoCard wildCard(sf::Color::Black, 50, false, false, 0, font);
		deck.push_back(wildCard);

	}
	
	return deck;
}

std::vector<UnoCard> UnoGame::shuffleDeck(std::vector<UnoCard> d)
{
	std::vector<UnoCard> newDeck;
	for (int c = 0; c < d.size(); c++)
	{
		newDeck.push_back(d.at(c));
	}


	int randCardIndex;
	for (int c = 0; c < newDeck.size(); c++)
	{
		UnoCard tempRef = newDeck.at(c);
		randCardIndex = std::rand() % newDeck.size();
		newDeck.at(c) = newDeck.at(randCardIndex);
		newDeck.at(randCardIndex) = tempRef;
	}
	return newDeck;
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
		UnoCard keepPlaying = discardPile.back();
		discardPile.pop_back();
		//std::cout << "keep playing is: " << keepPlaying->getReadableCard() << std::endl;

		//set up a fresh deck and insert the discard and any remaining draw cards
		std::vector<UnoCard> newDeck;
		newDeck.reserve(drawPile.size() + discardPile.size());
		newDeck.insert(newDeck.end(), drawPile.begin(), drawPile.end());
		newDeck.insert(newDeck.end(), discardPile.begin(), discardPile.end());

		//Reset wilds that were played onto discard pile
		for (int c = 0; c < newDeck.size(); c++)
		{
			//Wilds always have a value of 50
			if (newDeck.at(c).getValue() == 50)
			{
				newDeck.at(c).setColor(sf::Color::Black);
			}
			newDeck.at(c).setFaceDown(true);
		}

		//shuffle the new deck and reset draw pile to that new deck
		drawPile = shuffleDeck(newDeck);

		//clear the discard pile, put the one card back there to continue the game
		discardPile.clear();
		discardPile.push_back(keepPlaying);
	}
}

void UnoGame::createGameGlobals()
{
	//Set up golden reference cards
	REAL_CARDS = setupDeck();

	//TODO add more players
	players.push_back(*new Player("Alec"));
	players.push_back(*new Player("Jordan (BOT)"));

	//Set up buttons

	for (int b = 0; b < NUM_COLORS; b++)
	{
		UnoCard button(availableCardColors[b], -1, false, false, 0, font);
		buttons.push_back(button);
	}
}

std::vector<Player>& UnoGame::getPlayers()
{
	return players;
}

std::vector<UnoCard>& UnoGame::getDrawPile()
{
	return drawPile;
}

std::vector<UnoCard>& UnoGame::getDiscardPile()
{
	return discardPile;
}

void UnoGame::playCard(UnoCard c)
{
	std::cout << "Card played: " << c.getCardName() << " by " << getPlayers().at(!playerTurn).getName() << std::endl;
	clock.restart();
	c.setFaceDown(false);
	discardPile.push_back(c);

	//if it's wild, set colorPick = true and delay the turn increment
	if (c.getColor() == sf::Color::Black)
	{
		colorPick = true;
	}
	//increment turn if there are no skips or no reverse
	else if (!c.getSkip() && !c.getReverse())
	{
		playerTurn = !playerTurn;
	}
	
	//playerTurn = 0 not my turn
	//playersat(playerTurn) = me

	//Draw any pending drawCards
	for (int d = 1; d <= c.getDrawCards(); d++)
	{
		if (!isPlayerTurn())
			drawPile.back().setFaceDown(false);

		players.at(playerTurn).receiveCard(drawPile.back());
		drawPile.pop_back();
	}
}

//Click Handling

bool UnoGame::drawClicked(int x, int y)
{
	if (drawPile.back().getBounds().contains(x, y))
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
		std::vector<UnoCard>& currHand = players.at(0).getHand();
		//For each card in human's hand:
		for (int c = 0; c < currHand.size(); c++)
		{
			UnoCard& currCard = currHand.at(c);
			if (currCard.getBounds().contains(x, y))
			{
				if (isLegalCard(currCard))
				{
					playCard(currCard);
					currHand.erase(currHand.begin() + c);
				}
			}
		}
	}
	return false;
}

bool UnoGame::isLegalCard(UnoCard c)
{
	UnoCard discardCard = discardPile.back();

	if (c.getColor() == sf::Color::Black)
	{
		return true;
	}
	else if (c.getColor() == discardCard.getColor())
	{
		return true;
	}
	else if (c.getValue() <= 9 && c.getValue() == discardCard.getValue())
	{
		return true;
	}
	//if skip, reverse, and drawCards are all exactly the same, that means it's the same utility card (20 points)
	else if ((c.getSkip() == discardCard.getSkip() && c.getReverse() == discardCard.getReverse()) && (c.getDrawCards() == discardCard.getDrawCards() && c.getValue() == 20))
	{
		return true;
	}
	return false;
}

bool UnoGame::buttonsClicked(int x, int y, bool incrementTurn)
{
	for (int b = 0; b < NUM_COLORS; b++)
	{
		if (buttons.at(b).getBounds().contains(x, y))
		{
			if (incrementTurn)
				playerTurn = !playerTurn;
			colorPick = false;
			discardPile.back().setColor(buttons.at(b).getColor());
			return true;
		}
	}
	return false;
}

int UnoGame::over()
{
	if (players.at(0).getHand().empty())
	{
		return 1;
	}
	else if (players.at(1).getHand().empty())
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
	std::vector<UnoCard> losingHand = players.at(!winnerIndex).getHand();
	int score = 0;

	//Loop through losing player's hand, get total score
	for (int i = 0; i < losingHand.size(); i++)
	{
		score += losingHand.at(i).getValue();
	}

	players.at(winnerIndex).addScore(score);
	latestScore = score;
}

int UnoGame::getLatestScore()
{
	return latestScore;
}