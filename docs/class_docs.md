
# 📘 Class Documentation - HORRIFIED (Phase 2)

> Last updated: 2025-07-10  
> Author: Backend Team  (Setayesh)
> Status: Synced with Phase 2 GameState

---

## 🎯 Purpose

This document provides a clear overview of all major classes used in the game logic.  
It helps maintain structure during refactoring, serialization, and frontend-backend integration.

---

## 🧠 Overview of Classes

---

### 🦸 class Hero (Abstract Base)

|  Field         |  Type             | Description                                             |
|----------------|-------------------|---------------------------------------------------------|
| `playerName`   | string            | The hero/player name                                    |
| `type`         | enum HeroType     | Hero role: e.g., Mayor, Archaeologist                   |
| `location`     | Location*         | Current hero location                                   |
| `items`        | vector<Item>      | Items currently held                                    |
| `perkCards`    | vector<PerkCard>  | Special ability cards                                   |
| `isInHospital` | bool              | True if recovering in hospital                          |
| `isAlive`      | bool              | Is the hero still alive? (used in monster attack logic) |

---

### 👹 class Monster (Abstract Base)

|  Field            | Type                    | Description                                                    |
|-------------------|-------------------------|----------------------------------------------------------------|
| `name`            | string                  | Monster's name (Dracula, etc.)                                 |
| `location`        | Location*               | Current position on the map                                    |
| `frenzy`          | bool                    | If the monster is currently the frenzy monster                 |
| `isDefeated`      | bool                    | Whether this monster is already defeated                       |
| `objectiveStatus` | vector<string> / bool[] | Dracula: coffins destroyed / Invisible Man: evidence collected |

---

### 🧍 class Villager

|   Field        |    Type       |  Description                    |
|----------------|---------------|---------------------------------|
| `name`         | string        | Villager's name                 |
| `location`     | Location*     | Where the villager is currently |
| `destination`  | Location*     | Final goal location for rescue  |
| `isAlive`      | bool          | True if not killed by a monster |

---

### 🗺️ class Location

|  Field       | Type                    | Description                        |
|--------------|-------------------------|------------------------------------|
| `name`       | string                  | Unique name for location           |
| `neighbors`  | vector<Location*>       | Connected locations (for movement) |
| `items`      | vector<Item>            | Items available at this location   |
| `villagers`  | vector<Villager*>       | Villagers present                  |
| `heroes`     | vector<Hero*>           | Heroes currently here              |
| `monsters`   | vector<Monster*>        | Monsters currently here            |

---

### 🧰 class Item

|   Field    |   Type      |  Description                               |
|------------|-------------|--------------------------------------------|
| `name`     | string      | Item name (e.g., Torch)                    |
| `color`    | enum        | RED, BLUE, YELLOW                          |
| `power`    | int         | Numeric value used in combat               |
| `location` | Location*   | Current location (or null if held by hero) |

---

### 🎴 class PerkCard

|  Field       |  Type     |  Description                                    |
|--------------|-----------|-------------------------------------------------|
| `name`       | string    | Name of the perk                                |
| `description`| string    | Ability summary for UI                          |

---

### 👾 class MonsterCard

|  Field       |  Type     |  Description                                    |
|--------------|-----------|-------------------------------------------------|
| `name`       | string    | Card name                                       |
| `effect`     | string    | Effect to apply on draw (advance monster logic) |

---

### 🎮 class Game

|  Field               |  Type                     |  Description                       |
|----------------------|---------------------------|------------------------------------|
| `heroes`             | vector<Hero*>             | All active heroes                  |
| `monsters`           | vector<Monster*>          | All active monsters                |
| `locations`          | vector<Location*>         | All playable locations             |
| `villagers`          | vector<Villager*>         | Remaining villagers                |
| `itemBag`            | vector<Item>              | Undrawn items                      |
| `itemsOnBoard`       | vector<Item>              | Items placed on the board          |
| `perkDeck`           | vector<PerkCard>          | Unused perk cards                  |
| `monsterDeck`        | vector<MonsterCard>       | Monster encounter cards            |
| `currentTurn`        | int                       | Which turn the game is on          |
| `currentPhase`       | enum {Hero, Monster}      | Which phase is active              |
| `currentHeroIndex`   | int                       | Index of the hero playing          |
| `terrorLevel`        | int                       | Number of deaths / terror increase |
| `frenzyMonster`      | Monster*                  | The frenzy monster (extra turn)    |
| `defeatedMonsters`   | int                       | Number of monsters defeated        |
| `lastMonsterRolls`   | vector<string>            | Last rolls like "*", "!", "*"      |

---

## 🛠️ Key Methods (To be documented as implemented)

| Class   | Method           | Description                          |
|---------|------------------|--------------------------------------|
| Game    | `saveState()`    | Serialize current state to file      |
| Game    | `loadState()`    | Load state from file into memory     |
| Hero    | `takeAction()`   | Run main action menu for this hero   |
| Monster | `attack()`       | Perform monster attack based on dice |
| Game    | `advanceTask()`  | Advance quest progress for a monster |
| Game    | `defeatMonster()`| Final step to defeat a monster       |

---

##  Conventions & Notes

- All pointer fields must be saved using **unique string identifiers** (e.g., `location name`)
- Enum values (HeroType, MonsterType, Phase) should be saved as strings
- Keep this document updated if **new classes, methods, or states** are added during Phase 2
- Consider splitting abstract and derived classes (e.g., `Dracula`, `InvisibleMan`) in future versions

---

📘 This file is synced with [`GameState.md`](./GameState.md) and must be kept aligned to ensure consistency across Save/Load, UI, and gameplay logic.
