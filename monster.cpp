#include "monster.hpp"
#include "location.hpp"
#include "villager.hpp"
#include "game.hpp"
#include "dice.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "hero.hpp"
#include <queue>
#include <map>
#include <set>

Monster::Monster(const std::string& name, Location* startLocation)
    : name(name), currentLocation(startLocation), location(startLocation), defeated(false) {}
std::string Monster::getName() const {
    return name;
}

Location* Monster::getLocation() const {
    return location;
}

void Monster::moveTo(Location* newLocation) {
    location = newLocation;
}

bool Monster::isDefeated() const {
    return defeated;
}

void Monster::markDefeated() {
    defeated = true;
}

void Monster::defeat() {
    defeated = true;
}





Location* Monster::findNearestTargetLocation() {
    if (!location) return nullptr; // Monster has no current location

    std::queue<Location*> q;
    std::map<Location*, Location*> parentMap; // To reconstruct path
    std::set<Location*> visited;

    q.push(location);
    visited.insert(location);
    parentMap[location] = nullptr; // Current location has no parent

    Location* targetFound = nullptr;
    Location* firstStep = nullptr;

    while (!q.empty()) {
        Location* current = q.front();
        q.pop();

        // Check if current location contains a target
        if (!current->getHeroes().empty() || !current->getVillagers().empty()) {
            targetFound = current;
            break; // Found the nearest target
        }

        // Explore neighbors
        for (auto neighbor : current->getNeighbors()) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
                parentMap[neighbor] = current;
            }
        }
    }

    // If a target was found, backtrack to find the first step
    if (targetFound) {
        Location* currentPathLoc = targetFound;
        while (parentMap[currentPathLoc] != location && parentMap[currentPathLoc] != nullptr) {
            currentPathLoc = parentMap[currentPathLoc];
        }
        firstStep = currentPathLoc;
    }

    return firstStep;
}

void Monster::move(int steps) {
    for (int i = 0; i < steps; ++i) {
        Location* nextStep = findNearestTargetLocation();
        if (nextStep) {
            moveTo(nextStep);
            std::cout << name << " moved to " << nextStep->getName() << std::endl;

            // اگر به کسی رسید متوقف شود
            if (!nextStep->getHeroes().empty() || !nextStep->getVillagers().empty()) {
                std::cout << name << " reached a person and stopped.\n";
                break;
            }
        } else {
            std::cout << name << " has nowhere to go!\n";
            break;
        }
    }
}



void Monster::moveTowardTarget() {
    // Rule 2: If the monster's current location already contains a person, it does not move.
    if (!location->getHeroes().empty() || !location->getVillagers().empty()) {
        std::cout << name << " It is in a location with a hero or villager and does not move." << std::endl;
        return;
    }

    Location* nextStepLocation = findNearestTargetLocation();

    if (nextStepLocation) {
        // Rule 1: Whenever the monster reaches a person, it stops.
        // The findNearestTargetLocation returns the next step towards the nearest target.
        // If this next step itself contains a target, then the monster moves to it and stops.
        moveTo(nextStepLocation);
        std::cout << name << " to the place" << nextStepLocation->getName() << " Moved (towards the target)." << std::endl;

        // If the destination contains a hero or villager, it stops there.
        // No further movement is implied within this single moveTowardTarget call.
    } else {
        // Fallback: if no target found, move to a random neighbor
        auto neighbors = location->getNeighbors();
        if (!neighbors.empty()) {
            Location* randomNeighbor = neighbors[rand() % neighbors.size()];
            moveTo(randomNeighbor);
            std::cout << name << " to the place" << randomNeighbor->getName() << " Moved (random)" << std::endl;
        } else {
            std::cout << name << " جایی برای حرکت پیدا نکرد." << std::endl;
        }
    }
}

void Monster::takeAction(Game* game) {
    if (!defeated) {
        performAction(game);
    }
}

void Monster::respawn() {
    defeated = false;
    // Reset monster to its starting location
    location = currentLocation;
}




//az inja
bool Monster::attack(Game* game, const std::string& starResult, const std::string& exclamationResult) {
    bool attacked = false;
    Location* current = this->getLocation();

    // Only perform attack if starResult == "*"
    if (starResult == "*") {
        // First, check for villagers
        auto& villagers = current->getVillagers();
        if (!villagers.empty()) {
            Villager* victim = villagers.back();
            current->removeVillager(victim);
            game->increaseTerrorLevel();
            std::cout << "💀 Villager " << victim->getName() << " was killed by " << this->getName() << "!\n";
            return true;
        }
        // Then, check for heroes
        auto& heroes = game->getHeroes();
        std::vector<Hero*> heroesHere;
        for (Hero* hero : heroes) {
            if (hero && hero->getLocation() == current && hero->isAlive()) {
                heroesHere.push_back(hero);
            }
        }
        if (!heroesHere.empty()) {
            Hero* targetHero = heroesHere[0];
            if (heroesHere.size() > 1) {
                std::cout << "Multiple heroes are present! Choose who will be attacked:\n";
                for (size_t i = 0; i < heroesHere.size(); ++i) {
                    std::cout << (i+1) << ". " << heroesHere[i]->getPlayerName() << "\n";
                }
                int choice = 1;
                std::cout << "Enter the number of the hero to be attacked: ";
                std::cin >> choice;
                if (choice < 1 || choice > (int)heroesHere.size()) choice = 1;
                targetHero = heroesHere[choice-1];
            }
            std::cout << "⚔️ " << targetHero->getPlayerName() << " is being attacked by " << this->getName() << "!\n";
            std::cout << targetHero->getPlayerName() << ", you are being attacked!\n";
            std::cout << "Do you want to use one of your items to block the attack? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                if (!targetHero->getItems().empty()) {
                    targetHero->removeItem();
                    std::cout << "🛡️ " << targetHero->getPlayerName() << " used an item to block the attack!\n";
                    return true;
                } else {
                    std::cout << "You have no items to use!\n";
                }
            }
            // If player chooses not to use an item or has no items
            game->increaseTerrorLevel();
            targetHero->setInHospital(true);
            targetHero->moveTo(game->getLocationByName("Hospital"));
            std::cout << targetHero->getPlayerName() << " was injured and sent to the hospital!\n";
            return true;
        }
    }
    // Special power if exclamationResult == '!'
    if (exclamationResult == "!") {
        performAction(game);
        std::cout << name << " used special power with !" << std::endl;
    }
    std::cout << "No one was attacked.\n";
    return false;
}
//ta inja
