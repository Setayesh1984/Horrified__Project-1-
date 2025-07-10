# 🖥️ UI Requirements & Data Flow - HORRIFIED (Phase 2)

> Last updated: 2025-07-10  
> Author: Frontend & Backend Teams (Mahdi & Setayesh) 
> Status:  Final Version for Phase 2 Implementation

---

## 🎯 Purpose

This document defines the **graphical and structural requirements** for the User Interface (UI) in Phase 2.  
It includes:
- Expected UI features  
- Backend↔Frontend data communication  
- Events and visual feedback  
- Struct-based interface design

---

## 🖼️ Graphical Features Required in Phase 2

|  Feature                      | Description                                                            |
|-------------------------------|------------------------------------------------------------------------|
| Main Menu                     | Start game, resume, exit options                                       |
| Hero Selection Screen         | Choose between 6+ heroes (Phase 2 expansion)                           |
| Game Map Visualization        | Real-time rendering of locations, paths, heroes, monsters              |
| Hero HUD                      | Display name, HP, items, perks, and actions remaining                  |
| Monster Status UI             | Icons or boxes showing monster locations, frenzy status                |
| Villager Info Panel           | List of villagers with status (saved/in danger), location, destination |
| Item Display (Map + Bag)      | Items at locations and those held by each player                       |
| Action Buttons                | Move, Pick Up, Guide, Defeat, Perk, etc.                               |
| Dynamic Dialog Popups         | System messages, confirmations, win/lose, help                         |
| Save/Load Screens             | Visual file selectors or slots, triggered via UI buttons               |
| Turn & Phase Indicator        | Clearly show Hero vs Monster phase & current player turn               |
| Dice Roll Animation (optional)| Visually simulate dice rolling when attacking                          |
| Hover / Select Feedback       | Highlight locations or characters interactively                        |

---

## 📊 Data to be Displayed in UI

|  Game Data         |  Description                                           |
|--------------------|--------------------------------------------------------|
| Hero Info          | Name, type, current location, inventory, perks, actions|
| Monster Info       | Type, current location, frenzy status                  |
| Villagers          | Name, current location, destination, alive status      |
| Items              | Per location (on board), per hero (inventory)          |
| Perk Cards         | Cards held by each hero                                |
| Turn Counter       | Current turn number                                    |
| Game Phase         | Hero Phase or Monster Phase                            |
| Terror Level       | Current terror level (0-5)                             |
| Frenzy Monster     | Icon / name of current frenzy monster                  |
| Monster Card Drawn | Name + effect of the card (popup)                      |
| Game Map Data      | Connections between locations, available movement      |

---

## 📤 Events Sent From UI → Backend

|  UI Event                  | Triggered Backend Logic                          |
|----------------------------|--------------------------------------------------|
| Player Count Selection     | Initializes 1P / 2P setup                        |
| Hero Selection             | Sets hero type + name for each player            |
| Move to Location           | Move to adjacent node (validated by backend)     |
| Pick Up Item               | Adds item to inventory, removes from location    |
| Guide Villager             | Villager walks with hero or is moved to location |
| Use Perk Card              | Applies ability effect and removes from hand     |
| Attempt to Defeat Monster  | Triggers dice roll + combat sequence             |
| Respond to Monster Attack  | Choose whether to block with item                |
| Advance Monster Mission    | Drop evidence / destroy coffins                  |
| Show Help / Manual         | Opens overlay or modal                           |
| Save Game                  | Exports full GameState to file                   |
| Load Game                  | Imports full GameState and refreshes UI          |
| End Turn                   | Switches to next phase (Hero ↔ Monster)          |

---

## 🔌 Backend-Frontend Interface Points

All communication between frontend and game logic must go through **two main interfaces**:

### ✅ From Backend to UI:
```cpp
GameState getFullGameState();   // For rendering UI based on live data
