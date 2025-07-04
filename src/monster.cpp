#include "monster.hpp"
#include "location.hpp"
#include "villager.hpp"
#include "game.hpp"
#include "dice.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "hero.hpp"
#include <queue>
#include <map>
#include <set>

Monster::Monster(const std::string &name, Location *startLocation)
    : name(name), currentLocation(startLocation), location(startLocation), defeated(false) {}
std::string Monster::getName() const
{
    return name;
}

Location *Monster::getLocation() const
{
    return location;
}

void Monster::moveTo(Location *newLocation)
{
    location = newLocation;
}

bool Monster::isDefeated() const
{
    return defeated;
}

void Monster::markDefeated()
{
    defeated = true;
}

void Monster::defeat()
{
    defeated = true;
}

Location *Monster::findNearestTargetLocation()
{
    if (!location)
        return nullptr;

    std::queue<Location *> q;
    std::map<Location *, Location *> parentMap;
    std::set<Location *> visited;

    q.push(location);
    visited.insert(location);
    parentMap[location] = nullptr;

    Location *targetFound = nullptr;
    Location *firstStep = nullptr;

    while (!q.empty())
    {
        Location *current = q.front();
        q.pop();

        if (!current->getHeroes().empty() || !current->getVillagers().empty())
        {
            targetFound = current;
            break;
        }

        for (auto neighbor : current->getNeighbors())
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                q.push(neighbor);
                parentMap[neighbor] = current;
            }
        }
    }

    if (targetFound)
    {
        Location *currentPathLoc = targetFound;
        while (parentMap[currentPathLoc] != location && parentMap[currentPathLoc] != nullptr)
        {
            currentPathLoc = parentMap[currentPathLoc];
        }
        firstStep = currentPathLoc;
    }

    return firstStep;
}

void Monster::move(int steps)
{
    for (int i = 0; i < steps; ++i)
    {
        Location *nextStep = findNearestTargetLocation();
        if (nextStep)
        {
            moveTo(nextStep);
            std::cout << name << " moved to " << nextStep->getName() << std::endl;

            if (!nextStep->getHeroes().empty() || !nextStep->getVillagers().empty())
            {
                std::cout << name << " reached a person and stopped.\n";
                break;
            }
        }
        else
        {
            std::cout << name << " has nowhere to go!\n";
            break;
        }
    }
}

void Monster::moveTowardTarget()
{

    if (!location->getHeroes().empty() || !location->getVillagers().empty())
    {
        std::cout << name << " It is in a location with a hero or villager and does not move." << std::endl;
        return;
    }

    Location *nextStepLocation = findNearestTargetLocation();

    if (nextStepLocation)
    {

        moveTo(nextStepLocation);
        std::cout << name << " to the place" << nextStepLocation->getName() << " Moved (towards the target)." << std::endl;
    }
    else
    {

        auto neighbors = location->getNeighbors();
        if (!neighbors.empty())
        {
            Location *randomNeighbor = neighbors[rand() % neighbors.size()];
            moveTo(randomNeighbor);
            std::cout << name << " to the place" << randomNeighbor->getName() << " Moved (random)" << std::endl;
        }
        else
        {
            std::cout << name << " not found place to move" << std::endl;
        }
    }
}

void Monster::takeAction(Game *game)
{
    if (!defeated)
    {
        performAction(game);
    }
}

void Monster::respawn()
{
    defeated = false;

    location = currentLocation;
}

bool Monster::attack(Game *game, const std::string &starResult, const std::string &exclamationResult)
{
    bool attacked = false;
    Location *current = this->getLocation();

    if (starResult == "*")
    {

        auto &villagers = current->getVillagers();
        if (!villagers.empty())
        {
            Villager *victim = villagers.back();
            current->removeVillager(victim);
            game->increaseTerrorLevel();
            std::cout << " Villager " << victim->getName() << " was killed by " << this->getName() << "!\n";
            return true;
        }

        auto &heroes = game->getHeroes();
        std::vector<Hero *> heroesHere;
        for (Hero *hero : heroes)
        {
            if (hero && hero->getLocation() == current && hero->isAlive())
            {
                heroesHere.push_back(hero);
            }
        }
        if (!heroesHere.empty())
        {
            Hero *targetHero = heroesHere[0];
            if (heroesHere.size() > 1)
            {
                std::cout << "Multiple heroes are present! Choose who will be attacked:\n";
                for (size_t i = 0; i < heroesHere.size(); ++i)
                {
                    std::cout << (i + 1) << ". " << heroesHere[i]->getPlayerName() << "\n";
                }
                int choice = 1;
                std::cout << "Enter the number of the hero to be attacked: ";
                std::cin >> choice;
                if (choice < 1 || choice > (int)heroesHere.size())
                    choice = 1;
                targetHero = heroesHere[choice - 1];
            }
            std::cout << "⚔️ " << targetHero->getPlayerName() << " is being attacked by " << this->getName() << "!\n";
            std::cout << targetHero->getPlayerName() << ", you are being attacked!\n";
            std::cout << "Do you want to use one of your items to block the attack? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y')
            {
                if (!targetHero->getItems().empty())
                {
                    targetHero->removeItem();
                    std::cout << targetHero->getPlayerName() << " used an item to block the attack!\n";
                    return true;
                }
                else
                {
                    std::cout << "You have no items to use!\n";
                }
            }

            game->increaseTerrorLevel();
            targetHero->setInHospital(true);
            targetHero->moveTo(game->getLocationByName("Hospital"));
            std::cout << targetHero->getPlayerName() << " was injured and sent to the hospital!\n";
            return true;
        }
    }

    if (exclamationResult == "!")
    {
        performAction(game);
        std::cout << name << " used special power with !" << std::endl;
    }
    std::cout << "No one was attacked.\n";
    return false;
}
