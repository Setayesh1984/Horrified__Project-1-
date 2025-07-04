#include "dice.hpp"
#include <iostream>

Dice::Dice(DiceType type) : type(type), randomGenerator(std::random_device()()), distribution(1, 6) {}

std::string Dice::roll()
{
    int result = distribution(randomGenerator);
    if (type == DiceType::STAR)
    {
        return (result == 1) ? "*" : "";
    }
    else if (type == DiceType::EXCLAMATION)
    {
        return (result == 1) ? "!" : "";
    }
    return "";
}
