#include "UnoGame.h"

int UnoGame::NUM_COLORS = 6;

UnoGame::UnoGame()
{

}

UnoGame::UnoGame(int d)
{
	difficulty = d;

	//Random Seed
	srand(time(NULL));

	//Runs once
	createGameGlobals();

	//Runs each game
	setupBeginGame();
}

void UnoGame::setDelay(int ms)
{
	COMPUTER_DELAY_MS = ms;
}

void UnoGame::setupBeginGame()
{
	drawPile.clear();

	//Set up golden reference cards
	//setupDeck(REAL_CARDS);
	setupDeck(drawPile);

	std::cout << "Deck Created" << std::endl;

	shuffleDeck(drawPile);

	std::cout << "Deck Shuffled" << std::endl;

	discardPile.clear();

	playerTurn = false;
	colorPick = false;

	for (int p = 0; p < Resources::NUM_PLAYERS; p++)
	{
		players.at(p)->clearHand();
	}
	
	//Number of cards in hand
	int NUM_CARDS_IN_STARTING_HAND = 7;
	
	for (int d = 1; d <= NUM_CARDS_IN_STARTING_HAND; d++)
	{
		drawPile.back()->setFaceDown(false);
		//player "0" human receives currCard
		players.at(0)->receiveCard(std::move(drawPile.back()));
		drawPile.pop_back(); //do I need to pop this?

		//player "1" computer receives currCard
		players.at(1)->receiveCard(std::move(drawPile.back()));
		drawPile.pop_back(); //or this?
	}

	//Initial card flip draw >=+3 protection
	while (drawPile.back()->getDrawCards() > 2)
	{
		shuffleDeck(drawPile);
	}

	std::unique_ptr<UnoCard> drawCard = std::move(drawPile.back());
	drawPile.pop_back();

	playCard(std::move(drawCard), false);

	//if first card is Wild, player needs to select color.
	if (discardPile.back()->getColor() == sf::Color::Black)
	{
		playerTurn = true;
	}
}

//High Level Accessors

//First card that can be played will be
void UnoGame::runBasicComputerMoveOnPlayer(int playerNum)
{
	if (CPUMoveTimerClock.getElapsedTime().asMilliseconds() < COMPUTER_DELAY_MS)
	{
		return;
	}

	//switch indicates if a card is played this turn
	bool cardPlayed = false;

	//if no color pick is required
	if (!colorPickRequired())
	{
		//loop through the computer hand
		for (int c = 0; c < players.at(playerNum)->getHand().size(); c++)
		{
			
			if (isLegalCardNonWild(players.at(playerNum)->getHand().at(c)))
			{
				playCard(std::move(players.at(playerNum)->getHand().at(c)), false);
				players.at(playerNum)->getHand().erase(players.at(playerNum)->getHand().begin() + c);
				cardPlayed = true;
				break;
			}
		}

		//if a color, number, or utility hasn't been played yet,
		if (!cardPlayed)
		{
			//cycle through our hand again
			for (int c = 0; c < players.at(playerNum)->getHand().size(); c++)
			{
				
				//looking for wild cards to play. (any legal card technically, but only wilds will be left at this point)
				if (isLegalCard(players.at(playerNum)->getHand().at(c)))
				{
					playCard(std::move(players.at(playerNum)->getHand().at(c)), false);
					players.at(playerNum)->getHand().erase(players.at(playerNum)->getHand().begin() + c);
					cardPlayed = true;
					break;
				}
			}
		}

		//if we still haven't played anything, we must draw.
		if (!cardPlayed)
			drawCard(playerNum);
	}
	else //colorPick IS Required
	{
		bool colorSelected = false;
		for (int c = 0; c < players.at(playerNum)->getHand().size(); c++)
		{

			if (players.at(playerNum)->getHand().at(c)->getColor() != sf::Color::Black)
			{
				colorSelected = true;
				discardPile.back()->setColor(players.at(playerNum)->getHand().at(c)->getColor());
				noPickRequired();
				break;
			}
		}

		if (!colorSelected)
		{
			discardPile.back()->setColor(Resources::availableCardColors[std::rand() % difficulty]);
		}

		if (!discardPile.back()->getSkip())
		{
			playerTurn = !playerTurn;
		}

		CPUMoveTimerClock.restart();
	}
}

//returns the index of the card that chains from the hand
int UnoGame::checkChain(std::vector<std::unique_ptr<UnoCard>>& hand, std::unique_ptr<UnoCard>& card)
{
	for (int c = 0; c < hand.size(); c++)
	{
		if (checkLegalChainMove(card, hand.at(c)))
		{
			return c;
		}
	}
	return 0;
}

 
bool UnoGame::checkLegalChainMove(std::unique_ptr<UnoCard>& discardCard, std::unique_ptr<UnoCard>& cardToPlay)
{
	if (cardToPlay->getSkip())
	{
		if (cardToPlay->getColor() == sf::Color::Black)
		{
			return true;
		}
		else if (cardToPlay->getColor() == discardCard->getColor())
		{
			return true;
		}
		else if (cardToPlay->getValue() <= 9 && cardToPlay->getValue() == discardCard->getValue())
		{
			return true;
		}
		//if skip, reverse, and drawCards are all exactly the same, that means it's the same utility card (20 points)
		else if ((cardToPlay->getSkip() == discardCard->getSkip() && cardToPlay->getReverse() == discardCard->getReverse()) && (cardToPlay->getDrawCards() == discardCard->getDrawCards() && cardToPlay->getValue() == 20))
		{
			return true;
		}
	}

	return false;
}


void UnoGame::runAdvancedComputerMoveOnPlayer(int playerNum)
{
	if (CPUMoveTimerClock.getElapsedTime().asMilliseconds() < COMPUTER_DELAY_MS)
	{
		return;
	}

	//switch indicates if a card is played this turn
	bool cardPlayed = false;

	//if no color pick is required
	if (!colorPickRequired())
	{
		//check if we can win
		//only have 1 card that's not a skip
		int numPlainCards = 0;
		for (int c = 0; c < players.at(playerNum)->getHand().size(); c++)
		{
			//if the card can't skip, we consider it a plain card for this purpose.
			if(!players.at(playerNum)->getHand().at(c)->getSkip())
				numPlainCards++;
		}

		//Panic mode - opponent is about to go out OR we only have 1 plain card, so try to skip into end.
		if (players.at(!playerNum)->getHand().size() < 3 || numPlainCards == 1)
		{
			for (int c = 0; c < players.at(playerNum)->getHand().size(); c++)
			{
				if (isLegalCard(players.at(playerNum)->getHand().at(c)) && (players.at(playerNum)->getHand().at(c)->getDrawCards() > 0))
				{
					playCard(std::move(players.at(playerNum)->getHand().at(c)), false);
					players.at(playerNum)->getHand().erase(players.at(playerNum)->getHand().begin() + c);
					cardPlayed = true;
					break;
				}
			}
		}

		//fix this later, need to check if we can win first before this thing
		//check the draw pile for rare cards and draw if it's blue or better
		if (drawPile.back()->getDrawCards() >= 3)
		{
			drawCard(playerNum);
			cardPlayed = true;
		}

		//loop through the computer hand
		if (!cardPlayed)
		{
			for (int c = 0; c < players.at(playerNum)->getHand().size(); c++)
			{
				if (isLegalCardNonWild(players.at(playerNum)->getHand().at(c)))
				{
					playCard(std::move(players.at(playerNum)->getHand().at(c)), false);
					players.at(playerNum)->getHand().erase(players.at(playerNum)->getHand().begin() + c);
					cardPlayed = true;
					break;
				}
			}
		}
		
		//if a color, number, or utility hasn't been played yet,
		if (!cardPlayed)
		{
			//cycle through our hand again
			for (int c = 0; c < players.at(playerNum)->getHand().size(); c++)
			{
				//looking for wild cards to play. (any legal card technically, but only wilds will be left at this point)
				if (isLegalCard(players.at(playerNum)->getHand().at(c)))
				{
					playCard(std::move(players.at(playerNum)->getHand().at(c)), false);
					players.at(playerNum)->getHand().erase(players.at(playerNum)->getHand().begin() + c);
					cardPlayed = true;
					break;
				}
			}
		}

		//if we still haven't played anything, we must draw.
		if (!cardPlayed)
			drawCard(playerNum);
	}
	else //colorPick IS Required
	{
		bool colorSelected = false;
		for (int c = 0; c < players.at(playerNum)->getHand().size(); c++)
		{

			if (players.at(playerNum)->getHand().at(c)->getColor() != sf::Color::Black)
			{
				colorSelected = true;
				discardPile.back()->setColor(players.at(playerNum)->getHand().at(c)->getColor());
				noPickRequired();
				break;
			}
		}

		if (!colorSelected)
		{
			discardPile.back()->setColor(Resources::availableCardColors[std::rand() % difficulty]);
		}

		if (!discardPile.back()->getSkip())
		{
			playerTurn = !playerTurn;
		}

		CPUMoveTimerClock.restart();
	}
}

bool UnoGame::isPlayerTurn()
{
	return playerTurn;
}

sf::Color UnoGame::getDiscardColor()
{
	return discardPile.back()->getColor();
}

bool UnoGame::colorPickRequired()
{
	return discardPile.back()->getColor() == sf::Color::Black;
}

void UnoGame::noPickRequired()
{
	colorPick = false;
}

void UnoGame::drawCard(int playerNum)
{
	
	std::cout << "Card draw:   " << drawPile.back()->getCardName() << " by " << players.at(playerNum)->getName() << std::endl;
	std::unique_ptr<UnoCard> drawCard = std::move(drawPile.back());
	drawPile.pop_back();

	if (isLegalCard(drawCard))
	{
		playCard(std::move(drawCard), false);
	}
	else
	{
		if (playerNum == 0)
		{
			drawCard->setFaceDown(false);
		}
		players.at(playerNum)->receiveCard(std::move(drawCard));
		playerTurn = !playerTurn;
	}
	CPUMoveTimerClock.restart();
}

std::vector<std::unique_ptr<UnoCard>>& UnoGame::getButtons()
{
	return buttons;
}

void UnoGame::setupDeck(std::vector<std::unique_ptr<UnoCard>>& real_cards)
{
	//sf::Color defaultColor = sf::Color::Black;
	for (int c = 0; c < difficulty; c++)
	{
		//Zero Card (1 of each color)
		std::unique_ptr<UnoCard> zeroCard = std::make_unique<UnoCard>(Resources::availableCardColors[c], 0, false, false, 0, difficulty, Resources::CARD_FONT);
		real_cards.push_back(std::move(zeroCard));
		//Number Cards (2 of each color)
		for (int v = 1; v <= 9; v++)
		{
			std::unique_ptr<UnoCard> numCard1 = std::make_unique<UnoCard>(Resources::availableCardColors[c], v, false, false, 0, difficulty, Resources::CARD_FONT);
			real_cards.push_back(std::move(numCard1));
			std::unique_ptr<UnoCard> numCard2 = std::make_unique<UnoCard>(Resources::availableCardColors[c], v, false, false, 0, difficulty, Resources::CARD_FONT);
			real_cards.push_back(std::move(numCard2));
		}

		//Utility Cards (2 of each color)
		for (int u = 1; u <= 2; u++)
		{
			//Skip
			std::unique_ptr<UnoCard> skipCard = std::make_unique<UnoCard>(Resources::availableCardColors[c], 20, true, false, 0, difficulty, Resources::CARD_FONT);
			real_cards.push_back(std::move(skipCard));
			//Reverse
			std::unique_ptr<UnoCard> reverseCard = std::make_unique<UnoCard>(Resources::availableCardColors[c], 20, false, true, 0, difficulty, Resources::CARD_FONT);
			real_cards.push_back(std::move(reverseCard));
			//Draw 2
			std::unique_ptr<UnoCard> draw2Card = std::make_unique<UnoCard>(Resources::availableCardColors[c], 20, true, false, 2, difficulty, Resources::CARD_FONT);
			real_cards.push_back(std::move(draw2Card));
			//Draw 3 for hard + expert
			if (difficulty >= 5)
			{
				//Draw 3
				std::unique_ptr<UnoCard> draw3Card = std::make_unique<UnoCard>(Resources::availableCardColors[c], 20, true, false, 3, difficulty, Resources::CARD_FONT);
				real_cards.push_back(std::move(draw3Card));
				//Skip
				std::unique_ptr<UnoCard> skipCard = std::make_unique<UnoCard>(Resources::availableCardColors[c], 20, true, false, 0, difficulty, Resources::CARD_FONT);
				real_cards.push_back(std::move(skipCard));
				//Reverse
				std::unique_ptr<UnoCard> reverseCard = std::make_unique<UnoCard>(Resources::availableCardColors[c], 20, false, true, 0, difficulty, Resources::CARD_FONT);
				real_cards.push_back(std::move(reverseCard));
				//Draw 2
				std::unique_ptr<UnoCard> draw2Card = std::make_unique<UnoCard>(Resources::availableCardColors[c], 20, true, false, 2, difficulty, Resources::CARD_FONT);
				real_cards.push_back(std::move(draw2Card));
			}
		}

		//Wild Draw 4 (1 for each color)
		std::unique_ptr<UnoCard> drawWildCard = std::make_unique<UnoCard>(sf::Color::Black, 50, true, false, 4, difficulty, Resources::CARD_FONT);
		real_cards.push_back(std::move(drawWildCard));
		//Wild (1 for each color)
		std::unique_ptr<UnoCard> wildCard = std::make_unique<UnoCard>(sf::Color::Black, 50, false, false, 0, difficulty, Resources::CARD_FONT);
		real_cards.push_back(std::move(wildCard));
	}

	if (difficulty >= 6)
	{
		//Draw 8
		std::unique_ptr<UnoCard> draw8Card = std::make_unique<UnoCard>(sf::Color::Black, 300, true, false, 8, difficulty, Resources::CARD_FONT);
		real_cards.push_back(std::move(draw8Card));
	}
}

void UnoGame::shuffleDeck(std::vector<std::unique_ptr<UnoCard>>& deck)
{
	int randCardIndex;
	
	for (int c = 0; c < deck.size(); c++)
	{
		std::unique_ptr<UnoCard> tempCard = std::move(deck.at(c));
		randCardIndex = std::rand() % deck.size();
		deck.at(c) = std::move(deck.at(randCardIndex));
		deck.at(randCardIndex) = std::move(tempCard);
	}
	
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
		std::unique_ptr<UnoCard> keepPlaying = std::move(discardPile.back());
		discardPile.pop_back();
		while(!discardPile.empty())
		{
			if (discardPile.back()->getValue() >= 50)
			{
				discardPile.back()->setColor(sf::Color::Black);
			}
			discardPile.back()->setFaceDown(true);
			drawPile.push_back(std::move(discardPile.back()));
			discardPile.pop_back();
		}

		//shuffle the deck
		shuffleDeck(drawPile);

		//clear the discard pile, put the one card back there to continue the game
		discardPile.clear();
		discardPile.push_back(std::move(keepPlaying));
	}
}

void UnoGame::createGameGlobals()
{
	gameCount = 0;
	highestBounty = 0;
	//Create Players
	std::unique_ptr<Player> player1 = std::make_unique<Player>("Human");
	std::unique_ptr<Player> player2 = std::make_unique<Player>("Computer");

	players.push_back(std::move(player1));
	players.push_back(std::move(player2));

	//Create Text for Score and Hands
	sf::Text handValue0; sf::Text handValue1;
	sf::Text score0; sf::Text score1;


	handValue0.setFont(Resources::CARD_FONT); handValue1.setFont(Resources::CARD_FONT);
	handValue0.setCharacterSize(Resources::FONT_SIZE_NORMAL); handValue1.setCharacterSize(Resources::FONT_SIZE_NORMAL);

	score0.setFont(Resources::CARD_FONT); score1.setFont(Resources::CARD_FONT);
	score0.setCharacterSize(Resources::FONT_SIZE_LARGE); score1.setCharacterSize(Resources::FONT_SIZE_LARGE);

	handValues.push_back(handValue0); handValues.push_back(handValue1);
	scoreText.push_back(score0); scoreText.push_back(score1);

	gameCountText.setFont(Resources::CARD_FONT);
	gameCountText.setString("Games: " + std::to_string(gameCount));
	gameCountText.setCharacterSize(Resources::FONT_SIZE_SMALL);

	statsText.setFont(Resources::CARD_FONT);
	statsText.setString("Highest Bounty Collected: " + std::to_string(highestBounty));
	statsText.setCharacterSize(Resources::FONT_SIZE_NORMAL);

	//Set up buttons

	for (int b = 0; b < difficulty; b++)
	{
		std::unique_ptr<UnoCard> button = std::make_unique<UnoCard>(Resources::availableCardColors[b], -1, false, false, 0, difficulty, Resources::CARD_FONT);
		buttons.push_back(std::move(button));
	}

	//Set up positions of deck + discard
	deckPos_x = Resources::WINDOW_WIDTH / 2;
	deckPos_y = (Resources::WINDOW_HEIGHT / 8) * 3;
	discardPos_x = Resources::WINDOW_WIDTH / 2 - Resources::CARD_WIDTH - Resources::CARD_SPACING * 5;
	discardPos_y = deckPos_y;

	buttonsPos_x = (Resources::WINDOW_WIDTH / 2) - (((Resources::CARD_WIDTH * buttons.size()) + (Resources::CARD_SPACING * (buttons.size() - 1))) / 2);
	buttonsPos_y = ((Resources::WINDOW_HEIGHT / Resources::SCREEN_SECTIONS_RESOLUTION) * 5);
}

std::vector<std::unique_ptr<Player>>& UnoGame::getPlayers()
{
	return players;
}

std::vector<std::unique_ptr<UnoCard>>& UnoGame::getDrawPile()
{
	return drawPile;
}

std::vector<std::unique_ptr<UnoCard>>& UnoGame::getDiscardPile()
{
	return discardPile;
}

void UnoGame::updateAllTargetCardPos()
{
	//Fan out the player's hands
	players.at(0)->fanOutHand(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 8 * 6.5);
	players.at(1)->fanOutHand(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 8);

	//Set up positions of deck + discard
	deckPos_x = Resources::WINDOW_WIDTH / 2;
	deckPos_y = (Resources::WINDOW_HEIGHT / 8) * 3;
	discardPos_x = Resources::WINDOW_WIDTH / 2 - Resources::CARD_WIDTH - Resources::CARD_SPACING * 5;
	discardPos_y = deckPos_y;

	buttonsPos_x = (Resources::WINDOW_WIDTH / 2) - (((Resources::CARD_WIDTH * buttons.size()) + (Resources::CARD_SPACING * (buttons.size() - 1))) / 2);
	buttonsPos_y = ((Resources::WINDOW_HEIGHT / Resources::SCREEN_SECTIONS_RESOLUTION) * 5);
	
	//offset for deck
	offset = Resources::CARD_SPACING - 5;
	for (int c = drawPile.size() - 1; c >= 0; c--)
	{
		drawPile.at(c)->setTargetPosition(deckPos_x, deckPos_y);
		deckPos_x += offset;
	}

	//offset for discard
	offset = Resources::CARD_SPACING + Resources::CARD_SPACING / 2;
	
	for (int c = discardPile.size() - 1; c >= 0; c--)
	{
		discardPile.at(c)->setTargetPosition(discardPos_x, discardPos_y);
		discardPos_x -= offset;
	}

	offset = Resources::CARD_WIDTH + Resources::CARD_SPACING;
	//Fan out buttons
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons.at(i)->setTargetPosition(buttonsPos_x, buttonsPos_y);
		buttonsPos_x += offset;
	}
}

void UnoGame::playCard(std::unique_ptr<UnoCard> card, bool popDrawCardFirst)
{
	if (popDrawCardFirst)
	{
		drawPile.pop_back();
	}
	std::cout << "Card played: " << card->getCardName() << " by " << getPlayers().at(!playerTurn)->getName() << std::endl;

	card->setFaceDown(false);
	//if it's wild, set colorPick = true and delay the turn increment
	if (card->getColor() == sf::Color::Black)
	{
		colorPick = true;
	}
	//increment turn if there are no skips or no reverse
	else if (!card->getSkip() && !card->getReverse())
	{
		playerTurn = !playerTurn;
	}
	
	//Draw any pending drawCards
	for (int d = 1; d <= card->getDrawCards(); d++)
	{
		if (!isPlayerTurn())
			drawPile.back()->setFaceDown(false);

		players.at(playerTurn)->receiveCard(std::move(drawPile.back()));
		drawPile.pop_back();
	}

	discardPile.push_back(std::move(card));
	CPUMoveTimerClock.restart();
}

//Click Handling

bool UnoGame::drawClicked(int x, int y)
{
	if (drawPile.back()->getBounds().contains(x, y))
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
		std::vector<std::unique_ptr<UnoCard>>& currHand = getHandOfPlayer(0);
		//For each card in human's hand:
		for (int c = 0; c < currHand.size(); c++)
		{
			if (currHand.at(c)->getBounds().contains(x, y))
			{
				if (isLegalCard(currHand.at(c)))
				{
					playCard(std::move(players.at(0)->giveCard(c)), false);
				}
			}
		}
	}
	return false;
}

bool UnoGame::buttonsClicked(int x, int y, bool incrementTurn)
{

	//Check for button click first
	for (int b = 0; b < buttons.size(); b++)
	{
		if (buttons.at(b)->getBounds().contains(x, y))
		{
			if (incrementTurn)
				playerTurn = !playerTurn;
			colorPick = false;
			discardPile.back()->setColor(buttons.at(b)->getColor());
			CPUMoveTimerClock.restart();
			return true;
		}
	}

	//Auto play
	std::vector<std::unique_ptr<UnoCard>>& currHand = getHandOfPlayer(0);
	//For each card in human's hand:
	for (int c = 0; c < currHand.size(); c++)
	{
		
		if (currHand.at(c)->getBounds().contains(x, y))
		{
			discardPile.back()->setColor(currHand.at(c)->getColor());
			if (!incrementTurn)
			{
				playCard(std::move(currHand.at(c)), false);
				players.at(0)->getHand().erase(players.at(0)->getHand().begin() + c);
			}
			else
				playerTurn = !playerTurn;
			CPUMoveTimerClock.restart();
			return true;
		}
	}

	return false;
}

//Helper functions

bool UnoGame::isLegalCard(std::unique_ptr<UnoCard>& card)
{
	if (card->getColor() == sf::Color::Black)
	{
		return true;
	}
	else if (card->getColor() == discardPile.back()->getColor())
	{
		return true;
	}
	else if (card->getValue() <= 9 && card->getValue() == discardPile.back()->getValue())
	{
		return true;
	}
	//if skip, reverse, and drawCards are all exactly the same, that means it's the same utility card (20 points)
	else if ((card->getSkip() == discardPile.back()->getSkip() && card->getReverse() == discardPile.back()->getReverse()) && (card->getDrawCards() == discardPile.back()->getDrawCards() && card->getValue() == 20))
	{
		return true;
	}
	return false;
}

bool UnoGame::isLegalCardNonWild(std::unique_ptr<UnoCard>& card)
{
	if (card->getColor() != sf::Color::Black)
	{
		if (card->getColor() == discardPile.back()->getColor())
		{
			return true;
		}
		else if (card->getValue() <= 9 && card->getValue() == discardPile.back()->getValue())
		{
			return true;
		}
		//if skip, reverse, and drawCards are all exactly the same, that means it's the same utility card (20 points)
		else if ((card->getSkip() == discardPile.back()->getSkip() && card->getReverse() == discardPile.back()->getReverse()) && (card->getDrawCards() == discardPile.back()->getDrawCards() && card->getValue() == 20))
		{
			return true;
		}
	}
	return false;
}

int UnoGame::over()
{
	if (players.at(0)->getHand().empty())
	{
		return 1;
	}
	else if (players.at(1)->getHand().empty())
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

void UnoGame::updateAllRealCardPos(sf::Time dt)
{
	handValues.at(0).setPosition(Resources::WINDOW_WIDTH / 2 - handValues.at(0).getGlobalBounds().width / 2, Resources::WINDOW_HEIGHT / 15 * 14);
	handValues.at(1).setPosition(Resources::WINDOW_WIDTH / 2 - handValues.at(1).getGlobalBounds().width / 2, Resources::WINDOW_HEIGHT / 15 - handValues.at(1).getGlobalBounds().height / 2);

	scoreText.at(0).setPosition(Resources::WINDOW_WIDTH / 20 * 16, Resources::WINDOW_HEIGHT / 15 * 14);
	scoreText.at(1).setPosition(Resources::WINDOW_WIDTH / 20 * 16, Resources::WINDOW_HEIGHT / 15 - scoreText.at(1).getGlobalBounds().height / 2);

	gameCountText.setPosition(Resources::WINDOW_WIDTH / 22, Resources::WINDOW_HEIGHT / 15 * 13 + (gameCountText.getGlobalBounds().height * 2.5));

	statsText.setPosition(Resources::WINDOW_WIDTH / 22, Resources::WINDOW_HEIGHT - (statsText.getGlobalBounds().height * 2));
	
	//For each player, loop through their hand and updatePosition
	for (int p = 0; p < 2; p++)
	{
		//For each card in player's hand
		for (int c = 0; c < players.at(p)->getHand().size(); c++)
		{
			players.at(p)->getHand().at(c)->updatePosition(dt);
		}
	}

	//Loop through discard pile to update positions
	for (int c = 0; c < discardPile.size(); c++)
	{
		discardPile.at(c)->updatePosition(dt);
	}

	//Loop through draw pile to update positions
	for (int c = 0; c < drawPile.size(); c++)
	{
		drawPile.at(c)->updatePosition(dt);
	}

	for (int c = 0; c < buttons.size(); c++)
	{
		buttons.at(c)->updatePosition(dt);
	}
}

int UnoGame::getGameCount()
{
	return gameCount;
}

void UnoGame::runOnceOnEndGame()
{
	gameCount++;
	gameCountText.setString("Games: " + std::to_string(gameCount));
	//over() returns 1 or 2 for player's 0 or 1 win state, so subtract 1 to get the index of the winner
	int winnerIndex = over() - 1;
	int latestScore = players.at(!winnerIndex)->getHandValue();
	players.at(winnerIndex)->addScore(latestScore);
	std::cout << "[SCORE] Adding " << latestScore << " to " << players.at(winnerIndex)->getName() << "'s score. They now have " << players.at(winnerIndex)->getScore() << " points." << std::endl;
	//check for highest bounty for humans
	if (!winnerIndex)
	{
		if (latestScore > highestBounty)
		{
			highestBounty = latestScore;
			statsText.setString("Highest Bounty Collected: " + std::to_string(highestBounty));
		}
	}
}


std::vector<std::unique_ptr<UnoCard>>& UnoGame::getHandOfPlayer(int p)
{
	return players.at(p)->getHand();
}

void UnoGame::resetPlayers()
{
	for (int i = 0; i < Resources::NUM_PLAYERS; i++)
	{
		players.at(i).reset();
	}
}

void UnoGame::updateHandValueText()
{
	for (int p = 0; p < Resources::NUM_PLAYERS; p++)
	{
		handValues.at(p).setString("Hand Value: " + std::to_string(players.at(p)->getHandValue()));
	}
	
}

void UnoGame::updateScoreText()
{
	for (int p = 0; p < Resources::NUM_PLAYERS; p++)
	{
		scoreText.at(p).setString("Score: " + std::to_string(players.at(p)->getScore()));
	}
}

sf::Text UnoGame::getHandValueText(int p)
{
	return handValues.at(p);
}

sf::Text UnoGame::getGameCountText()
{
	return gameCountText;
}

sf::Text UnoGame::getScoreText(int p)
{
	return scoreText.at(p);
}

sf::Text UnoGame::getStatsText()
{
	return statsText;
}