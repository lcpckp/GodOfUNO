#include "GameEngine.h"

GameEngine::GameEngine(sf::VideoMode videomode, int diff)
{
	game = std::make_unique<UnoGame>(diff);
	Resources::COMPUTER_DELAY_MS = Resources::COMPUTER_DELAY_BRISK; // Change Speed here
	videoMode1 = videomode;

	if (!Resources::autoPlay) //game settings for human
	{
		gameStarted = false;
		selectedDifficulty = diff;
	}
	else //game settings for computer
	{
		gameStarted = true;
		//game = std::make_unique<UnoGame>(diff);
		game->setDelay(Resources::COMPUTER_DELAY_MS);
	}
	
	mouseClickPos = sf::Vector2i(-5, -5);
	shuffleRequest = false;
	pauseRequest = false;
	gameEndScriptHasRun = false;

	escapeMenuIcon.setTexture(Resources::ESCtexture);
	escapeMenuIcon.setPosition(20, 30);
	escapeMenuText.setFont(Resources::CARD_FONT);
	escapeMenuText.setString("Menu");
	escapeMenuText.setCharacterSize(Resources::FONT_SIZE_NORMAL);
	escapeMenuText.setPosition(80, 32);

	automaticTextIcon.setTexture(Resources::Atexture);
	automaticTextIcon.setPosition(20, 90);
	automaticText.setFont(Resources::CARD_FONT);
	automaticText.setString("Auto-Play");
	automaticText.setCharacterSize(Resources::FONT_SIZE_NORMAL);
	automaticText.setPosition(80, 92);

	reshuffleText.setFont(Resources::CARD_FONT);
	reshuffleText.setString("Reshuffle");
	reshuffleText.setCharacterSize(Resources::FONT_SIZE_NORMAL);
	reshuffleText.setPosition(Resources::WINDOW_WIDTH / 2 - reshuffleText.getGlobalBounds().width / 2, Resources::WINDOW_HEIGHT / 2 + reshuffleText.getGlobalBounds().height / 2);
	reshuffleTextIcon.setTexture(Resources::Rtexture);
	reshuffleTextIcon.setPosition(reshuffleText.getPosition() + sf::Vector2f(-60.f, -5.f));

	musicText.setFont(Resources::CARD_FONT);
	musicText.setString("Music");
	musicText.setCharacterSize(Resources::FONT_SIZE_NORMAL);
	musicText.setPosition(80, 152);
	musicTextIcon.setTexture(Resources::Mtexture);
	musicTextIcon.setPosition(20, 150);
}

void GameEngine::pollEvents(sf::RenderWindow& mainWindow)
{
	
	while (mainWindow.pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::MouseButtonPressed:
			mouseClickPos = sf::Mouse::getPosition(mainWindow);
			//std::cout << "[MOUSE] mouseClickPos.x : " << mouseClickPos.x << " mouseClickPos.y : " << mouseClickPos.y << std::endl;
			break;
		case sf::Event::Closed:
			mainWindow.close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
			{
				pauseRequest = true;
			}
			if (ev.key.code == sf::Keyboard::A)
				Resources::autoPlay = !Resources::autoPlay;
			if (ev.key.code == sf::Keyboard::R)
				shuffleRequest = true;
			if (ev.key.code == sf::Keyboard::Up)
				Resources::COMPUTER_DELAY_MS = Resources::COMPUTER_DELAY_EXTREME;
			if (ev.key.code == sf::Keyboard::Down)
				Resources::COMPUTER_DELAY_MS = Resources::COMPUTER_DELAY_BRISK;
			if (ev.key.code == sf::Keyboard::M)
				musicRequest = true;
			break;
		}
	}
}

int GameEngine::requestingClose()
{
	return closeRequest;
}

std::unique_ptr<State> GameEngine::getNextState()
{
	return std::make_unique<PauseMenu>(videoMode1, this);
}

bool GameEngine::hasNextState()
{
	bool pause = pauseRequest;
	pauseRequest = false;
	return pause;
}

void GameEngine::update(sf::Time dt)
{
	if (gameStarted)
	{
		
		game->updateHandValueText();
		game->updateScoreText();
		
		//std::cout << "gamestarted heartbeat" << std::endl;
		game->updateAllRealCardPos(dt);
		game->updateAllTargetCardPos();

		if (!game->over())
		{
			if (game->getDrawPile().size() <= 10)
			{
				game->reshuffle();
			}

			if (game->isPlayerTurn())
			{
				if (!Resources::autoPlay)
				{
					if (game->colorPickRequired()) //colorPick IS required
					{
						//Set the wild card on the discard pile, to the color of the selected button card. (happens inside buttons clicked) If it's Draw4, don't increment turn. (parameter)
						if (game->buttonsClicked(mouseClickPos.x, mouseClickPos.y, !game->getDiscardPile().back()->getSkip()))
						{
							game->noPickRequired();
						}
					}
					else //Normal turn, colorPick NOT required
					{
						if (!game->handCardClicked(mouseClickPos.x, mouseClickPos.y))
						{
							if (!game->getDrawPile().empty())
							{
								game->drawClicked(mouseClickPos.x, mouseClickPos.y);
								game->updateAllRealCardPos(dt);
							}
						}
					}
				}
				else
				{
					game->runAdvancedComputerMoveOnPlayer(0);
				}

			}
			else
			{
				game->runAdvancedComputerMoveOnPlayer(1);
			}
			resetGameTimer.restart();
			gameEndScriptHasRun = false;
		}
		else //Game is over
		{
			if (!gameEndScriptHasRun)
			{
				game->runOnceOnEndGame();
				gameEndScriptHasRun = true;
				
				// Allow for pauses during long simulations
				if (game->getGameCount() % Resources::PAUSE_EVERY_NUM_GAMES == 0)
					Resources::DELAY_BETWEEN_GAME_MS = Resources::DELAY_EVERY_NUM_GAMES_MS;
				else
					Resources::DELAY_BETWEEN_GAME_MS = Resources::DELAY_BETWEEN_GAMES_DEFAULT;
			}

			if (shuffleRequest)
			{
				game->reshuffle();
				shuffleRequest = false;
			}

			if (Resources::autoPlay)
			{
				if (!(resetGameTimer.getElapsedTime().asMilliseconds() < Resources::DELAY_BETWEEN_GAME_MS))
				{
					game->reshuffle();
				}
			}
		}
	}
	else //game has not started
	{
		game->setDelay(Resources::COMPUTER_DELAY_MS);
		gameStarted = true;
	}

	mouseClickPos = sf::Vector2i(-500, -500);
	game->setDelay(Resources::COMPUTER_DELAY_MS);
}

void GameEngine::drawGame(sf::RenderWindow& window)
{
	if (gameStarted)
	{
		//draws DRAW pile
		std::vector<std::unique_ptr<UnoCard>>& drawDeck = game->getDrawPile();
		for (int c = 0; c < drawDeck.size(); c++)
		{
			window.draw(*drawDeck.at(c));
		}

		//discard pile
		std::vector<std::unique_ptr<UnoCard>>& discardDeck = game->getDiscardPile();
		for (int c = 0; c < discardDeck.size(); c++)
		{
			window.draw(*discardDeck.at(c));
		}
		//Draws buttons if required
		if (game->colorPickRequired() && game->isPlayerTurn())
		{
			for (int b = 0; b < selectedDifficulty; b++)
			{
				window.draw(*game->getButtons().at(b));
			}
		}

		//draws all player hands
		for (int p = 0; p < Resources::NUM_PLAYERS; p++)
		{
			std::vector<std::unique_ptr<UnoCard>>& currHand = game->getHandOfPlayer(p);
			//For each card in player's hand
			for (int c = 0; c < currHand.size(); c++)
			{
				window.draw(*currHand.at(c));
			}
		}

		for (int p = 0; p < Resources::NUM_PLAYERS; p++)
		{
			window.draw(game->getHandValueText(p));
			window.draw(game->getScoreText(p));
		}

		window.draw(game->getGameCountText());
		window.draw(automaticText);
		window.draw(automaticTextIcon);
		window.draw(escapeMenuIcon);
		window.draw(escapeMenuText);
		window.draw(musicText);
		window.draw(musicTextIcon);
		if (game->over())
		{
			window.draw(reshuffleText);
			window.draw(reshuffleTextIcon);
		}
		window.draw(game->getStatsText());
	}
}

void GameEngine::render(sf::RenderWindow& window)
{
	window.clear(sf::Color(30, 30, 30));

	drawGame(window);

	window.display();
}

void GameEngine::depositUnoGame(std::unique_ptr<UnoGame> ug)
{
	game = std::move(ug);
}

std::unique_ptr<UnoGame> GameEngine::getUnoGame()
{
	return std::move(game);
}