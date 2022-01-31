#include "StartMenu.h"



StartMenu::StartMenu(sf::VideoMode videomode)
{
    startClicked = false;
    init = false;
    difficulty = 0;

    titleText.setFont(Resources::CARD_FONT);
    titleText.setString("God of UNO");
    titleText.setCharacterSize(Resources::FONT_SIZE_XL);

    authorText.setFont(Resources::CARD_FONT);
    authorText.setString("Alec Pickup (2021)");
    authorText.setCharacterSize(Resources::FONT_SIZE_SMALL);
    
    //numColorsText.setString("Select Difficulty\n(number of colors)");
    //numColorsText.setCharacterSize(Resources::FONT_SIZE_NORMAL);
    //sf::FloatRect localTextBounds = numColorsText.getLocalBounds(); //gets bounds of text itself
    //numColorsText.setOrigin(localTextBounds.left + (localTextBounds.width / 2.f), localTextBounds.top + (localTextBounds.height / 2.f));
    //numColorsText.setPosition(Resources::WINDOW_WIDTH / 2, Resources::WINDOW_HEIGHT / 2);
    
    startMenuVideoMode = videomode;

    mouseClickPos = sf::Vector2i(-500, -500);
    setStartingPosition();
}

void StartMenu::setStartingPosition()
{
    init = true;
    nextState = nullptr;
    difficulty = 0;
    for (int b = 0; b < UnoGame::NUM_COLORS; b++)
    {
        
        std::unique_ptr<UnoCard> button = std::make_unique<UnoCard>(Resources::availableCardColors[b], -1, false, false, 0, UnoGame::NUM_COLORS, Resources::CARD_FONT);
        
        switch (b)
        {
        case 2:
            button->setTargetPosition(0, 0);
            button->setValue(3);
            break;
        case 3:
            button->setTargetPosition(Resources::WINDOW_WIDTH, 0);
            button->setValue(4);
            break;
        case 4:
            button->setTargetPosition(0, Resources::WINDOW_HEIGHT);
            button->setValue(5);
            break;
        case 5:
            button->setTargetPosition(Resources::WINDOW_WIDTH, Resources::WINDOW_HEIGHT);
            button->setValue(6);
            break;
        default:
            button->setTargetPosition((Resources::WINDOW_WIDTH / 2) - (Resources::CARD_WIDTH / 2), (Resources::WINDOW_HEIGHT / 2) - (Resources::CARD_HEIGHT / 2));
            break;
        }

        buttons.push_back(std::move(button));
    }

    std::unique_ptr<UnoCard> startButton = std::make_unique<UnoCard>(sf::Color::Black, -1, false, false, 0, 1, Resources::CARD_FONT);
    startButton->setTargetPosition((Resources::WINDOW_WIDTH / 2) - (Resources::CARD_WIDTH / 2), (Resources::WINDOW_HEIGHT / 2) - (Resources::CARD_HEIGHT / 2));
    buttons.push_back(std::move(startButton));
}

bool StartMenu::startButtonClicked(int x, int y)
{
    if (!startClicked)
    {
        startClicked = buttons.at(UnoGame::NUM_COLORS)->getBounds().contains(x, y);
    }
    else
    {
        buttons.at(2)->setTargetPosition(Resources::WINDOW_WIDTH / 2 - (Resources::CARD_WIDTH * 3) - Resources::CARD_WIDTH / 2, Resources::WINDOW_HEIGHT / 2 - (Resources::CARD_HEIGHT * 2) - Resources::CARD_HEIGHT / 2);
        buttons.at(3)->setTargetPosition(Resources::WINDOW_WIDTH / 2 + (Resources::CARD_WIDTH * 3) - Resources::CARD_WIDTH / 2, Resources::WINDOW_HEIGHT / 2 - (Resources::CARD_HEIGHT * 2) - Resources::CARD_HEIGHT / 2);
        buttons.at(4)->setTargetPosition(Resources::WINDOW_WIDTH / 2 - (Resources::CARD_WIDTH * 3) - Resources::CARD_WIDTH / 2, Resources::WINDOW_HEIGHT / 2 + (Resources::CARD_HEIGHT * 2) - Resources::CARD_HEIGHT / 2);
        buttons.at(5)->setTargetPosition(Resources::WINDOW_WIDTH / 2 + (Resources::CARD_WIDTH * 3) - Resources::CARD_WIDTH / 2, Resources::WINDOW_HEIGHT / 2 + (Resources::CARD_HEIGHT * 2) - Resources::CARD_HEIGHT / 2);
    }
    

    return startClicked;
}

int StartMenu::getDifficultyButtonClicked(int x, int y)
{
    //Lowest difficulty -1 to get index of button for that difficulty
    for (int b = UnoGame::LOWEST_DIFFICULTY - 1; b < UnoGame::NUM_COLORS; b++)
    {
        if (buttons.at(b)->getBounds().contains(x, y))
        {
            //convert index back to "difficulty" setting
            return b + 1;
        }
    }
    return 0;
}

void StartMenu::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(30, 30, 30));
    if (!startClicked)
    {
        window.draw(*buttons.at(UnoGame::NUM_COLORS));
    }
    else
    {
        for (int i = 2; i < UnoGame::NUM_COLORS; i++)
        {
            window.draw(*buttons.at(i));
        }
        window.draw(titleText);
        window.draw(authorText);
    }

    //window.draw(numColorsText);

    window.display();
}

std::unique_ptr<State> StartMenu::getNextState()
{
    return std::move(nextState);
}

bool StartMenu::hasNextState()
{
    return difficulty;
}

int StartMenu::requestingClose()
{
    return closeRequest;
}

void StartMenu::pollEvents(sf::RenderWindow& window)
{
    while (window.pollEvent(ev))
    {
        switch (ev.type)
        {
        case sf::Event::MouseButtonPressed:
            mouseClickPos = sf::Mouse::getPosition(window);
            std::cout << "[MOUSE] mouseClickPos.x : " << mouseClickPos.x << " mouseClickPos.y : " << mouseClickPos.y << std::endl;
            break;
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (ev.key.code == sf::Keyboard::Escape)
                closeRequest = 1;
            break;
        }
    }
}

void StartMenu::update(sf::Time dt)
{
    if(!init)
        setStartingPosition();

    for (int i = 0; i < buttons.size(); i++)
    {
        buttons.at(i)->updatePosition(dt);
    }

    if(!difficulty)
        difficulty = getDifficultyButtonClicked(mouseClickPos.x, mouseClickPos.y);
    else
    {
        nextState = std::make_unique<GameEngine>(startMenuVideoMode, difficulty);
        closeRequest = 0;
        init = 0;
    }
        

    if(startButtonClicked(mouseClickPos.x, mouseClickPos.y))
        mouseClickPos = sf::Vector2i(-500, -500);

    titleText.setPosition(Resources::WINDOW_WIDTH / 2 - titleText.getGlobalBounds().width / 2, Resources::WINDOW_HEIGHT / 2 - titleText.getGlobalBounds().height / 2);
    authorText.setPosition(Resources::WINDOW_WIDTH / 2 - authorText.getGlobalBounds().width / 2, Resources::WINDOW_HEIGHT - authorText.getGlobalBounds().height * 2);
}

std::vector<std::unique_ptr<UnoCard>>& StartMenu::getButtons()
{
    return buttons;
}