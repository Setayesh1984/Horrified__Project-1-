#include "location.hpp"
#include <algorithm> //remove-if
#include <iostream>
#include "item.hpp"
#include <stdexcept> //out of range

Location::Location(const std::string &name) : name(name) {}

std::string Location::getName() const
{
    return name;
}

void Location::addItem(const Item &item)
{
    items.push_back(item);
}

void Location::removeItem(const std::string &itemName)
{
    items.erase(std::remove_if(items.begin(), items.end(),
                               [&](const Item &i)
                               { return i.getName() == itemName; }),
                items.end());
}

const std::vector<Item> &Location::getItems() const
{
    return items;
}

void Location::addHero(Hero *hero)
{
    heroes.push_back(hero);
}

void Location::removeHero(Hero *hero)
{
    heroes.erase(std::remove(heroes.begin(), heroes.end(), hero), heroes.end());
}

const std::vector<Hero *> &Location::getHeroes() const
{
    return heroes;
}

void Location::addMonster(Monster *monster)
{
    monsters.push_back(monster);
}

void Location::removeMonster(Monster *monster)
{
    monsters.erase(std::remove(monsters.begin(), monsters.end(), monster), monsters.end());
}

const std::vector<Monster *> &Location::getMonsters() const
{
    return monsters;
}

void Location::addVillager(Villager *villager)
{
    villagers.push_back(villager);
}

void Location::removeVillager(Villager *villager)
{
    villagers.erase(std::remove(villagers.begin(), villagers.end(), villager), villagers.end());
}

const std::vector<Villager *> &Location::getVillagers() const
{
    return villagers;
}

void Location::addNeighbor(Location *location)
{
    neighbors.push_back(location);
}

const std::vector<Location *> &Location::getNeighbors() const
{
    return neighbors;
}

void Location::printItems() const
{
    if (items.empty())
    {
        std::cout << "No items in this location\n";
    }
    else
    {
        std::cout << "Available items:\n";
        for (size_t i = 0; i < items.size(); ++i)
        {
            std::cout << i + 1 << ". " << items[i].getName()
                      << " | Power: " << items[i].getPower()
                      << "\n";
        }
    }
}

Item Location::removeItem(int index)
{
    if (index < 0 || index >= items.size())
    {
        throw std::out_of_range("Invalid item index");
    }
    Item pickedItem = items[index];
    items.erase(items.begin() + index);
    return pickedItem;
}

void Location::clearItems()
{
    items.clear();
}

std::vector<Villager *> &Location::getVillagers()
{
    return villagers;
}

Villager *Location::removeVillager(int index)
{
    if (index < 0 || index >= villagers.size())
    {
        throw std::out_of_range("Villager index invalid");
    }
    Villager *v = villagers[index];
    villagers.erase(villagers.begin() + index);
    return v;
}