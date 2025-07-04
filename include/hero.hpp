
#ifndef HERO_HPP
#define HERO_HPP

#include <string>
#include <vector>
#include "item.hpp"
#include "perkcard.hpp"

enum class HeroType
{
    Mayor,
    Archaeologist
};

class Location;

class Hero
{
private:
    std::string playerName;
    HeroType type;
    int maxActions;
    int remainingActions;
    std::vector<Item> inventory;
    std::vector<Item> items;
    Location *currentLocation;
    std::string name;
    std::vector<PerkCard> perkCards;

    std ::string getRole() const;
    int health;
    bool alive;
    bool inHospital = false;

public:
    Hero(const std::string &playerName, HeroType type, Location *startLocation);
    int getHealth() const { return health; }
    int getActions() const { return remainingActions; }
    const std::vector<Item> &getItems() const { return inventory; };
    Location *getCurrentLocation() const { return currentLocation; }
    std::string getPlayerName() const;
    HeroType getType() const;
    int getMaxActions() const;
    int getRemainingActions() const;
    Location *getLocation() const;
    bool isAlive() const;
    void takeDamage(int amount);

    void takeAllItems();

    void resetActions();
    void removeItem();

    void decrementAction(); // kam kardane action baad az anjam kar
    void moveTo(Location *location);
    void addItem(const Item &item);
    void useItem(const std::string &itemName);
    const std::vector<Item> &getInventory() const;

    const std::vector<PerkCard> &getPerkCards() const;

    bool hasSpecialAction() const;
    void useSpecialAction();
    std::string getName() const;

    void removeItemsByIndexes(const std::vector<int> &indexes);
    void addPerkCard(const PerkCard &card);
    void usePerkCard(int index);
    void removePerkCard(int index);
    void addActions(int count);

    inline std::string heroTypeToString(HeroType type)
    {
        switch (type)
        {
        case HeroType::Mayor:
            return "Mayor";
        case HeroType::Archaeologist:
            return "Archalogist";
        default:
            return "anonymous";
        }
    }

    std::vector<Item> &getItemsRef() { return items; }

    void setInHospital(bool value) { inHospital = value; }
    bool isInHospital() const { return inHospital; }
};

#endif
