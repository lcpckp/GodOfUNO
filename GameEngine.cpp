#include "GameEngine.h"

GameEngine::GameEngine()
{
	scoreTallied = false;
	game = *new UnoGame();
	mouseClickPos = sf::Vector2i(0, WINDOW_HEIGHT);
}

void GameEngine::pollEvents(sf::RenderWindow& mainWindow)
{
	while (mainWindow.pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::MouseButtonPressed:
			mouseClickPos = sf::Mouse::getPosition(mainWindow);
			std::cout << "[MOUSE] mouseClickPos.x : " << mouseClickPos.x << " mouseClickPos.y : " << mouseClickPos.y << std::endl;
			break;
		case sf::Event::Closed:
			mainWindow.close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				mainWindow.close();
			if (ev.key.code == sf::Keyboard::Space)
			{
				//Do Nothing
			}
			if (ev.key.code == sf::Keyboard::R)
			{
				shuffleRequest = true;
			}
			break;
		}
	}
}

void GameEngine::updateGame()
{
	if (/*game->getDrawPile()->size() < 5 ||*/ shuffleRequest)
	{
		game.reshuffle();
		shuffleRequest = false;
		std::cout << "discard pile back() card is: " << game.getDiscardColor() << std::endl;
	}

	if (!game.over())
	{
		if (game.isPlayerTurn())
		{
			if (game.colorPickRequired()) //colorPick IS required
			{
				//Set the wild card on the discard pile, to the color of the selected button card. If it's Draw4, don't increment turn.
				if (game.buttonsClicked(mouseClickPos.x, mouseClickPos.y, !(game.getDiscardUtility() == unoutils::Draw4)))
				{
					game.noPickRequired();
				}
			}
			else //Normal turn, colorPick NOT required
			{

				if (!game.handCardClicked(mouseClickPos.x, mouseClickPos.y))
				{

					if (!game.getDrawPile()->empty())
					{

						game.drawClicked(mouseClickPos.x, mouseClickPos.y);
					}
				}
			}
		}
		else
		{
			game.runBasicComputerMove();
		}
	}
	else //Game is over - process click to restart game
	{
		if (!scoreTallied)
		{
			game.tallyScore();
			scoreTallied = true;
		}
		else
		{
			scoreTallied = false;
		}
	}

	/* if game is over
		
		*/
	mouseClickPos = sf::Vector2i(0, WINDOW_HEIGHT);
}

void GameEngine::drawGame(sf::RenderWindow* window)
{
	//I have positions of hands, decks, discard piles, whatever else on screen stored here


	//Player 1 // INDEX 0
	std::vector<UnoCard>* currHand = game.getPlayers()->at(0).getHand();
	int numCardsInHand = currHand->size();
	for (int c = 0; c < numCardsInHand; c++)
	{
		currHand->at(c).setVisibility(true);
	}
	
	int playerHandPos_x = (WINDOW_WIDTH / 2) - (((CARD_WIDTH * numCardsInHand) + (CARD_SPACING * (numCardsInHand - 1))) / 2);
	int playerHandPos_y = (WINDOW_HEIGHT / 8) * 6.5;
	printCards(window, *game.getPlayers()->at(0).getHand(), playerHandPos_x, playerHandPos_y);

	//Player 2 // INDEX 1
	currHand = game.getPlayers()->at(1).getHand();
	numCardsInHand = currHand->size();
	for (int c = 0; c < numCardsInHand; c++)
	{
		currHand->at(c).setVisibility(false);
	}

	playerHandPos_x = (WINDOW_WIDTH / 2) - (((CARD_WIDTH * numCardsInHand) + (CARD_SPACING * (numCardsInHand - 1))) / 2);
	playerHandPos_y = (WINDOW_HEIGHT / 8);
	if (game.over())
	{
		printCards(window, *game.getPlayers()->at(1).getHand(), playerHandPos_x, playerHandPos_y);
	}
	else
	{
		printCards(window, *game.getPlayers()->at(1).getHand(), playerHandPos_x, playerHandPos_y);
	}

	//Deck + Discard Pos
	int deckPos_x = WINDOW_WIDTH / 2;
	int deckPos_y = (WINDOW_HEIGHT / 8) * 3;
	int discardPos_x = WINDOW_WIDTH / 2 - CARD_WIDTH - CARD_SPACING * 5;
	int discardPos_y = deckPos_y;

	printCardsDeck(window, *game.getDrawPile(), deckPos_x, deckPos_y);
	printCardsDiscard(window, *game.getDiscardPile(), discardPos_x, discardPos_y);

	//If game is over, print out scores
	if (game.over())
	{
		//drawWinText(window, game);
	}

	if (game.colorPickRequired() && game.isPlayerTurn())
	{
		//drawButtons(&mainWindow, *game);
		//Buttons go in the middle of the screen, minus half the width of the 4 buttons plus spacing. On the Y Axis we put it on the 5th eigth
		int numButtons = game.getButtons()->size();

		//					middle of screen  - ((( size of the buttons )   +        (size of spacing))          / 2)
		int buttonsPos_x = (WINDOW_WIDTH / 2) - (((CARD_WIDTH * numButtons) + (CARD_SPACING * (numButtons - 1))) / 2);
		int buttonsPos_y = ((WINDOW_HEIGHT / SCREEN_SECTIONS_RESOLUTION) * 5);

		printCards(window, *game.getButtons(), buttonsPos_x, buttonsPos_y);
	}
}

void GameEngine::renderGame(sf::RenderWindow& window)
{
	window.clear(sf::Color(30, 30, 30));
	drawGame(&window);
	window.display();
}

//Drawing Helper functions


//Print Cards takes starting point in window, and sets position of a set of cards (parameter 'd') to a HAND configuration.
void GameEngine::printCards(sf::RenderWindow* window, std::vector<UnoCard>& d, int x, int y)
{
	for (int i = 0; i < d.size(); i++)
	{
		d.at(i).updatePosition(x, y);
		window->draw(d.at(i));
		x += CARD_WIDTH + CARD_SPACING;
	}
}

//printCardsDeck, given a starting point in a window and a set of cards, adjusts the positions of all the cards to be in a DECK configuration.
void GameEngine::printCardsDeck(sf::RenderWindow* window, std::vector<UnoCard>& d, int x, int y)
{
	for (int i = 0; i < d.size(); i++)
	{
		d.at(i).updatePosition(x, y);
		window->draw(d.at(i));
		x += CARD_SPACING;
	}
}

void GameEngine::printCardsDiscard(sf::RenderWindow* window, std::vector<UnoCard>& d, int x, int y)
{
	
	for (int i = 0; i < d.size(); i++)
	{
		d.at(i).updatePosition(x, y);
		std::cout << d.at(i).getReadableCard();
		std::cout << d.at(i).getBaseCard().getGlobalBounds().top << std::endl;
		window->draw(d.at(i));
		x -= CARD_SPACING + 2;
	}
}

void GameEngine::drawButtons(sf::RenderWindow* window, UnoGame& game)
{
	//Buttons go in the middle of the screen, minus half the width of the 4 buttons plus spacing. On the Y Axis we put it on the 5th eigth
	int numButtons = game.getButtons()->size();

	//					middle of screen  - ((( size of the buttons )   +        (size of spacing))          / 2)
	int buttonsPos_x = (WINDOW_WIDTH / 2) - (((CARD_WIDTH * numButtons) + (CARD_SPACING * (numButtons - 1))) / 2);
	int buttonsPos_y = ((WINDOW_HEIGHT / SCREEN_SECTIONS_RESOLUTION) * 5);

	printCards(window, *game.getButtons(), buttonsPos_x, buttonsPos_y);
}