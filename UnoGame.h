#pragma once
#include "UnoCard.h"
#include "Player.h"

class UnoGame
{
private:
	const int COMPUTER_DELAY_MS = 500;
	const int NUM_COLORS = 6;
	std::vector<sf::Color> availableCardColors { sf::Color(180, 0, 0), sf::Color(220, 170, 0) , sf::Color(0, 180, 0), sf::Color(0, 0, 180), sf::Color(220,100,0), sf::Color(180, 0, 220)};

	std::vector<Player> players;
	std::vector<UnoCard> buttons;
	std::vector<UnoCard> drawPile;
	std::vector<UnoCard> discardPile;
	std::vector<UnoCard> REAL_CARDS;
	std::vector<UnoCard> setupDeck();
	std::vector<UnoCard> shuffleDeck(std::vector<UnoCard> d);

	sf::Font font;
	void createGameGlobals();

	bool playerTurn;
	bool colorPick;
	void setupBeginGame();
	int latestScore;
	bool scoreTallied;

	sf::Clock clock;

public:
	UnoGame();
	std::vector<Player>& getPlayers();
	std::vector<UnoCard>& getDrawPile();
	std::vector<UnoCard>& getDiscardPile();
	std::vector<UnoCard>& getHandofPlayer(int p);
	std::vector<UnoCard>& getButtons();
	sf::Color getDiscardColor();
	bool isPlayerTurn();
	void playCard(UnoCard c);
	void reshuffle();
	bool drawClicked(int x, int y);
	bool handCardClicked(int x, int y);
	bool buttonsClicked(int x, int y, bool incrementTurn);
	void drawCard(int playerNum);
	bool colorPickRequired();
	void noPickRequired();
	void runBasicComputerMove();
	void tallyScore();
	int getLatestScore();
	int over();
	bool isLegalCard(UnoCard c);

};

