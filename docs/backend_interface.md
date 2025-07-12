# 🔌 Backend Interface for Frontend - Phase 2

> Author: Backend Team  (Setayesh)
> Last updated: 2025-07-12

---

## 📦 Purpose

This document defines the shared interface between Backend and Frontend (UI) in Phase 2 of the HORRIFIED project.  
It provides a full explanation of the `GameState` structure and how UI will communicate with the game logic.

---

## 🧠 GameState Struct Overview

`GameState` is a single struct that contains the **entire game state** — including heroes, monsters, items, turns, etc.

### 🔻 Function: `GameState Game::getGameState()`
- Exports the current state of the game.
- Called by Frontend to **read** the full game state.

### 🔺 Function: `void Game::setGameState(const GameState&)`
- Overwrites current game state with a new one.
- Called by Frontend to **load** a previous save or restart the game.

---

## 📑 GameState Fields

| Field Name            | Type                    | Description                                  |
|-----------------------|-------------------------|----------------------------------------------|
| currentTurn           | int                     | Current turn number                          |
| terrorLevel           | int                     | Current terror level                         |
| defeatedMonsters      | int                     | Number of monsters defeated                  |
| frenzyMonsterName     | std::string             | Name of the frenzy monster                   |
| heroes                | vector<HeroState>       | List of all active heroes                    |
| monsters              | vector<MonsterState>    | List of all monsters and their state         |
| locations             | vector<LocationState>   | Name + items + villagers per location        |
| villagers             | vector<VillagerState>   | Current villagers, their location/status     |
| itemBag               | vector<Item>            | Remaining item bag (draw pile)               |
| perkDeck              | vector<PerkCard>        | Remaining Perk cards                         |
| monsterDeck           | vector<MonsterCard>     | Remaining Monster cards                      |
| lastMonsterDiceRolls  | vector<string>          | Dice rolled in last monster phase            |

>  NOTE: Whenever new game content or features are added (new heroes, mechanics…), these fields must be updated accordingly.

---

## 🔁 Example Workflow

1. `GameState current = game.getGameState();`  
2. Save it to file or send to UI
3. Later: `game.setGameState(previousState);` to restore

---

## 🧪 Used In

- Save/Load game to file (Phase 3)
- Frontend → Backend sync
- Multiplayer future updates (turn sharing)

