#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>
#include "hero.hpp"
#include "monster.hpp"
#include "location.hpp"
#include "item.hpp"
#include "perkcard.hpp"
#include "monstercard.hpp"
#include "dice.hpp"

class Game
{

private:
    std::vector<Hero *> heroes;
    std::vector<Monster *> monsters;
    std::vector<Location *> locations;
    std::vector<Item> items;
    std::vector<Item> itemBag;
    std::vector<PerkCard> perkDeck;
    std::vector<MonsterCard> monsterDeck;
    std::vector<Villager *> villagers;
    std::vector<std::string> lastMonsterDiceRolls;

    Dice starDice1;       // First '*' die
    Dice starDice2;       // Second '*' die
    Dice exclamationDice; // '!' die
    int currentHeroIndex = 0;
    int currentTurn;
    int terrorLevel;
    int defeatedMonsters;
    Monster *frenzyMonster;
    bool skipNextMonsterPhase;              // New flag to skip monster phase
    std::vector<std::string> lastDiceRolls; // To store dice results for display
    bool draculaDefeated = false;
    bool invisibleManDefeated = false;

public:
    Game();
    ~Game();

    void initialize();
    void start();
    void drawItem();
    Location *findLocation(const std::string &name);
    Monster *findMonster(const std::string &name);
    Monster *getMonsterByName(const std::string &name);
    Location *getLocationByName(const std::string &name);
    Hero *getCurrentHero();

    Hero *getHeroAtLocation(Location *loc);

    void heroPhase(Hero *hero);
    void monsterPhase();
    void loadMonsterCards();
    bool isGameWon();
    bool isGameOver() const;
    bool isGameLost() const;

    const std::vector<Hero *> &getHeroes() const;
    const std::vector<Location *> &getLocations() const;
    const std::vector<Monster *> &getMonsters() const;
    const std::vector<Item> &getItemBag() const;
    const std::vector<Villager *> &getVillagers() const { return villagers; };
    void addVillager(Villager *villager)
    {
        villagers.push_back(villager);
    }

    void increaseTerrorLevel()
    {
        terrorLevel++;
    }

    void loadItems();
    void loadLocations();
    void loadMonsters();
    void loadVillagers();
    void assignHeroes();
    void removeItemFromBag();
    void showFinalStatus() const;
    void increaseTerror();

    void setSkipNextMonsterPhase(bool skip);
    void defeatMonster(Hero *hero);
    void advanceTask(Hero *hero);


    void handleMove(Hero* hero, int& actions);
    void showItems(Hero* hero);
    void pickUpItems(Hero* hero, int& actions);
    void guideVillager(Hero* hero, int& actions);
    void usePerkCard(Hero* hero);
    void showHelpMenu();
    void handleQuit();
    void handleSpecialAction(Hero* hero, int& actions);








    int getTerrorLevel() const { return terrorLevel; }

    const std::vector<std::string> &getLastMonsterDiceRolls() const
    {
        return lastMonsterDiceRolls;
    }

    void rewardHeroWithPerk(Hero *hero, Villager *villager);

    const std::vector<PerkCard> &getPerkDeck() const { return perkDeck; }

    void initializeFrenzySystem();
    void passFrenzyMarker();
    Monster *getFrenzyMonster() const { return frenzyMonster; }
    void setFrenzyMonster(Monster *monster) { frenzyMonster = monster; }
    bool isMonsterFrenzied(Monster *monster) const { return frenzyMonster == monster; }

    int calculateDistance(Location *from, Location *to);
    Location *findNextLocationTowards(Location *from, Location *to);
};

#endif
