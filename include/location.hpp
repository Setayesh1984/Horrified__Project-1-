
#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <memory>   //smart pointer
#include "item.hpp"

class Hero;        // forward declaration
class Monster;
class Villager;

class Location 
{
private:
    std::string name;
    std::vector<Item> items;
    std::vector<Hero*> heroes;
    std::vector<Monster*> monsters;
    std::vector<Villager*> villagers;
    std::vector<Location*> neighbors;    //list makan baraye harekat khooneha

public:
    Location(const std::string& name);

   


    std::string getName() const;

    void addItem(const Item& item);
    void removeItem(const std::string& itemName);

    Item removeItem(int index);
    const std::vector<Item>& getItems() const;
    void clearItems();

    void addHero(Hero* hero);
    void removeHero(Hero* hero);
    const std::vector<Hero*>& getHeroes() const;

    void addMonster(Monster* monster);
    void removeMonster(Monster* monster);
    const std::vector<Monster*>& getMonsters() const;

    void addVillager(Villager* villager);  
    void removeVillager(Villager* villager);
    const std::vector<Villager*>& getVillagers() const;

    void addNeighbor(Location* location);
    const std::vector<Location*>& getNeighbors() const;  //motasel kardan khanehha baraye harekat

    void printItems() const;


    std::vector<Villager*>& getVillagers();      //get villager list
Villager* removeVillager(int index);      

};

#endif