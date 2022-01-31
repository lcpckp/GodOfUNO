#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include "UnoCard.h"
#include "State.h"
#include "Resources.h"
#include "UnoGame.h"
#include "GameEngine.h"
#include "PauseMenu.h"


class StartMenu : public State
{
private:
	//Event handling
	sf::Vector2i mouseClickPos;
	sf::Event ev;
	sf::VideoMode startMenuVideoMode;

	//Menu Status
	bool startClicked;
	bool init;
	int difficulty;

	//Display Info
	sf::Font font;
	sf::Text titleText;
	sf::Text authorText;
	//sf::Text numColorsText;
	std::vector<std::unique_ptr<UnoCard>> buttons;
	std::unique_ptr<State> nextState;

public:
	//Constructor/Destructor
	StartMenu(sf::VideoMode videomode);

	//Init
	void setStartingPosition(); //runs once on start

	//Implement state methods
	void pollEvents(sf::RenderWindow& window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
	std::unique_ptr<State> getNextState();
	bool hasNextState();
	int requestingClose();

	//Click Handling
	bool startButtonClicked(int x, int y);
	int getDifficultyButtonClicked(int x, int y);

	//Get
	std::vector<std::unique_ptr<UnoCard>>& getButtons();
};

