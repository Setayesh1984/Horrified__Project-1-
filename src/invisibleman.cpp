// invisibleman.cpp
// Implementation of the InvisibleMan monster and its special logic for the Horrified game.

#include "invisibleman.hpp"
#include "game.hpp"
#include "algorithm"
#include <iostream>
#include "string"
#include <map>
#include <queue>
#include <set>
#include "game.hpp"     // Include Game to access game elements
#include "villager.hpp" // Include Villager header for complete type definition

// Constructor: Initializes the Invisible Man and sets up required evidence
InvisibleMan::InvisibleMan(Location *startLocation)
    : Monster("Invisible Man", startLocation), evidencesFound(0) {
    // Initialize the map with required evidence types (all set to not found)
    requiredEvidence["Footprint"] = false;
    requiredEvidence["Glove"] = false;
    requiredEvidence["Photo"] = false;
    requiredEvidence["Note"] = false;
    requiredEvidence["Bottle"] = false;
}

// Returns the number of evidence pieces found
int InvisibleMan::getEvidenceCount() const
{
    return evidencesFound;
}

// The special action for the Invisible Man during the Monster Phase
// Handles attacking villagers or moving toward the nearest villager
void InvisibleMan::performAction(Game *game)
{
    std::cout << "Invisible Man's power (Stalk Unseen) is activated!" << std::endl;

    // If there are villagers at the current location, attack one
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
        // If no villagers, move up to two spaces toward the nearest villager
        std::cout << "No villagers to attack. Invisible Man moves towards nearest villager." << std::endl;
        for (int i = 0; i < 2; ++i)
        {
            // Breadth-first search (BFS) to find the nearest location with a villager
            Location* start = location;
            std::queue<Location*> q;
            std::map<Location*, Location*> parentMap;
            std::set<Location*> visited;
            q.push(start);
            visited.insert(start);
            parentMap[start] = nullptr;
            Location* targetFound = nullptr;
            Location* firstStep = nullptr;
            while (!q.empty()) {
                Location* current = q.front();
                q.pop();
                // If this location has a villager, we've found our target
                if (!current->getVillagers().empty()) {
                    targetFound = current;
                    break;
                }
                // Add neighbors to the BFS queue
                for (auto neighbor : current->getNeighbors()) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        q.push(neighbor);
                        parentMap[neighbor] = current;
                    }
                }
            }
            // Backtrack to find the first step toward the target
            if (targetFound) {
                Location* currentPathLoc = targetFound;
                while (parentMap[currentPathLoc] != start && parentMap[currentPathLoc] != nullptr) {
                    currentPathLoc = parentMap[currentPathLoc];
                }
                firstStep = currentPathLoc;
            }
            // Move to the next step if a target was found
            if (firstStep) {
                moveTo(firstStep);
                std::cout << "Invisible Man moved to " << firstStep->getName() << std::endl;
                // If we reach a villager, stop moving
                if (!location->getVillagers().empty()) {
                    std::cout << "Invisible Man reached a villager and stopped." << std::endl;
                    break;
                }
            } else {
                // No villagers found anywhere on the board
                std::cout << "Invisible Man could not find a villager to move towards." << std::endl;
                break;
            }
        }
    }
}

// Checks if the Invisible Man can be defeated
// Requires all evidence to be collected and at least 9 power in red items
bool InvisibleMan::canBeDefeated(const std::vector<Item> &items) const
{
    // Check if all evidence has been collected
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

    // Sum the power of all red items
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

// Mark evidence as found for a given location
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

// Returns the name of this monster
std::string InvisibleMan::getName() const
{
    return "Invisible Man";
}

// Handles the Invisible Man attacking a hero (not used for Stalk Unseen)
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

// Returns the required evidence map (for UI or logic)
const std::map<std::string, bool>& InvisibleMan::getRequiredEvidence() const {
    return requiredEvidence;
}

void InvisibleMan::markEvidenceFound(const std::string& name) {
    if (requiredEvidence.find(name) != requiredEvidence.end()) {
        requiredEvidence[name] = true;
    }
}
