#include "Creature.hpp"

int mutate(int k) {
  return std::max(k + rand() % 3 - 1, 1);
}

//==================================================  Creature  ==================================================

int Creature::howMany = 0;

Creature::Creature(CreatureType type, int health, int maxHealth, int healthTick, bool ill):
  type(type), health(health), maxHealth(maxHealth), healthTick(mutate(healthTick)), ill(ill) {
  ++ howMany;
}  

int Creature::getCount() {
  return Creature::howMany - 2;
}

bool Creature::isDead() const {
  return health <= 0;
}

bool Creature::isIll() const {
  return ill;
}

bool Creature::canReproduce() const {
  return health >= maxHealth;
}

void Creature::makeIll() {
  ill = true;
}

int Creature::getHealth() const {
  return health;
}

void Creature::updateHealth(int toAdd) {
  health += toAdd;
  if (health > maxHealth) {
    health = maxHealth;
  }
}

Creature::~Creature() {
  -- howMany;
}

std::ostream& operator << (std::ostream &os, Creature &c) {
  os << "Maximum health: " << c.maxHealth <<  '\n';
  os << "Health tic: " << c.healthTick <<  '\n';
  return os;
}

//==================================================  Prey  ==================================================

Prey::Prey():
  Creature(CreatureType::PREY, 1, defaultInstance->maxHealth,
      defaultInstance->healthTick, defaultInstance->ill) {}

Prey::Prey(int maxHealth, int healthTick):
  Creature(CreatureType::PREY, 1, maxHealth, healthTick, false) {}

Prey* Prey::defaultInstance = nullptr;

void Prey::reset() {
  health = 1;
  maxHealth = defaultInstance->maxHealth;
  healthTick = defaultInstance->healthTick;
  ill = defaultInstance->ill;
}

void Prey::updateHealth() {
  health += (-2 * (int)ill + 1) * healthTick;
  if (health > maxHealth) {
    health = maxHealth;
  }
}

void Prey::resetHealth() {
  health = 1;
}

//==================================================  Predator  ==================================================

Predator::Predator():
  Creature(CreatureType::PREDATOR, defaultInstance->maxHealth, defaultInstance->maxHealth,
      defaultInstance->healthTick, defaultInstance->ill) {}

Predator::Predator(int maxHealth, int healthTick):
  Creature(CreatureType::PREDATOR, maxHealth, maxHealth, healthTick, false) {}

Predator* Predator::defaultInstance = nullptr;

void Predator::reset() {
  health = defaultInstance->maxHealth;
  maxHealth = defaultInstance->maxHealth;
  healthTick = defaultInstance->healthTick;
  ill = defaultInstance->ill;
}

void Predator::updateHealth() {
  health -= healthTick;
}

void Predator::resetHealth() {
  health = maxHealth;
}
