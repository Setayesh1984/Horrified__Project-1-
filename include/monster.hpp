#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <string>
#include "location.hpp"

class Game;

class Monster
{
protected:
    std::string name;
    Location *currentLocation;
    Location *location;
    bool defeated;

public:
    Monster(const std::string &name, Location *startLocation);
    virtual ~Monster() = default;

    virtual int getFrenzyOrder() const = 0;

    bool attack(Game *game, const std::string &starResult, const std::string &exclamationResult);
    Location *getCurrentLocation() const { return currentLocation; }

    Location *getLocation() const;
    void moveTo(Location *location);

    bool isDefeated() const;
    void markDefeated();

    virtual void performAction(Game *game) = 0;

    void defeat();
    void move(int steps);
    void moveTowardTarget();
    virtual std::string getName() const = 0;
    virtual bool canBeDefeated(const std::vector<Item> &items) const = 0;
    void takeAction(Game *game);
    void respawn();

protected:
    Location *findNearestTargetLocation(); // Helper for BFS
};

#endif
