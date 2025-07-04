 cmake_minimum_required(VERSION 3.10)
 project(Horrified)

 set(CMAKE_CXX_STANDARD 17)

 include_directories(include)

 add_executable(Horrified
     src/main.cpp
     src/item.cpp
     src/location.cpp
     src/hero.cpp
     src/monster.cpp
     src/dracula.cpp
     src/invisibleman.cpp
     src/villager.cpp
     src/perkcard.cpp
     src/monstercard.cpp
     src/game.cpp
     src/dice.cpp
)
