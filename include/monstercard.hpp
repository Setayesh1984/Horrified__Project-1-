#ifndef MONSTERCARD_HPP
#define MONSTERCARD_HPP

#include <string>
#include <vector>
#include <utility>

class Game;
class Location;
class Monster;
class Villager;

class MonsterCard
{

private:

    std::string name;
    int itemsToRemove;
    int moves;
    int diceRolls;
    bool frenzy;
    bool villagerEffect;

    std::vector<std::tuple<std::string, std::string, std::string>> villagersToSummon;

    std::vector<std::string> monsterMovementOrder;

public:

    MonsterCard(const std::string &name, int itemsToRemove, int moves, int diceRolls,
                const std::vector<std::tuple<std::string, std::string, std::string>> &villagersToSummon,
                const std::vector<std::string> &monsterMovementOrder);

    std::string getName() const;
    int getItemsToRemove() const;
    int getMoves() const;
    int getDiceRolls() const;

    const std::vector<std::tuple<std::string, std::string, std::string>> &getVillagersToSummon() const;
    const std::vector<std::string> &getMonsterMovementOrder() const;

    void applyEffects(Game *game);
    bool isFrenzy() const { return frenzy; }
    bool hasVillagerEffect() const { return villagerEffect; }
};

#endif