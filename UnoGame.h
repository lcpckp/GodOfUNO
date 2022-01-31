#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include "UnoCard.h"
#include "Player.h"
#include "Resources.h"
#include<iostream>
#include<stack>


class UnoGame
{
private:
	
	int COMPUTER_DELAY_MS = Resources::COMPUTER_DELAY_BRISK; // Default value, change speed in GameEngine.cpp

	//Game objects
	std::vector<std::unique_ptr<Player>> players; //players contain hands
	std::vector<std::unique_ptr<UnoCard>> buttons; //for choosing
	std::vector<std::unique_ptr<UnoCard>> drawPile;
	std::vector<std::unique_ptr<UnoCard>> discardPile;

	//Positions of deck and discard, player + opponent hands
	float deckPos_x; float deckPos_y; float discardPos_x; float discardPos_y;
	float playerHandPos_x; float playerHandPos_y;
	float compHandPos_x; float compHandPos_y;
	float offset;
	int buttonsPos_x; int buttonsPos_y;

	//Deck vector, deck creation, and shuffle utility
	//std::vector<std::unique_ptr<UnoCard>> REAL_CARDS;
	void setupDeck(std::vector<std::unique_ptr<UnoCard>>& real_cards);
	void shuffleDeck(std::vector<std::unique_ptr<UnoCard>>& deck);

	//Display info during game
	std::vector<sf::Text> scoreText;
	std::vector<sf::Text> handValues;
	std::vector<sf::Text> playerNames;
	sf::Text gameCountText;
	sf::Text statsText;
	int highestBounty;
	int gameCount;


	void createGameGlobals();
	void setupBeginGame();

	//Game Status
	bool playerTurn;
	bool colorPick;
	bool scoreTallied;
	
	//Computer Delay Clock
	sf::Clock CPUMoveTimerClock;

public:
	//Constructors
	UnoGame();
	UnoGame(int d);
	UnoGame(const UnoGame&) = delete;
	UnoGame& operator=(const UnoGame&) = delete;
	~UnoGame() = default;

	//Available Colors in Game
	static const sf::Color availableCardColors[];
	static int NUM_COLORS;
	static const int LOWEST_DIFFICULTY = 3;
	int difficulty; //dynamically set to # between 3 and NUM_COLORS

	//Accessors
	std::vector<std::unique_ptr<Player>>& getPlayers();
	std::vector<std::unique_ptr<UnoCard>>& getDrawPile();
	std::vector<std::unique_ptr<UnoCard>>& getDiscardPile();
	std::vector<std::unique_ptr<UnoCard>>& getButtons();
	sf::Color getDiscardColor();
	std::vector<std::unique_ptr<UnoCard>>& getHandOfPlayer(int p);
	int getGameCount();
	sf::Text getScoreText(int p);
	sf::Text getHandValueText(int p);
	sf::Text getGameCountText();
	sf::Text getStatsText();

	//Game status accessors
	int over();
	bool isPlayerTurn();
	bool colorPickRequired();

	bool isLegalCard(std::unique_ptr<UnoCard>& c);
	bool isLegalCardNonWild(std::unique_ptr<UnoCard>& c);

	// Updates to game or cards
	void updateAllRealCardPos(sf::Time dt); //updates real position
	void updateAllTargetCardPos();
	void playCard(std::unique_ptr<UnoCard> c, bool popDrawCardFirst);
	void drawCard(int playerNum);
	void reshuffle();
	void runOnceOnEndGame();
	void noPickRequired();
	void setDelay(int ms);
	void resetPlayers();
	void updateHandValueText();
	void updateScoreText();

	//AI scripts
	void runBasicComputerMoveOnPlayer(int playerNum);
	void runAdvancedComputerMoveOnPlayer(int playerNum);
	void runUltimateComputerMoveOnPlayer(int playerNum);
	int checkChain(std::vector<std::unique_ptr<UnoCard>>& hand, std::unique_ptr<UnoCard>& card);
	bool checkLegalChainMove(std::unique_ptr<UnoCard>& discardCard, std::unique_ptr<UnoCard>& cardToPlay);

	//Human input
	bool drawClicked(int x, int y);
	bool handCardClicked(int x, int y);
	bool buttonsClicked(int x, int y, bool incrementTurn);

};

