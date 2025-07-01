
#include "perkcard.hpp"
#include "game.hpp"
#include "hero.hpp"
#include "monster.hpp"
#include <iostream>
#include <algorithm>
#include <random>

PerkCard::PerkCard(const std::string &name, const std::string &description)
    : name(name), description(description) {}

std::string PerkCard::getName() const
{
    return name;
}

std::string PerkCard::getDescription() const
{
    return description;
}

void PerkCard::activate(Game *game, Hero *hero) const
{
    std::cout << "Cart" << name << " is run: " << description << std::endl;

    if (name == "Visit from the Detective")
    {
        // Place the Invisible Man in a desired location on the game board.
        std::cout << "Choose a location to place the Invisible Man:" << std::endl;
        for (size_t i = 0; i < game->getLocations().size(); ++i)
        {
            std::cout << i + 1 << ". " << game->getLocations()[i]->getName() << std::endl;
        }
        int choice;
        // Input validation
        while (!(std::cin >> choice) || choice < 1 || choice > static_cast<int>(game->getLocations().size()))
        {
            std::cout << "Invalid choice. Please choose a valid location (1-" << game->getLocations().size() << "): ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume the rest of the line

        Location *chosenLocation = game->getLocations()[choice - 1];
        for (auto monster : game->getMonsters())
        {
            if (monster->getName() == "Invisible Man")
            {
                monster->moveTo(chosenLocation);
                std::cout << "Invisible Man moved to " << chosenLocation->getName() << std::endl;
                break;
            }
        }
    }
    else if (name == "Break of Dawn")
    {
        // The next monster phase is skipped. Draw 2 items from the item bag and place them in your location.
        game->setSkipNextMonsterPhase(true); // Set the flag using the setter
        std::cout << "Next monster phase will be skipped." << std::endl;

        // Draw 2 items and place in desired locations
        for (int i = 0; i < 2; ++i)
        {
            if (!game->getItemBag().empty())
            {
                Item drawnItem = game->getItemBag().back();
                
                // Need a non-const getter for itemBag or a method to remove from itemBag
                game->removeItemFromBag(); // Placeholder for a method to remove an item from the bag
               
                std::cout << "Place " << drawnItem.getName() << " (Power: " << drawnItem.getPower() << ") in which location?" << std::endl;
                for (size_t j = 0; j < game->getLocations().size(); ++j)
                {
                    std::cout << j + 1 << ". " << game->getLocations()[j]->getName() << std::endl;
                }
                int locChoice;
                // Input validation
                while (!(std::cin >> locChoice) || locChoice < 1 || locChoice > static_cast<int>(game->getLocations().size()))
                {
                    std::cout << "Invalid choice. Please choose a valid location (1-" << game->getLocations().size() << "): ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Consume the rest of the line

                Location *targetLocation = game->getLocations()[locChoice - 1];
                targetLocation->addItem(drawnItem);
                std::cout << drawnItem.getName() << " placed in " << targetLocation->getName() << std::endl;
            }
            else
            {
                std::cout << "Item bag is empty. Could not draw more items." << std::endl;
                break;
            }
        }
    }
    else if (name == "Overstock")
    {
        // Each hero draws an item from the item bag and places it in its place.
        for (auto h : game->getHeroes())
        {
            if (!game->getItemBag().empty())
            {
                Item drawnItem = game->getItemBag().back();
               
                game->removeItemFromBag(); // Placeholder for a method to remove an item from the bag
                h->getLocation()->addItem(drawnItem);
                std::cout << h->getPlayerName() << " drew " << drawnItem.getName()
                          << " and placed it in " << h->getLocation()->getName() << std::endl;
            }
            else
            {
                std::cout << "Item bag is empty. Could not draw item for " << h->getPlayerName() << std::endl;
            }
        }
    }
    else if (name == "Late into the Night")
    {
        // The player whose turn it is will have two additional Actions.
        hero->addActions(2);
        std::cout << hero->getPlayerName() << " gained 2 additional actions." << std::endl;
    }
    else if (name == "Repel")
    {
        // Move each monster 2 spaces
        for (auto monster : game->getMonsters())
        {
            monster->move(2);
        }
        std::cout << "All monsters moved 2 spaces." << std::endl;
    }
    else if (name == "Hurry")
    {
        // Move each hero 2 spaces
        for (auto h : game->getHeroes())
        {
            // Simplified: move to a random neighbor twice. Needs more sophisticated logic if paths matter.
            for (int i = 0; i < 2; ++i)
            {
                auto neighbors = h->getLocation()->getNeighbors();
                if (!neighbors.empty())
                {
                    std::random_device rd;
                    std::mt19937 g(rd());
                    std::uniform_int_distribution<> distrib(0, neighbors.size() - 1);
                    Location *nextLoc = neighbors[distrib(g)];
                    h->moveTo(nextLoc);
                    std::cout << h->getPlayerName() << " moved to " << nextLoc->getName() << std::endl;
                }
                else
                {
                    std::cout << h->getPlayerName() << " has no neighbors to move to." << std::endl;
                    break; // Can't move further if no neighbors
                }
            }
        }
        std::cout << "All heroes moved 2 spaces." << std::endl;
    }
}