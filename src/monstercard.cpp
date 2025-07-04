#include "monstercard.hpp"
#include "game.hpp"
#include "location.hpp"
#include "monster.hpp"
#include "villager.hpp"
#include <iostream>
#include <tuple>


MonsterCard::MonsterCard(const std::string &name, int itemsToRemove, int moves, int diceRolls,
                         const std::vector<std::tuple<std::string, std::string, std::string>> &villagersToSummon,
                         const std::vector<std::string> &monsterMovementOrder)
    : name(name), itemsToRemove(itemsToRemove), moves(moves), diceRolls(diceRolls),
      villagersToSummon(villagersToSummon), monsterMovementOrder(monsterMovementOrder) {}


std::string MonsterCard::getName() const { return name; }
int MonsterCard::getItemsToRemove() const { return itemsToRemove; }
int MonsterCard::getMoves() const { return moves; }
int MonsterCard::getDiceRolls() const { return diceRolls; }

const std::vector<std::tuple<std::string, std::string, std::string>> &MonsterCard::getVillagersToSummon() const
{
    return villagersToSummon;
}

const std::vector<std::string> &MonsterCard::getMonsterMovementOrder() const
{
    return monsterMovementOrder;
}

// Effects of the card
void MonsterCard::applyEffects(Game *game)
{
    for (const auto &instr : villagersToSummon)
    {
        const std::string &event = std::get<0>(instr);
        const std::string &startLocName = std::get<1>(instr);
        const std::string &destLocName = std::get<2>(instr);

        if (event == "DraculaToHero")
        {
            Hero *currentHero = game->getCurrentHero();
            Monster *dracula = game->getMonsterByName("Dracula");
            if (dracula && currentHero)
            {
                dracula->moveTo(currentHero->getLocation());
                std::cout << "🦇 Dracula moved to " << currentHero->getLocation()->getName() << "\n";
            }
        }
        else if (event == "DraculaToCrypt")
        {
            Monster *dracula = game->getMonsterByName("Dracula");
            Location *crypt = game->getLocationByName("Crypt");
            if (dracula && crypt)
            {
                dracula->moveTo(crypt);
                std::cout << "Dracula moved to the Crypt.\n";
            }
        }
        else if (event == "InvisibleToMaxItems")
        {
            Monster *invisibleMan = game->getMonsterByName("Invisible Man");
            Location *best = nullptr;
            int maxItems = 0;
            for (auto loc : game->getLocations())
            {
                int count = loc->getItems().size();
                if (count > maxItems)
                {
                    maxItems = count;
                    best = loc;
                }
            }
            if (invisibleMan && best)
            {
                invisibleMan->moveTo(best);
                best->clearItems();
                std::cout << "Invisible Man stole all items from " << best->getName() << "\n";
            }
        }
        else
        {
            // Villager spawn event
            Location *startLoc = game->getLocationByName(startLocName);
            Location *destLoc = game->getLocationByName(destLocName);
            if (startLoc && destLoc)
            {
                Villager *villager = new Villager(event, startLoc, destLoc);
                startLoc->addVillager(villager);
                game->addVillager(villager);
                std::cout << "Villager " << event << " spawned at " << startLocName
                          << " (Destination: " << destLocName << ")\n";
            }
        }
    }

    // Remove items from the bag as specified by the card
    for (int i = 0; i < itemsToRemove; ++i)
    {
        game->drawItem();
    }

    // If a monster is at a location with a villager, kill the villager and increase terror
    for (auto monster : game->getMonsters())
    {
        auto &villagers = monster->getLocation()->getVillagers();
        if (!villagers.empty())
        {
            villagers[0]->markDead();
            game->increaseTerror();
            std::cout << "Villager " << villagers[0]->getName() << " was killed by " << monster->getName() << "!\n";
        }
    }
}
