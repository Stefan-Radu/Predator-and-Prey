#include <iostream>
#include <fstream>
#include "Game.hpp"

int main () {

  const int GAMES_CNT = 7;
  Game **g = new Game*[GAMES_CNT];

  for (int gameCnt = 0; gameCnt < GAMES_CNT; ++ gameCnt) {

    int preyPerc = rand() % 50 + 1;
    int predatorPerc = rand() % 50 + 1;
    int preyMaxHealth = rand() % 100 + 1;
    int preyHealthTic = rand() % 5 + 1;
    int predatorMaxHealth = rand() % 100 + 1;
    int predatorHealthTic = rand() % 5 + 1;

    g[gameCnt] = new Game(preyPerc, predatorPerc,
                    preyMaxHealth, preyHealthTic,
                    predatorMaxHealth, predatorHealthTic);

    std::cerr << "\nInitialized game no. " << gameCnt << " with:\n";
    std::cerr << "Prey percentage: " << predatorPerc << " / 1000\n";
    std::cerr << "Predator percentage: " << predatorPerc << " / 1000\n";
    std::cerr << "Maximum health of prey: " << preyMaxHealth <<  '\n';
    std::cerr << "Health tic of prey (+- 1): " << preyHealthTic <<  '\n';
    std::cerr << "Maximum health of predator: " << predatorMaxHealth <<  '\n';
    std::cerr << "Health tic of predator (+- 1): " << predatorHealthTic <<  '\n';

    g[gameCnt] -> run();
    delete g[gameCnt];
  }

  delete[] g;

  // TODO implement nuke / disease to killall
  // TODO assert limits
}