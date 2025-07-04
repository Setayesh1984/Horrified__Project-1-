

#include "invisibleman.hpp"
#include "game.hpp"
#include "algorithm"
#include <iostream>
#include "string"
#include <map>
#include <queue>
#include <set>
#include "game.hpp"
#include "villager.hpp"

InvisibleMan::InvisibleMan(Location *startLocation)
    : Monster("Invisible Man", startLocation), evidencesFound(0)
{

    requiredEvidence["Footprint"] = false;
    requiredEvidence["Glove"] = false;
    requiredEvidence["Photo"] = false;
    requiredEvidence["Note"] = false;
    requiredEvidence["Bottle"] = false;
}

int InvisibleMan::getEvidenceCount() const
{
    return evidencesFound;
}

void InvisibleMan::performAction(Game *game)
{
    std::cout << "Invisible Man's power (Stalk Unseen) is activated!" << std::endl;

    if (!location->getVillagers().empty())
    {
        Villager *targetVillager = location->getVillagers()[0];
        std::cout << "Invisible Man attacks villager " << targetVillager->getName()
                  << " in " << location->getName() << std::endl;
        location->removeVillager(targetVillager);
        delete targetVillager;
    }
    else
    {

        std::cout << "No villagers to attack. Invisible Man moves towards nearest villager." << std::endl;
        for (int i = 0; i < 2; ++i)
        {

            Location *start = location;
            std::queue<Location *> q;
            std::map<Location *, Location *> parentMap;
            std::set<Location *> visited;
            q.push(start);
            visited.insert(start);
            parentMap[start] = nullptr;
            Location *targetFound = nullptr;
            Location *firstStep = nullptr;
            while (!q.empty())
            {
                Location *current = q.front();
                q.pop();

                if (!current->getVillagers().empty())
                {
                    targetFound = current;
                    break;
                }
                // Add neighbors to the BFS queue
                for (auto neighbor : current->getNeighbors())
                {
                    if (visited.find(neighbor) == visited.end())
                    {
                        visited.insert(neighbor);
                        q.push(neighbor);
                        parentMap[neighbor] = current;
                    }
                }
            }

            if (targetFound)
            {
                Location *currentPathLoc = targetFound;
                while (parentMap[currentPathLoc] != start && parentMap[currentPathLoc] != nullptr)
                {
                    currentPathLoc = parentMap[currentPathLoc];
                }
                firstStep = currentPathLoc;
            }

            if (firstStep)
            {
                moveTo(firstStep);
                std::cout << "Invisible Man moved to " << firstStep->getName() << std::endl;

                if (!location->getVillagers().empty())
                {
                    std::cout << "Invisible Man reached a villager and stopped." << std::endl;
                    break;
                }
            }
            else
            {

                std::cout << "Invisible Man could not find a villager to move towards." << std::endl;
                break;
            }
        }
    }
}

bool InvisibleMan::canBeDefeated(const std::vector<Item> &items) const
{

    bool allEvidenceFound = std::all_of(
        evidencesCollected.begin(),
        evidencesCollected.end(),
        [](bool found)
        { return found; });

    if (!allEvidenceFound)
    {
        std::cout << " First, all 5 pieces of evidence must be collected!\n";
        return false;
    }

    int totalPower = 0;
    for (const auto &item : items)
    {
        if (item.getColor() == ItemColor::Red)
        {
            totalPower += item.getPower();
        }
    }

    if (totalPower >= 9)
    {
        std::cout << " The conditions for the Invisible Man's defeat have been created!\n";
        return true;
    }

    std::cout << "The power of red Item is not adequate....You need 9 " << totalPower << ")\n";
    return false;
}

void InvisibleMan::findEvidence(const std::string &location)
{
    auto it = std::find(evidenceLocations.begin(), evidenceLocations.end(), location);
    if (it != evidenceLocations.end())
    {
        int index = it - evidenceLocations.begin();
        evidencesCollected[index] = true;
        std::cout << "Evidance" << location << " Found! (Documents collected: "
                  << std::count(evidencesCollected.begin(), evidencesCollected.end(), true)
                  << "/5)\n";
    }
}

std::string InvisibleMan::getName() const
{
    return "Invisible Man";
}

void InvisibleMan::attack(Game *game)
{
    Location *loc = this->getLocation();
    Hero *target = game->getHeroAtLocation(loc);

    if (!target)
    {
        std::cout << "invisibleman tried to attack but no hero was found.\n";
        return;
    }

    std::cout << "invisibleman attacks " << target->getName() << "!\n";
    target->takeDamage(1);
}

const std::map<std::string, bool> &InvisibleMan::getRequiredEvidence() const
{
    return requiredEvidence;
}

void InvisibleMan::markEvidenceFound(const std::string &name)
{
    if (requiredEvidence.find(name) != requiredEvidence.end())
    {
        requiredEvidence[name] = true;
    }
}
