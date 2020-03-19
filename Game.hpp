#pragma once

#include <vector>
#include "Creature.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:

  Game(int, int, int, int);

  void run();

private:

  sf::RenderWindow window;

  const int WIDTH, HEIGHT;
  static const int MAX_PREY_PER_CELL = 3;
  static const int MULTIPLYER = 4;
  const int PREY_PERCENTAGE, PREDATOR_PERCENTAGE; // out of 100

  std::vector < std::vector < std::vector < Prey > > > preyMap, preyMapAux; 
  std::vector < std::vector < std::vector < Predator > > > predatorMap, predatorMapAux; 

  void initMap();
  void generateCreatures();
  void updateState();
  void display();
};
