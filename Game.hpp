#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Vec2D.hpp"
#include "Creature.hpp"

enum class SimulationStage { CREATURE_DEAD, MOVE_CELL, LIFE_TRANSFER };

class Game {
public:

  // TODO singleton

  Game(int, int, int, int, int, int);

  ~Game();

  void run();

  static const int CHANCE_MODULO = 100;

private:

  struct Cell {
    Cell();
    CreatureType type;
    Creature *creature;
    Cell& operator = (const Cell&);
    ~Cell();
  };

  bool endGame;
  int preyCnt, predatorCnt;

  sf::RenderWindow window;

  static const sf::Color RED;
  static const sf::Color GREEN;
  static const sf::Color PURPLE;
  static const sf::Color ORANGE;
  static const sf::Color CLEAR;

  const int PREY_PERCENTAGE, PREDATOR_PERCENTAGE;

  static const int ILLNESS_CHANCE = 7;
  static const int WIDTH = 600, HEIGHT = 400;
  static const int NEXT_POS_TRIES_THRESHOLD = 4;

  static constexpr double END_GAME_THRESHOLD = 15.0;
  static constexpr double NO_CREATURES_THRESOLD = 1.5;

  Prey* const defaultPrey;
  Predator* const defaultPredator;

  static Cell nullCell;

  Cell *world, *worldAux;
  sf::Vertex *pixels;

  void initWorld();
  void initPixels();
  void generateCreatures();
  void initEverything();

  int getNextCellIndex(const int&) const;

  void addCreature(Cell&, CreatureType);
  void removeCreature(Cell&);
  bool notSurvive(const int&);
  void chanceMakeIll(const int&);
  void interact(const int&, const int&);
  void transferLife(const int&, const int&);

  void updateCell(const int&);
  void updateState();

  void colorPixel(int&, int&, const sf::Color&);
  void display();

  Vec2D get2DPos(const int&) const; 
  int get1DPos(const Vec2D&) const; 
  int get1DPos(const int&, const int&) const; 
  Vec2D wrap(Vec2D) const;
};
