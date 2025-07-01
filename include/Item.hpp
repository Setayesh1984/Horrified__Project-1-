#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

enum class ItemColor 
{
    Red,
    Blue,
    Yellow,
    Green
};

class Item 
{
private:
    std::string name;
    ItemColor color;
    int power;
    std::string locationName;

public:
    Item(const std::string& name, ItemColor color, int power, const std::string& location);
    std::string itemColorToString(ItemColor color);
    std::string getName() const;
    ItemColor getColor() const;
    int getPower() const;
    std::string getLocation() const;
    std::string getLocationName() const;
    void setLocation(const std::string& newLocation);
};

#endif