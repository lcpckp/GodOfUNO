//Normal Stuff
#include<iostream>
#include<vector>
#include<chrono>
#include<thread>
#include<stack>
#include<map>

//Class Headers
#include"GameEngine.h"
#include"Resources.h"
#include"State.h"
#include"PauseMenu.h"
#include"StartMenu.h"

//SFML Libraries
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>

int main()
{
	//Random Seed
	srand(time(NULL));

	sf::Music background;
	background.openFromFile("background.wav");
	background.setLoop(true);
	background.setLoopPoints(sf::Music::TimeSpan(sf::seconds(27), sf::seconds(114)));
	background.setVolume(25);
	background.play();

	//Set up Render window and Event Listener
	sf::VideoMode videoMode1 = sf::VideoMode::getDesktopMode();
	sf::RenderWindow mainWindow(videoMode1, "God of UNO", sf::Style::Fullscreen | sf::Style::Close);

	//Set up clock for delta time
	sf::Clock clock;
	const sf::Time timePerFrame = sf::seconds(1.f / 60);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	//Set up global settings/resources
	std::unique_ptr<Resources> resource;
	resource = std::make_unique<Resources>(videoMode1);

	//Set up main game stack
	std::stack<std::unique_ptr<State>> mainStack;
	//Make and push the start menu to stack
	std::unique_ptr<State> startMenu1 = std::make_unique<StartMenu>(videoMode1);
	mainStack.push(std::move(startMenu1));

	//number of pops requested when exiting state
	int numPops = 0;
	//state passed by previous state to be pushed
	std::unique_ptr<State> nextState;


	//Main Game Loop
	while (!mainStack.empty())
	{
		//Handle Music
		if (mainStack.top()->requestingMusic())
		{
			if (background.getStatus() == sf::Music::Status::Playing)
				background.stop();
			else
			{
				background.play();
			}
		}
		
		//Main loop
		mainStack.top()->pollEvents(mainWindow);
		timeSinceLastUpdate = clock.restart(); // update delta time
		mainStack.top()->update(timeSinceLastUpdate);


		numPops = mainStack.top()->requestingClose();
		
		if (mainStack.top()->hasNextState())
			nextState = mainStack.top()->getNextState();

		if(!numPops && nextState == nullptr)
			mainStack.top()->render(mainWindow);
		else
		{
		for (int i = 0; i < numPops; i++)
			{
				mainStack.pop();
			}
			
		}

		if (nextState != nullptr)
			mainStack.push(std::move(nextState));
		numPops = 0;
	}

	return 0;
}