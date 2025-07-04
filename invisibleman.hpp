#ifndef INVISIBLEMAN_HPP
#define INVISIBLEMAN_HPP

#include "monster.hpp"
#include <map>


class InvisibleMan : public Monster 
{
private:
    int evidencesFound;
  //  int requiredEvidence;
    std::vector<std::string> evidenceLocations = {"Inn", "Barn", "Institute", "Laboratory", "Mansion"};
    std::vector<bool> evidencesCollected = {false, false, false, false, false};
    std::vector<std::string> evidenceNames = {"Footprint", "Glove", "Photo", "Note", "Bottle"};
    
    std::map<std::string, bool> requiredEvidence;


public:
  
    InvisibleMan(Location* startLocation);
    void attack(Game* game);

    int getEvidenceCount() const;
    void findEvidence(const std::string& location);  // بازیکن شواهد رو پیدا می‌کنه

    bool canBeDefeated(const std::vector<Item>& items) const override;
    std::string getName() const override;

    void performAction(Game* game) override;

    int getFrenzyOrder() const override { return 6; }

    
    bool dropEvidence(const Item& item, const std::string& location) {
        for (size_t i = 0; i < evidenceLocations.size(); ++i) {
            if (!evidencesCollected[i] &&
                evidenceLocations[i] == location &&
                item.getName() == evidenceNames[i]) {
                evidencesCollected[i] = true;
                return true;
            }
        }
        return false;
    }

    

    const std::map<std::string, bool>& getRequiredEvidence() const;
    void markEvidenceFound(const std::string& name); // برا
};

#endif

