#include "Game.hpp"
#include <thread>
#include <chrono>

Game::Game(int w, int h, int preyPerc, int predatorPerc):
  WIDTH(w), HEIGHT(h), PREY_PERCENTAGE(preyPerc), PREDATOR_PERCENTAGE(predatorPerc) {

  srand(time(0));

  initWorld();
  generateCreatures();

  window.create(sf::VideoMode(WIDTH * MULTIPLYER, HEIGHT * MULTIPLYER), "Predator & Prey");
}

void Game::initWorld() {
  world = new Cell[WIDTH * HEIGHT];
  worldAux = new Cell[WIDTH * HEIGHT];
}

void Game::generateCreatures() {

  for (int i = 0; i < HEIGHT; ++ i) {
    for (int j = 0; j < WIDTH; ++ j) {

      int chance = rand() % 100;
      if (chance < PREY_PERCENTAGE) {
        preyMap[i][j].push_back(Prey());
      }
      else if (chance < PREY_PERCENTAGE + PREDATOR_PERCENTAGE) {
        predatorMap[i][j].push_back(Predator());
      }
    }
  }
}

void Game::updateState() {

  for (int i = 0; i < HEIGHT; ++ i) {
    for (int j = 0; j < WIDTH; ++ j) {
      for (auto &prey : cell) {
        prey.updateHealth();
        prey.updatePosition(HEIGHT, WIDTH);
        auto newPos = prey.getPosition();

        if (prey.canReproduce() and (int) preyMapAux[newPos.i][newPos.j].size() - 1 < MAX_PREY_PER_CELL) {
          prey.resetHealth();
          preyMapAux[newPos.i][newPos.j].push_back(Prey(newPos));
        }
        preyMapAux[newPos.i][newPos.j].push_back(prey);
      }
      cell.clear();
    }
  }

  for (auto &line : predatorMap) {
    for (auto &cell : line) {
      for (auto &predator : cell) {
        predator.updateHealth();
        if (predator.isAlive()) {
          predator.updatePosition(HEIGHT, WIDTH);
          auto newPos = predator.getPosition();

          // try to eat prey
          if (not preyMapAux[newPos.i][newPos.j].empty()) {
            auto prey = preyMapAux[newPos.i][newPos.j].back();
            preyMapAux[newPos.i][newPos.j].pop_back();
            predator.updateHealth(prey.getHealth());
            if (predator.canReproduce()) {
              predatorMapAux[newPos.i][newPos.j].push_back(Predator(newPos));
            }
          }
          predatorMapAux[newPos.i][newPos.j].push_back(predator);
        }
      }

      cell.clear();
    }
  }

  swap(preyMap, preyMapAux);
  swap(predatorMap, predatorMapAux);
}

void Game::display() {

  window.clear(sf::Color::Black);
  for (int i = 0; i < HEIGHT; ++ i) {
    for (int j = 0; j < WIDTH; ++ j) {
      
      int preyCnt = (int) preyMap[i][j].size();
      int predatorCnt = (int) predatorMap[i][j].size();

      if (preyCnt + predatorCnt == 0) continue;

      sf::RectangleShape point(sf::Vector2f(MULTIPLYER, MULTIPLYER));
      point.setPosition(j * MULTIPLYER, i * MULTIPLYER);
      if (preyCnt > predatorCnt) {
        point.setFillColor(sf::Color::Green);
      }
      else {
        point.setFillColor(sf::Color::Red);
      }

      window.draw(point);
    }
  }

  window.display();
}

void Game::run() {

  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    updateState();
    display();
  }
}

Game::~Game() {
  delete [] world;
  delete [] worldAux;
}
