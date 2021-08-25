#pragma once
#include "UnoCard.h"
#include "Player.h"

class UnoGame
{
private:
	std::vector<UnoCard> buttons;
	std::vector<Player> players;
	std::vector<UnoCard> draw;
	std::vector<UnoCard> discard;
	std::vector<UnoCard> setupDeck(bool shuffle);
	std::vector<UnoCard> shuffleDeck(std::vector<UnoCard> d);

	sf::Font font;
	void createGameGlobals();

	bool playerTurn;
	bool colorPick;
	void setupBeginGame();
	int latestScore;
	bool scoreTallied;
public:
	UnoGame();
	std::vector<Player>* getPlayers();
	std::vector<UnoCard>* getDrawPile();
	std::vector<UnoCard>* getDiscardPile();
	std::vector<UnoCard>* getHandofPlayer(int p);
	std::vector<UnoCard>* getButtons();
	int getDiscardColor();
	int getDiscardUtility();
	bool isPlayerTurn();
	void playCard(UnoCard& c);
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

};

