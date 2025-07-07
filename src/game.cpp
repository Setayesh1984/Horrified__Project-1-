#include "game.hpp"
#include "dracula.hpp"
#include "invisibleman.hpp"
#include "villager.hpp"
#include "perkcard.hpp"
#include "monstercard.hpp"
#include "monster.hpp"
#include "hero.hpp"
#include "item.hpp"
#include "dice.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <map>
#include <climits>
#include <queue>
#include <set>

const std::vector<Hero *> &Game::getHeroes() const
{
    return heroes;
}

const std::vector<Location *> &Game::getLocations() const
{
    return locations;
}

const std::vector<Monster *> &Game::getMonsters() const
{
    return monsters;
}

const std::vector<Item> &Game::getItemBag() const
{
    return itemBag;
}

void Game::removeItemFromBag()
{
    if (!itemBag.empty())
    {
        itemBag.pop_back();
    }
}

void displayGameStatus(Game &game, Hero *hero1, Hero *hero2);

void Game::setSkipNextMonsterPhase(bool skip)
{
    skipNextMonsterPhase = skip;
}

Game::Game() : currentTurn(1),
               terrorLevel(0),
               defeatedMonsters(0),
               frenzyMonster(nullptr),
               starDice1(DiceType::STAR),
               starDice2(DiceType::STAR),
               exclamationDice(DiceType::EXCLAMATION)
{
    initialize();
}

Game::~Game()
{
    for (auto hero : heroes)
        delete hero;
    for (auto monster : monsters)
        delete monster;
    for (auto location : locations)
        delete location;
}

void Game::start()
{
    std::cout << "\nWelcome to the Horror Game!" << std::endl;
    int currentHeroIndex = 0;

    while (!isGameWon() && !isGameLost())
    {
        std::cout << "\n=== Turn " << currentTurn << " ===\n";
        displayGameStatus(*this, heroes[0], heroes[1]);

        Hero *currentHero = heroes[currentHeroIndex];
        if (currentHero->isAlive())
        {
            heroPhase(currentHero);
        }

        else
        {
            std::cout << currentHero->getName() << " is defeated and skips turn.\n";
        }

        monsterPhase();

        currentHeroIndex = (currentHeroIndex + 1) % heroes.size();

        if (currentHeroIndex == 0)
        {
            currentTurn++;
        }
    }

    showFinalStatus();
}

void Game::initialize()
{
    loadItems();
    loadLocations();
    loadMonsters();
    loadMonsterCards();
    initializeFrenzySystem(); // Initialize the frenzy system

    // Initialize Perk Cards
    for (int i = 0; i < 3; ++i)
    {
        perkDeck.push_back(PerkCard("Visit from the Detective", "Place the Invisible Man in a desired location on the game board."));
    }
    for (int i = 0; i < 3; ++i)
    {
        perkDeck.push_back(PerkCard("Break of Dawn", "The next monster phase is skipped. Draw 2 items from the item bag and place them in your location."));
    }
    for (int i = 0; i < 4; ++i)
    {
        perkDeck.push_back(PerkCard("Overstock", "Each player draws one item from the item bag and places it in their location."));
    }
    for (int i = 0; i < 4; ++i)
    {
        perkDeck.push_back(PerkCard("Late into the Night", "The player whose turn it is will have two additional Actions."));
    }
    for (int i = 0; i < 3; ++i)
    {
        perkDeck.push_back(PerkCard("Repel", "Move each monster 2 spaces. (You can use any available paths for monster movement, and there is no difference between paths)"));
    }
    for (int i = 0; i < 3; ++i)
    {
        perkDeck.push_back(PerkCard("Hurry", "Move each hero 2 spaces. (You can use any available paths for hero movement, and there is no difference between paths)"));
    }

    // Shuffle the perk deck
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(perkDeck.begin(), perkDeck.end(), g);

    assignHeroes();
}

void Game::loadItems()
{
    items.clear();   // Clear existing items on the map
    itemBag.clear(); // Clear existing items in the bag

    std::vector<Item> allItems;

    // Yellow Items
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Flower", ItemColor::Yellow, 2, "Docks"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Tarot Deck", ItemColor::Yellow, 3, "Camp"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Garlic", ItemColor::Yellow, 2, "INN"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("mirrored Box", ItemColor::Yellow, 3, "Mansion"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Stake", ItemColor::Yellow, 3, "Abbey"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Scroll of Thoth", ItemColor::Yellow, 4, "Museum"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Violin", ItemColor::Yellow, 3, "Camp"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Tablet", ItemColor::Yellow, 3, "Museum"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Wolfsbane", ItemColor::Yellow, 4, "Camp"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Charm", ItemColor::Yellow, 4, "Camp"));

    // Blue Items
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Anatomy Text", ItemColor::Blue, 1, "Institute"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Contrifuge", ItemColor::Blue, 1, "Laboratory"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Kite", ItemColor::Blue, 1, "Tower"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Research", ItemColor::Blue, 2, "Tower"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Telescope", ItemColor::Blue, 2, "Mansion"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Searchlight", ItemColor::Blue, 2, "Precinct"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Experiment", ItemColor::Blue, 2, "Laboratory"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Analysis", ItemColor::Blue, 2, "Institute"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Rotenone", ItemColor::Blue, 3, "Institute"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Cosmic Ray Diffuset", ItemColor::Blue, 3, "Tower"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Nebularium", ItemColor::Blue, 3, "Tower"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Monocane Mixture", ItemColor::Blue, 3, "INN"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Fossil", ItemColor::Blue, 3, "Camp"));

    // Red Items
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Dart", ItemColor::Red, 2, "INN"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Fire Poker", ItemColor::Red, 3, "Mansion"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Rapier", ItemColor::Red, 5, "Theatre"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Shovel", ItemColor::Red, 2, "Graveyard"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Torch", ItemColor::Red, 5, "Barn"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Pitchfork", ItemColor::Red, 4, "Barn"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Rifle", ItemColor::Red, 6, "Barn"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Silver Cane", ItemColor::Red, 6, "Shop"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Knife", ItemColor::Red, 3, "Docks"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Pistol", ItemColor::Red, 6, "Precinct"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Bear Trap", ItemColor::Red, 4, "Shop"));
    for (int i = 0; i < 2; ++i)
        allItems.push_back(Item("Speargun", ItemColor::Red, 4, "Institute"));

    if (allItems.size() != 70)
    {
        std::cout << "Error: Incorrect number of items loaded! Expected 70, got " << allItems.size() << std::endl;
        return;
    }

    // Shuffle all items
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allItems.begin(), allItems.end(), g);

    // Place 12 random items on the map
    int itemsToPlace = std::min(12, (int)allItems.size());
    for (int i = 0; i < itemsToPlace; ++i)
    {
        items.push_back(allItems[i]);
    }

    // Move remaining items to itemBag
    for (size_t i = itemsToPlace; i < allItems.size(); ++i)
    {
        itemBag.push_back(allItems[i]);
    }

    // Add items to locations (only for the 12 chosen for the map)
    for (const auto &item : items)
    {
        for (auto location : locations)
        {
            if (location->getName() == item.getLocation())
            {
                location->addItem(item);
                break;
            }
        }
    }
}

void Game::loadLocations()
{

    for (auto location : locations)
    {
        delete location;
    }
    locations.clear();

    // Create all locations from the new map
    Location *abbey = new Location("Abbey");
    Location *inn = new Location("INN");
    Location *cave = new Location("Cave");
    Location *camp = new Location("Camp");
    Location *mansion = new Location("Mansion");
    Location *museum = new Location("Museum");
    Location *precinct = new Location("Precinct");
    Location *crypt = new Location("Crypt");
    Location *theatre = new Location("Theatre");
    Location *farm = new Location("Farm");
    Location *tower = new Location("Tower");
    Location *dungeon = new Location("Dungeon");
    Location *docks = new Location("Docks");
    Location *shop = new Location("Shop");
    Location *laboratory = new Location("Laboratory");
    Location *institute = new Location("Institute");
    Location *church = new Location("Church");
    Location *hospital = new Location("Hospital");
    Location *graveyard = new Location("Graveyard");

    cave->addNeighbor(camp);

    camp->addNeighbor(cave);
    camp->addNeighbor(mansion);
    camp->addNeighbor(theatre);
    camp->addNeighbor(inn);

    mansion->addNeighbor(abbey);
    mansion->addNeighbor(museum);
    mansion->addNeighbor(church);
    mansion->addNeighbor(precinct);
    mansion->addNeighbor(shop);
    mansion->addNeighbor(theatre);
    mansion->addNeighbor(inn);
    mansion->addNeighbor(camp);

    museum->addNeighbor(mansion);
    museum->addNeighbor(shop);
    museum->addNeighbor(church);

    precinct->addNeighbor(mansion);
    precinct->addNeighbor(inn);
    precinct->addNeighbor(theatre);

    crypt->addNeighbor(abbey);

    inn->addNeighbor(mansion);
    inn->addNeighbor(precinct);
    inn->addNeighbor(theatre);
    inn->addNeighbor(camp);

    theatre->addNeighbor(inn);
    theatre->addNeighbor(farm);
    theatre->addNeighbor(tower);
    theatre->addNeighbor(shop);
    theatre->addNeighbor(mansion);
    theatre->addNeighbor(precinct);
    theatre->addNeighbor(camp);

    farm->addNeighbor(theatre);

    tower->addNeighbor(theatre);
    tower->addNeighbor(dungeon);
    tower->addNeighbor(docks);

    dungeon->addNeighbor(tower);

    docks->addNeighbor(tower);

    shop->addNeighbor(theatre);
    shop->addNeighbor(mansion);
    shop->addNeighbor(laboratory);
    shop->addNeighbor(museum);
    shop->addNeighbor(church);

    laboratory->addNeighbor(shop);
    laboratory->addNeighbor(institute);

    institute->addNeighbor(laboratory);

    church->addNeighbor(mansion);
    church->addNeighbor(hospital);
    church->addNeighbor(graveyard);
    church->addNeighbor(shop);
    church->addNeighbor(museum);

    hospital->addNeighbor(church);

    graveyard->addNeighbor(church);

    abbey->addNeighbor(mansion);
    abbey->addNeighbor(crypt);

    // Add locations to the game
    locations.push_back(abbey);
    locations.push_back(inn);
    locations.push_back(cave);
    locations.push_back(camp);
    locations.push_back(mansion);
    locations.push_back(museum);
    locations.push_back(precinct);
    locations.push_back(crypt);
    locations.push_back(theatre);
    locations.push_back(farm);
    locations.push_back(tower);
    locations.push_back(dungeon);
    locations.push_back(docks);
    locations.push_back(shop);
    locations.push_back(laboratory);
    locations.push_back(institute);
    locations.push_back(church);
    locations.push_back(hospital);
    locations.push_back(graveyard);

    // Add items to locations
    for (const auto &item : items)
    {
        for (auto location : locations)
        {
            if (location->getName() == item.getLocation())
            {
                location->addItem(item);
                break;
            }
        }
    }
}

void Game::loadMonsters()
{

    for (auto location : locations)
    {
        if (location->getName() == "Mansion")
        {
            monsters.push_back(new Dracula(location));
        }

        else if (location->getName() == "Theatre")
        {
            monsters.push_back(new InvisibleMan(location));
        }
    }
    frenzyMonster = monsters[0];
}

void Game::rewardHeroWithPerk(Hero *hero, Villager *villager)
{
    villager->markSaved();
    villager->getLocation()->removeVillager(villager);
    std::cout << "the villager saved and remove";

    if (!perkDeck.empty())
    {
        hero->addPerkCard(perkDeck.back());
        std::cout << hero->getPlayerName() << " received a perk card: "
                  << perkDeck.back().getName() << "!\n";
        perkDeck.pop_back();
    }
}

void Game::assignHeroes()
{
    std::string playerName1, playerName2;
    long long garlicTime1, garlicTime2;
    int heroType1, heroType2;

    std::cout << "\nPlayer 1, enter your name: ";
    std::cin >> playerName1;
    std::cout << playerName1 << ", when was the last time you ate garlic? (Enter a number representing days ago, e.g., 100 for 100 days ago): ";
    while (!(std::cin >> garlicTime1) || garlicTime1 < 0)
    {
        std::cout << "Invalid input. Please enter a non-negative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "\nPlayer 2, enter your name: ";
    std::cin >> playerName2;
    std::cout << playerName2 << ", when was the last time you ate garlic? (Enter a number representing days ago, e.g., 100 for 100 days ago): ";
    while (!(std::cin >> garlicTime2) || garlicTime2 < 0)
    {
        std::cout << "Invalid input. Please enter a non-negative number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Determine who chooses first based on garlic eating time (more recently chooses first)
    std::string firstPlayerName, secondPlayerName;
    int *firstHeroTypePtr;
    int *secondHeroTypePtr;

    if (garlicTime1 < garlicTime2)
    { // Player 1 ate garlic more recently
        firstPlayerName = playerName1;
        secondPlayerName = playerName2;
        firstHeroTypePtr = &heroType1;
        secondHeroTypePtr = &heroType2;
        std::cout << "\n"
                  << playerName1 << " ate garlic more recently. " << playerName1 << " chooses character first." << std::endl;
    }

    else if (garlicTime2 < garlicTime1)
    { // Player 2 ate garlic more recently
        firstPlayerName = playerName2;
        secondPlayerName = playerName1;
        firstHeroTypePtr = &heroType2;
        secondHeroTypePtr = &heroType1;
        std::cout << "\n"
                  << playerName2 << " ate garlic more recently. " << playerName2 << " chooses character first." << std::endl;
    }

    else
    { // Times are equal, player 1 chooses first by default
        firstPlayerName = playerName1;
        secondPlayerName = playerName2;
        firstHeroTypePtr = &heroType1;
        secondHeroTypePtr = &heroType2;
        std::cout << "\nBoth players ate garlic at the same time. " << playerName1 << " chooses character first." << std::endl;
    }

    // First player chooses character
    std::cout << "\n"
              << firstPlayerName << ", choose your character type (1: Archaeologist, 2: Mayor): ";
    while (!(std::cin >> *firstHeroTypePtr) || (*firstHeroTypePtr != 1 && *firstHeroTypePtr != 2))
    {
        std::cout << "Invalid choice. Please enter 1 for Archaeologist or 2 for Mayor: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Second player chooses character (cannot be the same type as first player)
    do
    {
        std::cout << "\n"
                  << secondPlayerName << ", choose your character type (1: Archaeologist, 2: Mayor): ";
        while (!(std::cin >> *secondHeroTypePtr) || (*secondHeroTypePtr != 1 && *secondHeroTypePtr != 2))
        {
            std::cout << "Invalid choice. Please enter 1 for Archaeologist or 2 for Mayor: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (*secondHeroTypePtr == *firstHeroTypePtr)
        {
            std::cout << "This character type is already chosen. Please choose a different one." << std::endl;
        }
    } while (*secondHeroTypePtr == *firstHeroTypePtr);

    // Create heroes and place them at a random location
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, locations.size() - 1);
    Location *startingLocation = locations[distrib(gen)];

    if (startingLocation)
    {
        Location *Docks = getLocationByName("Docks");
        Location *Theatre = getLocationByName("Theatre");

        Hero *hero1 = nullptr;
        Hero *hero2 = nullptr;

        if (heroType1 == 1)
        {
            hero1 = new Hero(playerName1, HeroType::Archaeologist, Docks);
            hero2 = new Hero(playerName2, HeroType::Mayor, Theatre);
        }
        else
        {
            hero2 = new Hero(playerName2, HeroType::Archaeologist, Docks);
            hero1 = new Hero(playerName1, HeroType::Mayor, Theatre);
        }
        heroes.push_back(hero1);
        heroes.push_back(hero2);
        Docks->addHero(hero1->getType() == HeroType::Archaeologist ? hero1 : hero2);
        Theatre->addHero(hero1->getType() == HeroType::Mayor ? hero1 : hero2);
        // Give a starting perk card to each hero from the deck
        if (!perkDeck.empty())
        {
            hero1->addPerkCard(perkDeck.back());
            perkDeck.pop_back();
        }
        if (!perkDeck.empty())
        {
            hero2->addPerkCard(perkDeck.back());
            perkDeck.pop_back();
        }
    }
}

void Game::heroPhase(Hero *hero)
{
    int actions = 4;

    if (hero->isInHospital())
    {
        hero->moveTo(this->getLocationByName("Hospital"));
        hero->setInHospital(false);
        std::cout << hero->getPlayerName() << " starts from the hospital in the next turn.\n";
    }

    std::cout << "\n===== " << hero->getPlayerName() << "'s Turn =====\n";
    std::cout << "\nCurrent Location: " << hero->getLocation()->getName() << std::endl;

    while (actions > 0)
    {
        std::cout << "\nYou have " << actions << " actions remaining." << std::endl;
        std::cout << "1. Move to a neighboring location" << std::endl;
        std::cout << "2. Show items in current location" << std::endl;
        std::cout << "3. Take an item" << std::endl;
        std::cout << "4. Guide a villager" << std::endl;
        std::cout << "5. Use a perk card" << std::endl;
        std::cout << "6. help" << std::endl;
        std::cout << "7. Advance Task" << std::endl;
        std::cout << "8. Defeat Monster" << std::endl;
        std::cout << "9. quit" << std::endl;
        std::cout << "10. Specal Action" << std::endl;
        std::cout << "please choose 1-10" << std::endl;

        std::string input;

        std::cin >> input;

        if (input == "quit")
        {
            std::cout << "Game is over...Bye" << std::endl;
            exit(0);
        }

        if (input == "help")
        {
            std::cout << "\n Available actions:\n";
            std::cout << " - move\n";
            std::cout << " - guide\n";
            std::cout << " - pickup\n";
            std::cout << " - advance\n";
            std::cout << " - defeat\n";
            std::cout << " - perk\n";
            std::cout << " - help\n";
            std::cout << " - quit\n";
            std::cout << " - specail\n";

            std::string commandHelp;
            while (true)
            {
                std::cout << "\nEnter the name of an action to see its description (or type 'quit' to return): ";
                std::getline(std::cin >> std::ws, commandHelp);

                if (commandHelp == "quit")
                    break;

                if (commandHelp == "move")
                {
                    std::cout << "move: Moves the hero to a neighboring location (1 action is subtracted)\n";
                }

                else if (commandHelp == "guide")
                {
                    std::cout << " guide: Leads a villager with him to a neighboring location (if have any villagers)\n";
                }

                else if (commandHelp == "pickup")
                {
                    std::cout << " pickup: Take items from the current location.\n";
                }

                else if (commandHelp == "advance")
                {
                    std::cout << " advance: Advancing the Monster Mission(Destroy Coffin or Find Evidence)\n";
                }

                else if (commandHelp == "defeat")
                {
                    std::cout << " defeat: Final attack on the monster (if the conditions for its defeat are met)\n";
                }

                else if (commandHelp == "perk")
                {
                    std::cout << " perk: Using one of the Perk cards available to you\n";
                }

                else if (commandHelp == "help")
                {
                    std::cout << " help: Display a list of actions and descriptions for each.\n";
                }

                else if (commandHelp == "specail")
                {
                    std::cout << "Specail: this option just for Archaeologist \n that mean Take as many items as he want in current location.\n";
                }

                else
                {
                    std::cout << " Unknown command. Please enter one of the actions or quit.\n";
                }
            }

            continue;
        }

        int choice;
        try
        {
            choice = std::stoi(input);
        }
        catch (...)
        {
            std::cout << "Invalid choice.please choose 1-10\n";

            continue;
        }
        if (choice < 1 || choice > 10)
        {
            std::cout << "Invalid choice. Please choose 1-10:   \n ";
            continue;

            std::cout << "Invalid choice. Please choose 1-10: ";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            handleMove(hero, actions);
            break;

        case 2:
        { // Show items
            hero->getLocation()->printItems();
            break;
        }

        case 3:
    pickUpItems(hero, actions);
    break;

    case 4:
    guideVillager(hero, actions);
    break;

        case 5:
        { // Use a perk card
            auto &perkCards = hero->getPerkCards();
            if (perkCards.empty())
            {
                std::cout << "You have no perk cards!" << std::endl;
            }
            else
            {
                std::cout << "\nYour perk cards:" << std::endl;
                for (size_t i = 0; i < perkCards.size(); ++i)
                {
                    std::cout << i + 1 << ". " << perkCards[i].getName()
                              << " - " << perkCards[i].getDescription() << std::endl;
                }

                int cardChoice;
                std::cout << "Choose card to use (1-" << perkCards.size() << "): ";
                while (!(std::cin >> cardChoice) || cardChoice < 1 || cardChoice > static_cast<int>(perkCards.size()))
                {
                    std::cout << "Invalid card choice. Please try again: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                perkCards[cardChoice - 1].activate(this, hero);

                std::cout << "Used " << perkCards[cardChoice - 1].getName() << "\n";
                hero->removePerkCard(cardChoice - 1);
            }
            break;
        }

            std::cout << "\n=== INTERACTIVE HELP MENU ===\n";
            std::cout << "Available actions: 1-10\n";
            std::cout << "Enter a number (1-10) to see detailed explanation, or 0 to return to game:\n";
            int helpChoice;
            while (true)
            {
                std::cout << "Enter choice (0-10): ";
                std::cin >> helpChoice;
                if (helpChoice == 0)
                    break;
                break;
            }

        case 6:
        { // Help
            std::cout << "\n=== INTERACTIVE HELP MENU ===\n";
            std::cout << "Available actions: 1-10\n";
            std::cout << "Enter a number (1-10) to see detailed explanation, or 0 to return to game:\n";

            int helpChoice;
            while (true)
            {
                std::cout << "Enter choice (0-10): ";
                std::cin >> helpChoice;

                if (helpChoice == 0)
                    break;

                switch (helpChoice)
                {
                case 1:
                    std::cout << "\n=== MOVE ACTION ===\n";
                    std::cout << "Cost: 1 action\n";
                    std::cout << "Description: Move your hero to an adjacent location on the map.\n";
                    std::cout << "Usage: You can move to any location connected by a line on the map.\n";
                    std::cout << "Strategy: Use this to explore the map, reach items, or escape monsters.\n";
                    break;
                case 2:
                    std::cout << "\n=== SHOW ITEMS ===\n";
                    std::cout << "Cost: 0 actions (free)\n";
                    std::cout << "Description: Display all items available in your current location.\n";
                    std::cout << "Usage: Shows item names and their power levels.\n";
                    std::cout << "Strategy: Always check what items are available before taking them.\n";
                    break;
                case 3:
                    std::cout << "\n=== TAKE ITEM ===\n";
                    std::cout << "Cost: 1 action\n";
                    std::cout << "Description: Pick up one or more items from your current location.\n";
                    std::cout << "Usage: Select items by number to add them to your inventory.\n";
                    std::cout << "Strategy: Collect items needed for defeating monsters or advancing tasks.\n";
                    break;
                case 4:
                    std::cout << "\n=== GUIDE VILLAGER ===\n";
                    std::cout << "Cost: 1 action\n";
                    std::cout << "Description: Guide a villager to safety or move them between locations.\n";
                    std::cout << "Usage: Help villagers reach their destination to earn perk cards.\n";
                    std::cout << "Strategy: Rescuing villagers provides valuable perk cards for later use.\n";
                    break;
                case 5:
                    std::cout << "\n=== USE PERK CARD ===\n";
                    std::cout << "Cost: 1 action\n";
                    std::cout << "Description: Use one of your perk cards for special abilities.\n";
                    std::cout << "Usage: Select a perk card from your inventory to activate it.\n";
                    std::cout << "Strategy: Perk cards can provide powerful advantages - use them wisely.\n";
                    break;
                case 6:
                    std::cout << "\n=== HELP ===\n";
                    std::cout << "Cost: 0 actions (free)\n";
                    std::cout << "Description: Show this interactive help menu.\n";
                    std::cout << "Usage: Get detailed explanations of all game actions.\n";
                    std::cout << "Strategy: Use this whenever you need to understand game mechanics.\n";
                    break;
                case 7:
                    std::cout << "\n=== ADVANCE TASK ===\n";
                    std::cout << "Cost: 1 action\n";
                    std::cout << "Description: Progress in defeating monsters by destroying coffins or dropping evidence.\n";
                    std::cout << "Usage: For Dracula - destroy coffins with appropriate items.\n";
                    std::cout << "       For Invisible Man - drop evidence at the Precinct.\n";
                    std::cout << "Strategy: This is essential for winning the game.\n";
                    break;
                case 8:
                    std::cout << "\n=== DEFEAT MONSTER ===\n";
                    std::cout << "Cost: 1 action\n";
                    std::cout << "Description: Attempt to defeat a monster at your location.\n";
                    std::cout << "Usage: Roll dice and meet conditions to defeat monsters.\n";
                    std::cout << "Strategy: Only attempt when you have the right items and conditions.\n";
                    break;
                case 9:
                    std::cout << "\n=== QUIT ===\n";
                    std::cout << "Cost: 0 actions (free)\n";
                    std::cout << "Description: Exit the game.\n";
                    std::cout << "Usage: Confirm your choice to quit the game.\n";
                    std::cout << "Strategy: Use this when you want to end your current game session.\n";
                    break;
                case 10:
                    std::cout << "\n=== SPECIAL ACTION ===\n";
                    std::cout << "Cost: 1 action\n";
                    std::cout << "Description: Archaeologist special ability - take all items from current location.\n";
                    std::cout << "Usage: Only available to Archaeologist character type.\n";
                    std::cout << "Strategy: Very powerful for collecting multiple items efficiently.\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please enter 0-10.\n";
                    break;
                }
                std::cout << "\n";
            }
            break;
        }

        case 7:
        { // Advance Task
            advanceTask(hero);
            break;
        }

        case 8:
        { // Defeat Monster
            defeatMonster(hero);
            break;
        }

        case 9:
        { // Quit
            std::cout << "Are you sure you want to quit? (y/n): ";
            char confirm;
            std::cin >> confirm;
            if (tolower(confirm) == 'y')
            {
                std::cout << "Thanks for playing!\n";
                exit(0);
            }
            break;
        }

        case 10:
        { // Special Action
            if (hero->getType() == HeroType::Archaeologist)
            {
                auto *currentLocation = hero->getLocation();
                const auto &neighbors = currentLocation->getNeighbors();
                if (neighbors.empty())
                {
                    std::cout << "No adjacent locations to pick items from." << std::endl;
                    break;
                }
                std::cout << "\nAdjacent locations:" << std::endl;
                for (size_t i = 0; i < neighbors.size(); ++i)
                {
                    std::cout << i + 1 << ". " << neighbors[i]->getName() << std::endl;
                }
                std::cout << "Choose a location to pick up items from (1-" << neighbors.size() << "): ";
                int locChoice = 0;
                while (!(std::cin >> locChoice) || locChoice < 1 || locChoice > static_cast<int>(neighbors.size()))
                {
                    std::cout << "Invalid choice. Please choose a valid location (1-" << neighbors.size() << "): ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                Location *chosenLocation = neighbors[locChoice - 1];
                const auto &locItems = chosenLocation->getItems();
                if (locItems.empty())
                {
                    std::cout << "No items available in this location." << std::endl;
                }
                else
                {
                    std::cout << "\nAvailable items in " << chosenLocation->getName() << ":" << std::endl;
                    for (size_t i = 0; i < locItems.size(); ++i)
                    {
                        std::cout << i + 1 << ". " << locItems[i].getName()
                                  << " (Power: " << locItems[i].getPower() << ")" << std::endl;
                    }
                    std::cout << "Choose items to take (enter numbers separated by spaces, e.g., '1 3'): ";
                    std::string lineInput;
                    std::getline(std::cin >> std::ws, lineInput);
                    std::stringstream ss(lineInput);
                    int itemNum;
                    std::vector<int> chosenIndexes;
                    while (ss >> itemNum)
                    {
                        if (itemNum >= 1 && itemNum <= static_cast<int>(locItems.size()))
                        {
                            chosenIndexes.push_back(itemNum - 1);
                        }
                        else
                        {
                            std::cout << "Invalid item number ignored: " << itemNum << std::endl;
                        }
                    }
                    std::sort(chosenIndexes.begin(), chosenIndexes.end());
                    chosenIndexes.erase(std::unique(chosenIndexes.begin(), chosenIndexes.end()), chosenIndexes.end());
                    if (chosenIndexes.empty())
                    {
                        std::cout << "No valid items selected." << std::endl;
                    }
                    else
                    {

                        for (int i = chosenIndexes.size() - 1; i >= 0; --i)
                        {
                            int indexToRemove = chosenIndexes[i];
                            if (indexToRemove >= 0 && indexToRemove < (int)chosenLocation->getItems().size())
                            {
                                Item picked = chosenLocation->removeItem(indexToRemove);
                                hero->addItem(picked);
                                std::cout << "Picked up " << picked.getName() << "\n";
                            }
                            else
                            {
                                std::cout << "Item index " << (indexToRemove + 1) << " is no longer valid and was skipped.\n";
                            }
                        }
                        actions--;
                    }
                }
            }
            else
            {
                std::cout << "This special action is only available for the Archaeologist.\n";
            }
            break;
        }

        default:
            std::cout << "Invalid choice. Please choose 1-10" << std::endl;
            break;
        }
    }
}

void Game::loadMonsterCards()
{
    // Gray cards
    monsterDeck.push_back(MonsterCard("The Delivery", 3, 1, 3, {{std::make_tuple("Wilbur & Chick", "Docks", "Barn")}}, {"Dracula"}));
    monsterDeck.push_back(MonsterCard("Fortune Teller", 3, 1, 2, {{std::make_tuple("Maleva", "Camp", "Institute")}}, {"Dracula"}));
    monsterDeck.push_back(MonsterCard("Former Employer", 3, 1, 2, {{std::make_tuple("Dr. Cranly", "Laboratory", "Precinct")}}, {"Invisible Man", "Dracula"}));
    monsterDeck.push_back(MonsterCard("The Innocent", 3, 1, 3, {{std::make_tuple("Maria", "Barn", "Inn")}}, {"Dracula", "Invisible Man"}));
    monsterDeck.push_back(MonsterCard("Hurried Assistant", 3, 2, 3, {{std::make_tuple("Fritz", "Tower", "Laboratory")}}, {"Dracula"}));
    monsterDeck.push_back(MonsterCard("Egyptian Expert", 3, 2, 2, {{std::make_tuple("Prof.Pearson", "Cave", "Museum")}}, {"Dracula", "Invisible Man"}));
    monsterDeck.push_back(MonsterCard("The Ichthyologist", 3, 1, 2, {{std::make_tuple("Dr.Read", "Institute", "Hospital")}}, {"Invisible Man"}));

    // Colored cards
    for (int i = 0; i < 3; ++i)
    {
        monsterDeck.push_back(MonsterCard("Form of The Bat", 2, 1, 2, {{std::make_tuple("DraculaToHero", "", "")}}, {"Invisible Man"}));
    }
    for (int i = 0; i < 3; ++i)
    {
        monsterDeck.push_back(MonsterCard("Sunrise", 0, 1, 2, {{std::make_tuple("DraculaToCrypt", "", "")}}, {"Invisible Man", "Dracula"}));
    }
    for (int i = 0; i < 5; ++i)
    {
        monsterDeck.push_back(MonsterCard("Thief", 2, 1, 3, {{std::make_tuple("InvisibleToMaxItems", "", "")}}, {"Invisible Man", "Dracula"}));
    }

    // New Bonus Cards
    for (int i = 0; i < 2; ++i)
    {
        monsterDeck.push_back(MonsterCard("Hypnotic Gaze", 2, 1, 2, {{std::make_tuple("NearestToDracula", "", "")}}, {"Invisible Man"}));
    }
    for (int i = 0; i < 2; ++i)
    {
        monsterDeck.push_back(MonsterCard("On The Move", 3, 3, 2, {{std::make_tuple("VillagersToSafety", "", "")}}, {"Frenzied Monster"}));
    }

    std::shuffle(monsterDeck.begin(), monsterDeck.end(), std::mt19937{std::random_device{}()});
}

void Game::initializeFrenzySystem()
{

    frenzyMonster = getMonsterByName("Dracula");
    if (frenzyMonster)
    {
        std::cout << "🔥 " << frenzyMonster->getName() << " starts with the Frenzy Marker!\n";
    }
}

void Game::passFrenzyMarker()
{

    if (frenzyMonster && frenzyMonster->getName() == "Dracula")
    {
        frenzyMonster = getMonsterByName("Invisible Man");
    }
    else if (frenzyMonster && frenzyMonster->getName() == "Invisible Man")
    {
        frenzyMonster = getMonsterByName("Dracula");
    }

    if (frenzyMonster)
    {
        std::cout << "🔥 Frenzy Marker passed to " << frenzyMonster->getName() << "!\n";
    }
}

bool Game::isGameOver() const
{
    // Game is won if all monsters are defeated
    for (auto hero : heroes)
    {
        if (!hero && hero->isAlive())
        {
            return true;
        }
    }

    return (terrorLevel >= 5);
}

bool Game::isGameWon()
{
    bool draculaDefeated = false;
    bool invisibleManDefeated = false;

    for (auto monster : monsters)
    {
        if (monster->getName() == "Dracula" && monster->isDefeated())
            draculaDefeated = true;
        if (monster->getName() == "Invisible Man" && monster->isDefeated())
            invisibleManDefeated = true;
    }

    if (draculaDefeated && invisibleManDefeated)
    {
        std::cout << "ALL MONSTERS DEFEATED! YOU WIN!\n";

        return true;
    }
    return false;
}

bool Game::isGameLost() const
{
    bool allHeroesDead = true;
    for (const auto &hero : heroes)
    {
        if (hero && hero->isAlive())
        {
            allHeroesDead = false;
            break;
        }
    }

    return (terrorLevel >= 5);
}

Monster *Game::findMonster(const std::string &name)
{
    for (auto monster : monsters)
    {
        if (monster->getName() == name)
        {
            return monster;
        }
    }
    return nullptr;
}

Monster *Game::getMonsterByName(const std::string &name)
{
    for (auto m : monsters)
    {
        if (m->getName() == name)
        {
            return m;
        }
    }
    return nullptr;
}

Location *Game::getLocationByName(const std::string &name)
{
    for (auto loc : locations)
    {
        if (loc->getName() == name)
        {
            return loc;
        }
    }
    return nullptr;
}

Hero *Game::getCurrentHero()
{
    return heroes[currentHeroIndex];
}

void Game::handleMove(Hero *hero, int &actions)
{
    Location *current = hero->getLocation();
    const auto &neighbors = current->getNeighbors();

    std::cout << "\nAvailable destinations:" << std::endl;
    for (size_t i = 0; i < neighbors.size(); ++i)
    {
        std::cout << i + 1 << ". " << neighbors[i]->getName() << std::endl;
    }

    int destChoice;
    std::cout << "Choose destination (1-" << neighbors.size() << "): ";
    while (!(std::cin >> destChoice) || destChoice < 1 || destChoice > static_cast<int>(neighbors.size()))
    {
        std::cout << "Invalid destination choice. Please try again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    Location *chosenLocation = neighbors[destChoice - 1];

    if (false)
    {
        std::cout << "You cannot stay in " << chosenLocation->getName() << ". Please choose another action." << std::endl;
    }
    else
    {
        hero->moveTo(chosenLocation);
        std::cout << "Moved to " << chosenLocation->getName() << "\n";
        actions--;
    }
}



void Game::pickUpItems(Hero* hero, int& actions) {
    const auto& locItems = hero->getLocation()->getItems();

    if (locItems.empty()) {
        std::cout << "No items available in this location." << std::endl;
        return;
    }

    std::cout << "\nAvailable items:" << std::endl;
    for (size_t i = 0; i < locItems.size(); ++i) {
        std::cout << i + 1 << ". " << locItems[i].getName()
                  << " (Power: " << locItems[i].getPower() << ")" << std::endl;
    }

    std::cout << "Choose items to take (enter numbers separated by spaces, e.g., '1 3'): ";
    std::string lineInput;
    std::getline(std::cin >> std::ws, lineInput);

    std::stringstream ss(lineInput);
    int itemNum;
    std::vector<int> chosenIndexes;

    while (ss >> itemNum) {
        if (itemNum >= 1 && itemNum <= static_cast<int>(locItems.size())) {
            chosenIndexes.push_back(itemNum - 1);
        } else {
            std::cout << "Invalid item number ignored: " << itemNum << std::endl;
        }
    }

    std::sort(chosenIndexes.begin(), chosenIndexes.end());
    chosenIndexes.erase(std::unique(chosenIndexes.begin(), chosenIndexes.end()), chosenIndexes.end());

    if (chosenIndexes.empty()) {
        std::cout << "No valid items selected." << std::endl;
        return;
    }

    // Pick up items in reverse order to avoid invalidating iterators
    for (int i = chosenIndexes.size() - 1; i >= 0; --i) {
        int indexToRemove = chosenIndexes[i];
        Item picked = hero->getLocation()->removeItem(indexToRemove);
        hero->addItem(picked);
        std::cout << "Picked up " << picked.getName() << "\n";
        actions--;
    }
}



void Game::guideVillager(Hero* hero, int& actions) {
    Location* current = hero->getLocation();

    auto& localVillagers = current->getVillagers();

    bool didSomething = false;

    // 1. حرکت از current به neighbor
    if (!localVillagers.empty()) {
        std::cout << "\n👥 Villagers in current location:\n";
        for (size_t i = 0; i < localVillagers.size(); ++i) {
            std::cout << i + 1 << ". " << localVillagers[i]->getName()
                      << " (Destination: " << localVillagers[i]->getDestination()->getName() << ")\n";
        }

        int villagerChoice;
        std::cout << "Choose a villager to guide (0 to skip): ";
        std::cin >> villagerChoice;

        if (villagerChoice >= 1 && villagerChoice <= static_cast<int>(localVillagers.size())) {
            Villager* villager = current->removeVillager(villagerChoice - 1);

            const auto& neighbors = current->getNeighbors();
            std::cout << "Choose destination:\n";
            for (size_t i = 0; i < neighbors.size(); ++i) {
                std::cout << i + 1 << ". " << neighbors[i]->getName() << '\n';
            }

            int destChoice;
            std::cin >> destChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (destChoice >= 1 && destChoice <= static_cast<int>(neighbors.size())) {
                Location* dest = neighbors[destChoice - 1];
                dest->addVillager(villager);
                villager->moveTo(dest);

                std::cout << "✅ Guided " << villager->getName() << " to " << dest->getName() << '\n';

                if (dest == villager->getDestination()) {
                    std::cout << "🎉 " << villager->getName() << " has reached their destination!\n";
                    rewardHeroWithPerk(hero, villager);
                }

                actions--;
                didSomething = true;
            } else {
                std::cout << "❌ Invalid destination. Villager returned.\n";
                current->addVillager(villager);
                villager->moveTo(current);
            }
        }
    }

    // 2. حرکت از neighbor به current
    const auto& neighbors = current->getNeighbors();
    for (Location* neighbor : neighbors) {
        auto& neighborVillagers = neighbor->getVillagers();
        if (!neighborVillagers.empty()) {
            std::cout << "\n👥 Villagers in neighbor: " << neighbor->getName() << '\n';
            for (size_t i = 0; i < neighborVillagers.size(); ++i) {
                std::cout << i + 1 << ". " << neighborVillagers[i]->getName()
                          << " (Destination: " << neighborVillagers[i]->getDestination()->getName() << ")\n";
            }

            int choice;
            std::cout << "Choose villager to bring from " << neighbor->getName() << " (0 to skip): ";
            std::cin >> choice;

            if (choice >= 1 && choice <= static_cast<int>(neighborVillagers.size())) {
                Villager* villager = neighbor->removeVillager(choice - 1);
                current->addVillager(villager);
                villager->moveTo(current);

                std::cout << "✅ Moved " << villager->getName() << " to your location.\n";

                if (current == villager->getDestination()) {
                    std::cout << "🎉 " << villager->getName() << " has reached their destination!\n";
                    rewardHeroWithPerk(hero, villager);
                }

                actions--;
                didSomething = true;
                break; // فقط یکی بیاد کافیه
            }
        }
    }

    if (!didSomething && localVillagers.empty()) {
        std::cout << "😕 No villagers available to guide.\n";
    }
}




void Game::advanceTask(Hero *hero)
{
    Location *current = hero->getLocation();

    for (Monster *monster : monsters)
    {
        if (monster->getName() == "Dracula")
        {
            Dracula *dracula = dynamic_cast<Dracula *>(monster);
            if (!dracula)
                continue;

            const auto &coffinLocations = dracula->getCoffinLocations();
            for (size_t i = 0; i < coffinLocations.size(); ++i)
            {
                if (coffinLocations[i] == current->getName() && !dracula->isCoffinDestroyed(i))
                {
                    if (dracula->destroyCoffin(hero->getItems(), current->getName()))
                    {
                        std::cout << "The coffin in place" << current->getName() << "is destroying\n";
                        hero->decrementAction();
                        return;
                    }
                    else
                    {
                        std::cout << "You do not have the right item to destroy the coffin.\n";
                        return;
                    }
                }
            }
        }
    }

    for (Monster *monster : monsters)
    {
        if (monster->getName() == "Invisible Man")
        {
            InvisibleMan *im = dynamic_cast<InvisibleMan *>(monster);
            if (!im)
                continue;
            if (current->getName() != "Precinct")
            {
                std::cout << "You must return to the Precinct to drop evidence!\n";
                return;
            }

            auto &items = hero->getItemsRef();
            for (auto it = items.begin(); it != items.end(); ++it)
            {
                if (im->dropEvidence(*it, current->getName()))
                {
                    std::cout << "Evidance" << it->getName() << " in" << current->getName() << " was placed\n";
                    items.erase(it);
                    hero->decrementAction();
                    return;
                }
            }

            std::cout << "You do not have a valid certificate for this location.\n";
            return;
        }
    }

    std::cout << " Nothing can be done to progress the task in this place.\n";
}

void Game::defeatMonster(Hero *hero)
{
    Location *heroLoc = hero->getLocation();

    for (Monster *monster : monsters)
    {
        if (!monster->isDefeated() && monster->getLocation() == heroLoc)
        {

            std::string roll1 = starDice1.roll();
            std::string roll2 = starDice2.roll();
            std::cout << "🎲 Dice Rolls: " << roll1 << " " << roll2 << std::endl;

            if (roll1 != "*" && roll2 != "*")
            {
                std::cout << " No '*' rolled. The attack failed.\n";

                return;
            }

            if (monster->canBeDefeated(hero->getItems()))
            {
                monster->defeat();

                Location *monsterLoc = monster->getLocation();
                if (monsterLoc)
                {
                    monsterLoc->removeMonster(monster);
                }
                std::cout << "✅ " << monster->getName() << " has been defeated!\n";
                defeatedMonsters++;

                if (monster->getName() == "Dracula")
                    draculaDefeated = true;
                if (monster->getName() == "Invisible Man")
                    invisibleManDefeated = true;

                if (frenzyMonster == monster)
                {
                    passFrenzyMarker();
                }

                return;
            }
            else
            {
                std::cout << " Conditions not met to defeat " << monster->getName() << ".\n";
            }

            return;
        }
    }

    std::cout << "No monster to defeat at your location.\n";
}

Hero *Game::getHeroAtLocation(Location *loc)
{
    for (auto hero : heroes)
    {
        if (hero && hero->getLocation() == loc && hero->isAlive())
            return hero;
    }
    return nullptr;
}

void Game::drawItem()
{
    if (!itemBag.empty())
    {
        Item item = itemBag.back();
        itemBag.pop_back();
        int i = rand() % locations.size();
        locations[i]->addItem(item);
        std::cout << "Item dropped at " << locations[i]->getName() << "\n";
    }
}

void Game::monsterPhase()
{
    std::cout << "\n=====  Monster Phase =====\n";

    if (!monsterDeck.empty())
    {
        MonsterCard card = monsterDeck.back();
        monsterDeck.pop_back();
        std::cout << " Monster Card Drawn: " << card.getName() << "\n";
        card.applyEffects(this);
        std::cout << "Monster Card Drawn: " << card.getName() << "\n";
        lastMonsterDiceRolls.clear();

        // Handle special card effects
        if (card.getName() == "Hypnotic Gaze")
        {
            // Move nearest hero or villager to Dracula
            Monster *dracula = getMonsterByName("Dracula");
            if (dracula)
            {
                Location *draculaLocation = dracula->getLocation();
                Hero *nearestHero = nullptr;
                Villager *nearestVillager = nullptr;
                int shortestHeroDistance = INT_MAX;
                int shortestVillagerDistance = INT_MAX;
                // Find nearest hero
                for (auto hero : heroes)
                {
                    if (hero && hero->isAlive())
                    {
                        int distance = calculateDistance(hero->getLocation(), draculaLocation);
                        if (distance < shortestHeroDistance)
                        {
                            shortestHeroDistance = distance;
                            nearestHero = hero;
                        }
                    }
                }
                // Find nearest villager
                for (auto villager : villagers)
                {
                    if (villager && !villager->getIsSaved())
                    {
                        int distance = calculateDistance(villager->getLocation(), draculaLocation);
                        if (distance < shortestVillagerDistance)
                        {
                            shortestVillagerDistance = distance;
                            nearestVillager = villager;
                        }
                    }
                }
                // Decide who to move: prefer hero if equal distance
                if ((nearestHero && shortestHeroDistance <= shortestVillagerDistance) && shortestHeroDistance > 0)
                {
                    Location *currentLocation = nearestHero->getLocation();
                    Location *nextLocation = findNextLocationTowards(currentLocation, draculaLocation);
                    if (nextLocation)
                    {
                        nearestHero->moveTo(nextLocation);
                        std::cout << "The nearest hero, " << nearestHero->getPlayerName() << ", was hypnotically drawn towards Dracula!\n";
                    }
                }
                else if (nearestVillager && shortestVillagerDistance > 0)
                {
                    Location *currentLocation = nearestVillager->getLocation();
                    Location *nextLocation = findNextLocationTowards(currentLocation, draculaLocation);
                    if (nextLocation)
                    {
                        currentLocation->removeVillager(nearestVillager);
                        nextLocation->addVillager(nearestVillager);
                        nearestVillager->moveTo(nextLocation);
                        std::cout << "The nearest villager, " << nearestVillager->getName() << ", was hypnotically drawn towards Dracula!\n";
                    }
                }
            }
        }
        else if (card.getName() == "On The Move")
        {

            passFrenzyMarker();

            for (auto villager : villagers)
            {
                if (villager && !villager->getIsSaved())
                {
                    Location *currentLocation = villager->getLocation();
                    Location *destination = villager->getDestination();
                    if (currentLocation != destination)
                    {
                        Location *nextLocation = findNextLocationTowards(currentLocation, destination);
                        if (nextLocation)
                        {
                            currentLocation->removeVillager(villager);
                            nextLocation->addVillager(villager);
                            villager->moveTo(nextLocation);
                            std::cout << "🏃 " << villager->getName() << " moved closer to safety!\n";
                        }
                    }
                }
            }
        }

        bool attackOccurred = false;
        for (const std::string &monsterName : card.getMonsterMovementOrder())
        {
            Monster *monster = getMonsterByName(monsterName);
            if (!monster || monster->isDefeated())
                continue;

            // Check if this is a frenzy attack
            bool isFrenzyAttack = false;
            if (monsterName == "Frenzied Monster")
            {
                monster = frenzyMonster; // Use the current frenzy monster
                isFrenzyAttack = true;
            }

            if (!monster || monster->isDefeated())
                continue;

            // Move monster only if not already with a person
            bool personAlreadyHere = false;
            for (auto hero : heroes)
            {
                if (hero && hero->getLocation() == monster->getLocation())
                {
                    personAlreadyHere = true;
                    break;
                }
            }
            for (auto villager : monster->getLocation()->getVillagers())
            {
                if (villager)
                {
                    personAlreadyHere = true;
                    break;
                }
            }
            if (!personAlreadyHere)
            {
                monster->move(card.getMoves());
            }
            else
            {
                std::cout << monster->getName() << " is already at a location with a person, does not move.\n";
            }

            for (int i = 0; i < card.getDiceRolls(); ++i)
            {
                std::string starResult = starDice1.roll();
                std::string exResult = exclamationDice.roll();
                std::string diceResult = "" + monster->getName() + " rolled: " + starResult + " " + exResult;
                if (isFrenzyAttack)
                {
                    diceResult += " (Frenzy Attack!)";
                }
                lastMonsterDiceRolls.push_back(diceResult);
                std::cout << monster->getName() << " rolled dice: " << starResult << " " << exResult;
                if (isFrenzyAttack)
                {
                    std::cout << " (Frenzy Attack!)";
                }
                std::cout << std::endl;

                if (starResult == "*")
                {
                    bool attackFinished = monster->attack(this, starResult, exResult);
                    if (attackFinished)
                    {

                        attackOccurred = true;
                        break;
                    }
                }
                else if (exResult == "!")
                {
                    monster->performAction(this);
                }
            }
            if (attackOccurred)
                break;
        }
    }
    else
    {
        std::cout << " Monster deck is empty!\n";
    }

    bool anyMonstersAlive = false;
    for (auto monster : monsters)
    {
        if (!monster->isDefeated())
        {
            anyMonstersAlive = true;
            break;
        }
    }
    if (monsterDeck.empty() && anyMonstersAlive)
    {
        std::cout << "⏰ The monster deck is empty and monsters remain! You lose!\n";
        showFinalStatus();
        exit(0);
    }

    displayGameStatus(*this, heroes[0], heroes[1]);
}

void Game::increaseTerror()
{
    ++terrorLevel;
}

void displayGameStatus(Game &game, Hero *hero1, Hero *hero2)
{
    std::cout << "\n====================\n";
    std::cout << "TERROR LEVEL: ";
    for (int i = 1; i <= 5; ++i)
    {
        std::cout << (i <= game.getTerrorLevel() ? "#" : "-");
    }
    std::cout << " (" << game.getTerrorLevel() << "/5)\n";
    std::cout << "====================\n";

    // Heroes Info
    std::cout << "\nHeroes:\n";
    for (Hero *hero : {hero1, hero2})
    {
        std::cout << "----------------------------------\n";
        std::cout << hero->getPlayerName() << " (" << hero->heroTypeToString(hero->getType()) << ")\n";
        std::cout << "Place: " << hero->getLocation()->getName() << "\n";
        std::cout << "Remaining actions: " << hero->getRemainingActions() << "\n";

        const auto &items = hero->getItems();
        std::cout << "Items(" << items.size() << "): ";
        for (const auto &item : items)
            std::cout << item.getName() << " ";
        std::cout << "\n";

        const auto &perks = hero->getPerkCards();
        std::cout << "Perk cards(" << perks.size() << "): ";
        for (const auto &perk : perks)
            std::cout << perk.getName() << " ";
        std::cout << "\n";
    }

    // Monsters
    std::cout << "\nMonsters status:\n";
    for (Monster *monster : game.getMonsters())
    {
        std::cout << " - " << monster->getName()
                  << " | Location: " << monster->getLocation()->getName()
                  << " | Status: " << (monster->isDefeated() ? "Defeated" : "active") << "\n";
    }

    // Frenzy Monster Status
    Monster *frenzyMonster = game.getFrenzyMonster();
    if (frenzyMonster && !frenzyMonster->isDefeated())
    {
        std::cout << "\nFrenzy Monster: " << frenzyMonster->getName() << " (attacks more frequently!)\n";
    }

    // Dracula Coffins
    Dracula *dracula = dynamic_cast<Dracula *>(game.getMonsterByName("Dracula"));
    if (dracula)
    {
        std::cout << "\nCoffin Status (Dracula):\n";
        const auto &coffins = dracula->getCoffinLocations();
        for (size_t i = 0; i < coffins.size(); ++i)
        {
            std::cout << " - " << coffins[i] << ": "
                      << (dracula->isCoffinDestroyed(i) ? "Destroyed" : "Intact") << "\n";
        }
    }

    // Evidence Status for Invisible Man
    InvisibleMan *im = dynamic_cast<InvisibleMan *>(game.getMonsterByName("Invisible Man"));
    if (im)
    {
        std::cout << "\nEvidence Status (Invisible Man):\n";
        const auto &evidence = im->getRequiredEvidence();
        for (const auto &e : evidence)
        {
            std::cout << " - " << e.first << ": " << (e.second ? "Found" : "Missing") << "\n";
        }
    }

    // Villager Details
    std::cout << "\nVillager Status:\n";
    int rescued = 0, inDanger = 0;
    for (auto v : game.getVillagers())
    {
        if (v->getIsSaved())
            rescued++;
        else
            inDanger++;
    }
    std::cout << " - Rescued: " << rescued << "\n";
    std::cout << " - Still in Danger: " << inDanger << "\n";

    // Show locations + who is there
    std::cout << "\nLocation Occupancy:\n";
    for (auto loc : game.getLocations())
    {
        std::cout << loc->getName() << ": ";

        bool hasSomething = false;

        for (auto hero : game.getHeroes())
        {
            if (hero->getLocation() == loc)
            {
                std::cout << "[Hero " << hero->getPlayerName() << "] ";
                hasSomething = true;
            }
        }

        for (auto m : game.getMonsters())
        {
            if (m->getLocation() == loc && !m->isDefeated())
            {
                std::cout << "[Monster " << m->getName() << "] ";
                hasSomething = true;
            }
        }

        for (auto v : loc->getVillagers())
        {
            std::cout << "[Villager " << v->getName() << "] ";
            hasSomething = true;
        }

        if (!loc->getItems().empty())
        {
            std::cout << "[" << loc->getItems().size() << " item(s)] ";
            hasSomething = true;
        }

        if (!hasSomething)
            std::cout << "(empty)";
        std::cout << "\n";
    }

    const auto &rolls = game.getLastMonsterDiceRolls();
    std::cout << "\nLast Monster Dice Rolls:\n";
    if (rolls.empty())
    {
        std::cout << " - No monster rolled dice yet.\n";
    }
    else
    {
        for (const auto &roll : rolls)
        {
            std::cout << " - " << roll << "\n";
        }
    }

    std::cout << "Items in Bag: " << game.getItemBag().size() << "\n";

    int remainingPerk = game.getPerkDeck().size();
    std::cout << "Perk Cards Remaining in Deck: " << remainingPerk << "\n";

    int heldPerk = hero1->getPerkCards().size() + hero2->getPerkCards().size();
    int usedPerk = 20 - remainingPerk - heldPerk;
    std::cout << "Perk Cards Used: " << usedPerk << "\n";

    std::cout << "===========================================\n";
    std::cout << "            MAP OVERVIEW\n";
    std::cout << "===========================================\n";
    std::cout << "Cave --- Camp  Precinct   Inn     Barn    Dungeon\n";
    std::cout << "         |      |          |       |         |\n";
    std::cout << "         |______|__________|____Theater____Tower___Docks\n";
    std::cout << "         |                         |________________\n";
    std::cout << "         |                                         |\n";
    std::cout << "         |                           ___Graveyard  |\n";
    std::cout << "      Mansion__________Church________|             |\n";
    std::cout << "         |        |                  |___Hospital  |\n";
    std::cout << "         |        |____Museum                      |\n";
    std::cout << "Abbey____|        |                                |\n";
    std::cout << "  |               |                                |\n";
    std::cout << "  Crypt           |                                |\n";
    std::cout << "                  Shop_____________________________|\n";
    std::cout << "Institute____Lab__|\n";
    std::cout << "===========================================\n\n";
}

void Game::showFinalStatus() const
{
    std::cout << "\n=============================\n";
    std::cout << " FINAL GAME STATUS\n";
    std::cout << "=============================\n";
    std::cout << "Terror Level: " << terrorLevel << "/5\n";
    std::cout << "Monster Cards Remaining: " << monsterDeck.size() << "\n";
    std::cout << "Monster Status:\n";
    for (Monster *monster : monsters)
    {
        std::cout << " - " << monster->getName()
                  << " | Location: " << monster->getLocation()->getName()
                  << " | Status: " << (monster->isDefeated() ? "Defeated" : "active") << "\n";
    }
}

int Game::calculateDistance(Location *from, Location *to)
{
    if (!from || !to || from == to)
        return 0;

    std::set<Location *> visited;
    std::queue<std::pair<Location *, int>> q;
    q.push({from, 0});
    visited.insert(from);

    while (!q.empty())
    {
        auto [current, distance] = q.front();
        q.pop();

        if (current == to)
        {
            return distance;
        }

        for (auto neighbor : current->getNeighbors())
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                q.push({neighbor, distance + 1});
            }
        }
    }

    return INT_MAX;
}

Location *Game::findNextLocationTowards(Location *from, Location *to)
{
    if (!from || !to || from == to)
        return nullptr;

    std::set<Location *> visited;
    std::queue<std::pair<Location *, Location *>> q;
    q.push({from, nullptr});
    visited.insert(from);

    while (!q.empty())
    {
        auto [current, previous] = q.front();
        q.pop();

        if (current == to)
        {

            while (previous != from)
            {
                current = previous;

                for (auto neighbor : from->getNeighbors())
                {
                    if (calculateDistance(neighbor, to) == calculateDistance(from, to) - 1)
                    {
                        previous = from;
                        from = neighbor;
                        break;
                    }
                }
            }
            return current;
        }

        for (auto neighbor : current->getNeighbors())
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                q.push({neighbor, current});
            }
        }
    }

    return nullptr;
}
