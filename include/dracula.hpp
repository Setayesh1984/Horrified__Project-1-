#ifndef DRACULA_HPP
#define DRACULA_HPP

#include "monster.hpp"

class Dracula : public Monster 
{
    
private:

    int coffinsRemaining;
    int remainingCoffins = 4;
   
    std::vector<std::string> coffinLocations = {"Cave", "Dungeon", "Crypt", "Graveyard"};
    std::vector<bool> coffinsDestroyed = {false, false, false, false};

public:

    Dracula(Location* startLocation);
    void attack(Game* game);

    void performAction(Game* game) override;
    
    bool destroyCoffin(const std::vector<Item>& items , const std::string& locationName);
    bool canBeDefeated(const std::vector<Item>& items) const override;
  
    std::string getName() const override;
    std::vector<Item> selectedItems;
    std::vector<int> chosenIndexes;
   
const std::vector<std::string>& getCoffinLocations() const { return coffinLocations; }


bool isCoffinDestroyed(size_t index) const { return coffinsDestroyed[index]; }

int getFrenzyOrder() const override { return 1; }


};

#endif
