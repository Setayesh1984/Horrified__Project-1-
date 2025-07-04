#include "hero.hpp"
#include "location.hpp"
#include "villager.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

Hero::Hero(const std::string &playerName, HeroType type, Location *startLocation)
    : playerName(playerName), type(type), currentLocation(startLocation), health(3), alive(true), inHospital(false)
{
    if (type == HeroType::Archaeologist)
    {
        maxActions = 4;
    }
    else
    {
        maxActions = 5;
    }
    remainingActions = maxActions;
}

std::string Hero::getPlayerName() const
{
    return playerName;
}

std::string Hero::getName() const
{
    return name;
}

HeroType Hero::getType() const
{
    return type;
}

int Hero::getMaxActions() const
{
    return maxActions;
}

int Hero::getRemainingActions() const
{
    return remainingActions;
}

Location *Hero::getLocation() const
{
    return currentLocation;
}

void Hero::resetActions()
{
    remainingActions = maxActions;
}

void Hero::decrementAction()
{
    if (remainingActions > 0)
        remainingActions--;
}

void Hero::moveTo(Location *location)
{
    if (currentLocation)
    {
        currentLocation->removeHero(this);
    }
    currentLocation = location;
    if (currentLocation)
    {
        currentLocation->addHero(this);
    }

    currentLocation->removeHero(this);
    location->addHero(this);
    currentLocation = location;

    auto villagersCopy = currentLocation->getVillagers();
    for (auto villager : villagersCopy)
    {
        if (villager->getLocation() == villager->getSafeLocation())
        {
           
            addPerkCard(PerkCard("Villager Saved", "You rescued a villager!"));

           
            villager->markSaved();
            currentLocation->removeVillager(villager);

            std::cout << villager->getName() << " be saved!!!\n";
        }
    }
}


void Hero::useSpecialAction()
{
    if (type != HeroType::Archaeologist)
        return;

    for (auto neighbor : currentLocation->getNeighbors())
    {
        auto items = neighbor->getItems();
        for (const auto &item : items)
        {
            addItem(item);
        }
        neighbor->clearItems();
    }
    decrementAction();
}

void Hero::addItem(const Item &item)
{
    inventory.push_back(item);
}

void Hero::takeDamage(int amount)
{
    health -= amount;
    if (health <= 0)
    {
        alive = false;
    }
}

const std::vector<PerkCard> &Hero::getPerkCards() const
{
    return perkCards;
}

void Hero::usePerkCard(int index)
{
    if (index >= 0 && index < perkCards.size())
    {
        perkCards.erase(perkCards.begin() + index);
    }
}

bool Hero::isAlive() const
{
    return alive;
}

void Hero::addActions(int count)
{
    remainingActions += count;
}

void Hero::addPerkCard(const PerkCard &card)
{
    perkCards.push_back(card);
}
void Hero::removePerkCard(int index)
{
    if (index >= 0 && index < perkCards.size())
    {
        perkCards.erase(perkCards.begin() + index);
    }
}

void Hero::takeAllItems()
{
    if (!currentLocation)
    {
        std::cout << "You're not at a valid location.\n";
        return;
    }

    auto &items = currentLocation->getItems();
    if (items.empty())
    {
        std::cout << "No items to take from this location.\n";
        return;
    }

    std::cout << "You picked up all items:\n";
    for (const auto &item : items)
    {
        std::cout << " - " << item.getName() << " (Power: " << item.getPower() << ")\n";
        this->addItem(item);
    }

    currentLocation->clearItems();
}

void Hero::removeItem()
{
    if (!items.empty())
    {
        items.pop_back();
    }
}