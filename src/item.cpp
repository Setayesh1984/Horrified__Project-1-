#include "item.hpp"
#include "string"

Item::Item(const std::string &name, ItemColor color, int power, const std::string &location)
    : name(name), color(color), power(power), locationName(location) {}

std::string Item::getName() const
{
    return name;
}

ItemColor Item::getColor() const
{
    return color;
}

int Item::getPower() const
{
    return power;
}

std::string Item::getLocation() const
{
    return locationName;
}

void Item::setLocation(const std::string &newLocation)
{
    locationName = newLocation;
}

std::string Item::itemColorToString(ItemColor color)
{
    switch (color)
    {
    case ItemColor::Red:
        return "Red";
    case ItemColor::Blue:
        return "Blue";
    case ItemColor::Yellow:
        return "Yellow";
    default:
        return "Unknown";
    }
}

std::string Item::getLocationName() const
{
    return locationName;
}
