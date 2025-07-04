
#include "dracula.hpp"
#include <iostream>
#include "game.hpp"

#include "vector"
#include "algorithm"


Dracula::Dracula(Location* startLocation)
    : Monster("Dracula", startLocation), coffinsRemaining(4) {}


void Dracula::performAction(Game* game) 
{
    std::cout << "Dracula's power (Dark Charm) is activated!" << std::endl;

    if (location->getHeroes().empty()) 
    {
        // Find the current active hero - placeholder for now, ideally passed or managed by Game
        Hero* activeHero = nullptr;
        if (!game->getHeroes().empty()) { // Use the public getter
            activeHero = game->getHeroes()[0]; 
        }

        if (activeHero && activeHero->getLocation() != this->location) 
        {
            std::cout << "Dracula uses Dark Charm to pull " << activeHero->getPlayerName() 
                      << " to his location: " << this->location->getName() << std::endl;
            activeHero->moveTo(this->location);
        } 

        else if (activeHero && activeHero->getLocation() == this->location) 
        {
             std::cout << activeHero->getPlayerName() << " is already in Dracula's location, Dark Charm has no effect." << std::endl;
        }

    } 
    
    
    else 
    {
        std::cout << "Dracula is already in a location with a hero, Dark Charm has no effect." << std::endl;
    }
}



    bool Dracula::destroyCoffin(const std::vector<Item>& items, const std::string& locationName) 
    {
    
    auto it = std::find(coffinLocations.begin(), coffinLocations.end(), locationName);
    if (it == coffinLocations.end()) {
        std::cout << "Location of " << locationName << "is not coffin of dracula\n";
        return false;
    }
    
    int index = it - coffinLocations.begin();

    int redPower = 0;
    for (const auto& item : items) 
    {
        if (item.getColor() == ItemColor::Red) 
        {
            redPower += item.getPower();
        }
    }
    
  
    if (redPower >= 6 && !coffinsDestroyed[index]) 
    {
        coffinsDestroyed[index] = true;
        remainingCoffins--;
        std::cout << "coffin in" << locationName << "destroying!!!  Rest of other coffin is " 
                  << remainingCoffins << ")\n";
        return true;
    }
    
    if (redPower < 6) 
    {
        std::cout << "The power of red Item is not adequate....You need 6 " << redPower << ")\n";
    } 
    
    else 
    
    {
        std::cout << " The coffin in " << locationName << "befor destroyed \n";
    }

    return false;
}

bool Dracula::canBeDefeated(const std::vector<Item>& items) const 
{
    // چک کردن نابودی همه تابوت‌ها
    bool allDestroyed = std::all_of(coffinsDestroyed.begin(), coffinsDestroyed.end(),[](bool b) { return b; });
     
    if (!allDestroyed) 
    {
        std::cout << "At first you must destroy all 4 coffin \n";
        return false; 
    }
    
    // محاسبه قدرت آیتم‌های زرد
    int yellowPower = 0;
    for (const auto& item : items) 
    {
        if (item.getColor() == ItemColor::Yellow) 
        {
            yellowPower += item.getPower();
        }
    }
    
    if (yellowPower >= 6) 
    {
        std::cout << "The conditions for defeating the monster have been met!!!\n";
        return true;
    }
    
    std::cout << "The power of yellow Item is not adequate....You need 6 " << yellowPower << ")\n";
    return false;
}




std::string Dracula::getName() const 
{
    return "Dracula";
}



void Dracula::attack(Game* game) 
{
    Location* loc = this->getLocation();
    Hero* target = game->getHeroAtLocation(loc);

    if (!target) {
        std::cout << "Dracula tried to attack but no hero was found.\n";
        return;
    }

    std::cout << "Dracula attacks " << target->getName() << "!\n";
    target->takeDamage(1);
}