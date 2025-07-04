#ifndef PERKCARD_HPP
#define PERKCARD_HPP

#include <string>

class Game; // Forward declaration
class Hero; // Forward declaration
class Monster; // Forward declaration

class PerkCard {
private:
    std::string name;
    std::string description;

public:
    PerkCard(const std::string& name, const std::string& description);

    std::string getName() const;
    std::string getDescription() const;

    void activate(Game* game, Hero* hero) const;

};

#endif