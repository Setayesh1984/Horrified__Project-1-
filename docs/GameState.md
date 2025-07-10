# 🧠 Game State - HORRIFIED (Phase 2)

> Last updated: 2025-07-10  
> Author: Backend Team (Setayesh) 
> Status:  Ready for Serialization + UI Sync

---

## 🎯 Purpose

This document defines the **full game state structure** that must be serialized (saved/loaded) and kept in sync with the UI.

It serves as the foundation for:
- Save/Load System 
- Backend ↔ Frontend Communication 
- Graphical Display (Raylib or UI Framework) 

---

## 💾 Main Serializable Game State Fields

|  Field Name          |  Related Class    |  Type                  |  Description                                             |
|----------------------|-------------------|------------------------|----------------------------------------------------------|
| `heroes`             | Game              | vector<Hero>           | All active heroes with name, type, location, items, etc. |
| `monsters`           | Game              | vector<Monster>        | All monsters with current state, location, frenzy, etc.  |
| `locations`          | Game              | vector<Location>       | All map points, with item/villager lists and neighbors   |
| `itemsInBag`         | Game              | vector<Item>           | Undrawn items remaining in the item bag                  |
| `perkDeck`           | Game              | vector<PerkCard>       | Remaining Perk cards in the draw pile                    |
| `monsterDeck`        | Game              | vector<MonsterCard>    | Remaining Monster cards in the draw pile                 |
| `villagers`          | Game              | vector<Villager>       | All villagers on map or saved                            |
| `terrorLevel`        | Game              | int                    | Current terror level (0-5)                               |
| `currentTurn`        | Game              | int                    | Turn number (used in Save/Resume)                        |
| `currentPhase`       | Game              | enum {Hero, Monster}   | Current game phase (used for restoring mid-turn)         |
| `currentHeroIndex`   | Game              | int                    | Which hero is currently playing their phase              |
| `frenzyMonster`      | Game              | string (monster name)  | Name of the frenzy monster (for lookup)                  |
| `defeatedMonsters`   | Game              | int                    | Number of monsters defeated so far                       |
| `lastMonsterRolls`   | Game              | vector<string>         | Last monster dice results (e.g. ["*", "!", "*"])         |

---

## 🧩 Entity Structure Overview

Each component in the state is defined by its own struct or class. Below are the fields that must be serialized inside each.

---

### 🦸 Hero

| Field          | Type                   | Notes                      |
|----------------|------------------------|----------------------------|
| `name`         | string                 | Player's name              |
| `type`         | enum HeroType          | e.g., Mayor, Archaeologist |
| `location`     | string (location name) | Pointer by name            |
| `items`        | vector<Item>           | Held items                 |
| `perkCards`    | vector<PerkCard>       | Held perks                 |
| `isInHospital` | bool                   | Whether hero is recovering |

---

### 👹 Monster

| Field             | Type                   | Notes                                 |
|-------------------|------------------------|---------------------------------------|
| `name`            | string                 | Monster type (Dracula, Invisible Man) |
| `location`        | string                 | Pointer by name                       |
| `frenzy`          | bool                   | Is frenzy active?                     |
| `objectiveStatus` | vector<bool or string> | Coffin status or evidence collected   |

---

### 🧍 Villager

| Field        | Type                  | Notes                           |
|--------------|-----------------------|---------------------------------|
| `name`       | string                | Villager's name                 |
| `location`   | string                | Current location                |
| `destination`| string                | Where they must be saved to     |
| `isAlive`    | bool                  | Dead villagers increase terror! |

---

### 📍 Location

| Field        | Type                  | Notes                                         |
|--------------|-----------------------|-----------------------------------------------|
| `name`       | string                | Unique ID of the location                     |
| `items`      | vector<Item>          | Items present at location                     |
| `villagers`  | vector<string>        | Names of present villagers                    |
| `heroes`     | vector<string>        | Names of present heroes                       |
| `monsters`   | vector<string>        | Names of present monsters                     |
| `neighbors`  | vector<string>        | Connected location names (for movement logic) |

---

### 🧰 Item

| Field      | Type     | Notes                               |
|------------|----------|-------------------------------------|
| `name`     | string   | e.g., "Torch", "Rifle"              |
| `color`    | enum     | RED, BLUE, YELLOW                   |
| `power`    | int      | Item power rating (used in attacks) |

---

### 🎴 PerkCard

| Field         | Type     | Notes               |
|---------------|----------|---------------------|
| `name`        | string   | Perk card title     |
| `description` | string   | UI text for ability |

---

## 🔐 Serialization Notes

- Use JSON or binary formats
- Enum values should be serialized as strings (e.g., `"Archaeologist"`)
- Pointers (e.g., Location*) should be stored as **unique names** and resolved on load

---

## 🔮 Future Fields (Planned in Phase 2)

- `menuState: enum` → Which screen was active on save?
- `selectedHeroes: vector<string>` → For replay
- `highlightedPath: vector<string>` → For animated moves (in UI)

---

## 📎 Related Documents

- [`class_docs.md`](./class_docs.md) → Class-level descriptions
- [`ui_requirements.md`](./ui_requirements.md) → Required UI ↔ Backend interfaces

---

📘 **Tip:** Any additions to the game logic (new monsters, heroes, or map changes) must be reflected here and kept in sync with the struct used for save/load and UI rendering.



