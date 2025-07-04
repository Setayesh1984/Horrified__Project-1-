#include "villager.hpp"


Villager::Villager(const std::string &name, Location *start, Location *destination)
    : name(name), currentLocation(start), safeDestination(destination), isAlive(true), isSaved(false) {}

std::string Villager::getName() const
{
    return name;
}

Location *Villager::getLocation() const
{
    return currentLocation;
}

Location *Villager::getDestination() const
{
    return safeDestination;
}

void Villager::moveTo(Location *location)
{
    currentLocation = location;
}

bool Villager::getIsAlive() const
{
    return isAlive;
}

bool Villager::getIsSaved() const
{
    return isSaved;
}

void Villager::markSaved()
{
    isSaved = true;
}

void Villager::markDead()
{
    isAlive = false;
}

Location *Villager::getSafeLocation() const
{
    return safeLocation;
}
