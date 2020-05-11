#include "Game.hpp"
#include <chrono>
#include <thread>

//==================================================  Initialization  ==================================================

const sf::Color Game::GREEN(10, 166, 88, 17);
const sf::Color Game::RED(194, 16, 16, 17);
const sf::Color Game::PURPLE(140, 16, 194, 17);
const sf::Color Game::ORANGE(207, 122, 10, 17);
const sf::Color Game::CLEAR(0, 0, 0, 17);

int Game::gameCount = 1;

Game::Game():
  WIDTH(userInput("Provide window width: ")),
  HEIGHT(userInput("Provide window height: ")),
  PREY_PERCENTAGE(rand() % MAX_CREATURE_PERCENTAGE + 1),
  PREDATOR_PERCENTAGE(rand() % MAX_CREATURE_PERCENTAGE + 1),
  defaultPrey(new Prey(rand() % MAX_CREATURE_HEALTH + 1, rand() % MAX_CREATURE_HEALTH_TIC + 1)),
  defaultPredator(new Predator(rand() % MAX_CREATURE_HEALTH + 1, rand() % MAX_CREATURE_HEALTH_TIC + 1)) {

  srand(time(0));
  initEverything();
  logDetails();
}

Game& Game::getInstance() {
  static Game game;
  return game;
}

int Game::userInput(const char *message) const {

  int input;
  bool notOK = true;

  while (notOK) {

    notOK = false;
    std::cerr << message;

    try {
     std::cin >> input; 
     if (input > 1001) {
       throw -1;
     }
     else if (input <= 0) {
       throw -2;
     }
    }
    catch (int e) {
      notOK = true;
      switch (e) {
        case -1:
          std::cerr << "number too big\n";
          break;
        case -2:
          std::cerr << "number too small\n";
          break;
      }
    }
  }

  return input;
}

void Game::logDetails() const {

  std::system("clear");

  std::cerr << "Initialized game no. " << gameCount << " with:\n";
  std::cerr << "Prey percentage: " << PREY_PERCENTAGE << " / " << Game::CHANCE_MODULO << '\n';
  std::cerr << "Predator percentage: " << PREDATOR_PERCENTAGE << " / " << Game::CHANCE_MODULO << '\n';
  std::cerr << "Prey:\n" << *defaultPrey;
  std::cerr << "Predator:\n" << *defaultPredator;
}

void Game::resetGame() {

  PREY_PERCENTAGE = rand() % MAX_CREATURE_PERCENTAGE + 1;
  PREDATOR_PERCENTAGE = rand() % MAX_CREATURE_PERCENTAGE + 1;

  delete defaultPrey;
  defaultPrey = new Prey(rand() % MAX_CREATURE_HEALTH + 1, rand() % MAX_CREATURE_HEALTH_TIC + 1);

  delete defaultPredator;
  defaultPredator = new Predator(rand() % MAX_CREATURE_HEALTH + 1, rand() % MAX_CREATURE_HEALTH_TIC + 1);

  ++ gameCount;
  endGame = false;
  elapsedTime = 0.0;

  generateCreatures();
  logDetails();
}

void Game::initEverything() {

  srand(time(0));

  endGame = false;
  elapsedTime = 0.0;

  initWorld();
  initPixels();
  generateCreatures();

  window.create(sf::VideoMode(WIDTH << 1, HEIGHT << 1), "Predator & Prey");
  window.setFramerateLimit(27);
  window.setVerticalSyncEnabled(true);
}

void Game::initWorld() {
  world = new Creature*[WIDTH * HEIGHT]();
  worldAux = new Creature*[WIDTH * HEIGHT]();
}

void Game::initPixels() {
  pixels = new sf::Vertex[WIDTH * HEIGHT << 2];
  for (int i = 0; i < (HEIGHT << 1); ++ i) {
    for (int j = 0; j < (WIDTH << 1); ++ j) {
      pixels[i * (WIDTH << 1) + j].position = sf::Vector2f(j, i);
    }
  }
}

void Game::generateCreatures() {
  for (int cell = 0; cell < WIDTH * HEIGHT; ++ cell ) {

    if (world[cell]) {
      delete world[cell];
      world[cell] = nullptr;
    }

    int chance = rand() % CHANCE_MODULO;
    if (chance < PREY_PERCENTAGE) {
      world[cell] = new Prey(*defaultPrey);
    }
    else if (chance < PREY_PERCENTAGE + PREDATOR_PERCENTAGE) {
      world[cell] = new Predator(*defaultPredator);
    }
  }
}

//==================================================  Game Logic  ==================================================

CreatureType Game::getCreatureType(Creature *& creature) const {
  return creature ? creature->type : CreatureType::NOTHING;
}

void Game::removeCreature(Creature *&creature) {
  delete creature;
  creature = nullptr;
}

void Game::addCreature(Creature *&creature, CreatureType type) {
  switch (type) {
    case CreatureType::PREY:
      creature = new Prey(*defaultPrey);
      break;
    case CreatureType::PREDATOR:
      creature = new Predator(*defaultPredator);
      break;
    case CreatureType::NOTHING:
      std::cerr << "Cannot create NOTHING";
      break;
  }
}

bool Game::notSurvive(Creature *& creature) {

  creature->updateHealth();

  if (creature->isDead()) {
    removeCreature(creature);
    return true; 
  }
  return false;
}

void Game::chanceMakeIll(Creature *&creature) {
  if (endGame and rand() % CHANCE_MODULO < ILLNESS_CHANCE) {
    creature->makeIll();
  }
}

int Game::getNextCellIndex(const int &cell) const {
  int nextCell = get1DPos(wrap(get2DPos(cell) + Vec2D::getRandomWay()));
  for (int i = 0; i < NEXT_POS_TRIES_THRESHOLD and getCreatureType(worldAux[nextCell]) == getCreatureType(world[cell]); ++ i) {
    nextCell = get1DPos(wrap(get2DPos(cell) + Vec2D::getRandomWay()));
  }
  return nextCell;
}

void Game::interact(const int &cellIndex, const int &nextCellIndex) {

  // move cell
  if (getCreatureType(worldAux[nextCellIndex]) == CreatureType::NOTHING) {
    worldAux[nextCellIndex] = world[cellIndex];
  }
  else {
    switch (getCreatureType(world[cellIndex])) {
      case CreatureType::PREY:
        worldAux[nextCellIndex]->resetHealth();
        removeCreature(world[cellIndex]);
        break;
      case CreatureType::PREDATOR:
        world[cellIndex]->resetHealth();
        removeCreature(worldAux[nextCellIndex]);
        worldAux[nextCellIndex] = world[cellIndex];
        break;
      case CreatureType::NOTHING:
        std::cerr << "Cannot move NOTHING\n";
        break;
    }
  }

  // reproduce
  if (worldAux[nextCellIndex]->canReproduce() and getCreatureType(worldAux[cellIndex]) == CreatureType::NOTHING) {
    addCreature(worldAux[cellIndex], getCreatureType(worldAux[nextCellIndex]));
  }
}

void Game::transferLife(const int &cellIndex, const int &nextCellIndex) {
  worldAux[nextCellIndex]->updateHealth(world[cellIndex]->getHealth());
  removeCreature(world[cellIndex]);
}

void Game::updateCell(const int &cellIndex) {

  if (getCreatureType(world[cellIndex]) == CreatureType::NOTHING) return;
  if (notSurvive(world[cellIndex])) return;

  chanceMakeIll(world[cellIndex]);

  int nextCellIndex = getNextCellIndex(cellIndex);

  if (getCreatureType(worldAux[nextCellIndex]) != getCreatureType(world[cellIndex])) {
    interact(cellIndex, nextCellIndex);
  }
  else {
    transferLife(cellIndex, nextCellIndex);
  }

  world[cellIndex] = nullptr;
}

void Game::updateState() {
  for (int cellIndex = 0; cellIndex < WIDTH * HEIGHT; ++ cellIndex) {
    updateCell(cellIndex);
  }
  std::swap(worldAux, world);
}

//==================================================  Functionality  ==================================================

void Game::colorPixel(int &i, int &j, const sf::Color &color) {
  pixels[(i << 1) * (WIDTH << 1) + (j << 1)].color = color;
  pixels[(i << 1) * (WIDTH << 1) + (j << 1 | 1)].color = color;
  pixels[(i << 1 | 1) * (WIDTH << 1) + (j << 1)].color = color;
  pixels[(i << 1 | 1) * (WIDTH << 1) + (j << 1 | 1)].color = color;
}

void Game::display() {

  for (int i = 0; i < HEIGHT; ++ i) {
    for (int j = 0; j < WIDTH; ++ j) {
      
      Creature *creature = world[get1DPos(i, j)];

      switch(getCreatureType(creature)) {
        case CreatureType::NOTHING:
          colorPixel(i, j, CLEAR);
          break;
        case CreatureType::PREY:
          if(creature->isIll()) colorPixel(i, j, ORANGE);
          else colorPixel(i, j, GREEN);
          break;
        case CreatureType::PREDATOR:
          if (creature->isIll()) colorPixel(i, j, PURPLE);
          else colorPixel(i, j, RED);
          break;
      }
    }
  }

  window.draw(pixels, WIDTH * HEIGHT << 2, sf::Points);

  sf::Text text;
  sf::Font font;
  font.loadFromFile("./assets/Peepo.ttf");
  text.setFont(font);
  text.setString("Press N to start a new simulation\n"
                "Press Q to quit\n");
  text.setCharacterSize((WIDTH / 32.0f));
  text.setFillColor(sf::Color::White);
  text.setPosition(7, 7);
  window.draw(text);

  window.display();
}

void Game::run() {

  window.clear(CLEAR);
  window.display();

  auto lastTime = std::chrono::high_resolution_clock::now();

  while (window.isOpen()) {

    auto currentTime = std::chrono::high_resolution_clock::now();
    elapsedTime += std::chrono::duration_cast < std::chrono::milliseconds > (currentTime - lastTime).count();
    lastTime = currentTime;

    if (not endGame and elapsedTime > END_GAME_THRESHOLD) {
      defaultPrey->makeIll();
      defaultPredator->makeIll();
      endGame = true;
    }

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      else if (event.type == sf::Event::KeyPressed) { 
        if (event.key.code == sf::Keyboard::Q) {
          window.close();
        }
        else if (event.key.code == sf::Keyboard::N) {

          sf::Text text;
          sf::Font font;
          font.loadFromFile("./assets/Peepo.ttf");
          text.setFont(font);
          text.setString("N");
          text.setCharacterSize(WIDTH / 4.0f);
          text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height);
          text.setPosition(WIDTH, HEIGHT);
          text.setFillColor(sf::Color::White);
          window.draw(text);

          resetGame();
        }
      }
    }

    updateState();
    display();
  }
}

Vec2D Game::get2DPos(const int &cell) const {
  return Vec2D(cell / WIDTH, cell % WIDTH);
}

int Game::get1DPos(const Vec2D &pos) const {
  return pos.i * WIDTH + pos.j;
}

int Game::get1DPos(const int &i, const int &j) const {
  return i * WIDTH + j;
}

Vec2D Game::wrap(Vec2D pos) const {
  if (pos.i < 0) pos.i = HEIGHT - 1;
  else if (pos.i >= HEIGHT) pos.i = 0;
  if (pos.j < 0) pos.j = WIDTH - 1;
  else if (pos.j >= WIDTH) pos.j = 0;
  return pos;
}

Game::~Game() {

  elapsedTime = 0;
  PREY_PERCENTAGE = 0;
  gameCount = endGame = 0;
  PREDATOR_PERCENTAGE = 0;

  delete defaultPrey;
  delete defaultPredator;

  for (int i = 0; i < WIDTH * HEIGHT; ++ i) {
    delete world[i];
    delete worldAux[i];
  }
  delete[] world;
  delete[] worldAux;

  delete[] pixels;
}
