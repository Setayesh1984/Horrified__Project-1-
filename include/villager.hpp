
#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include <string>
#include "location.hpp"

class Villager
{
private:
    std::string name;
    Location *currentLocation;
    Location *safeDestination;
    Location *safeLocation;

    bool isAlive;
    bool isSaved;

public:
    Villager(const std::string &name, Location *start, Location *destination);
    Location *getSafeLocation() const;

    void markSaved();

    std::string getName() const;
    Location *getLocation() const;
    Location *getDestination() const;

    void moveTo(Location *location);
    bool getIsAlive() const;
    bool getIsSaved() const;

    void markDead();
};

#endif