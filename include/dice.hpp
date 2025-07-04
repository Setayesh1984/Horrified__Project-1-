#ifndef DICE_HPP
#define DICE_HPP

#include <string>
#include <vector>
#include <random>

enum class DiceType 
{
    STAR,        // Represents a die with '*' on one face
    EXCLAMATION  // Represents a die with '!' on one face
};

class Dice 
{
private:
    DiceType type;
    std::mt19937 randomGenerator;
    std::uniform_int_distribution<> distribution;

public:
    Dice(DiceType type);
    std::string roll();
};

#endif 
