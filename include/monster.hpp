#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <string>
#include "location.hpp"

class Game; // Forward declaration of Game class

class Monster 
{
    protected:
        std::string name;
        Location* currentLocation;
        Location* location;
        bool defeated;
    
    public:
        Monster(const std::string& name, Location* startLocation);
        virtual ~Monster() = default;

        virtual int getFrenzyOrder() const = 0;
        
        bool attack(Game* game, const std::string& starResult, const std::string& exclamationResult);
        Location* getCurrentLocation()const{return currentLocation;}
        //std::string getName() const;
        
        Location* getLocation() const;
        void moveTo(Location* location);
    
        bool isDefeated() const;
        void markDefeated();
    
        virtual void performAction(Game* game) = 0;
        // virtual bool canBeDefeated() const = 0;  // ✅ این خط رو اضافه کن
        
        void defeat();
        void move(int steps);
        void moveTowardTarget();
        virtual std::string getName() const = 0;  // کلاس انتزاعی
        virtual bool canBeDefeated(const std::vector<Item>& items) const = 0;
        void takeAction(Game* game);  // Modified to accept Game* parameter
        void respawn();     // New method for respawning
        
    protected:
        Location* findNearestTargetLocation(); // Helper for BFS
    };
    

#endif
