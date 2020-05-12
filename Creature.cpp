#include "Creature.hpp"

int mutate(int k) {
  return std::max(k + rand() % 3 - 1, 1);
}

//==================================================  Creature  ==================================================

int Creature::howMany = 0;

Creature::Creature(CreatureType type, bool ill, int health, int maxHealth, int healthTick):
  type(type), ill(ill), health(health), maxHealth(maxHealth), healthTick(mutate(healthTick)) {
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
  Creature(CreatureType::PREY, defaultInstance->ill, 1, defaultInstance->maxHealth,
      defaultInstance->healthTick) {}

Prey::Prey(int maxHealth, int healthTick):
  Creature(CreatureType::PREY, false, 1, maxHealth, healthTick) {}

Prey* Prey::defaultInstance = nullptr;

void Prey::reset() {
  ill = defaultInstance->ill;
  health = 1;
  maxHealth = defaultInstance->maxHealth;
  healthTick = defaultInstance->healthTick;
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
  Creature(CreatureType::PREDATOR, defaultInstance->ill, defaultInstance->maxHealth, 
      defaultInstance->maxHealth, defaultInstance->healthTick) {}

Predator::Predator(int maxHealth, int healthTick):
  Creature(CreatureType::PREDATOR, false, maxHealth, maxHealth, healthTick) {}

Predator* Predator::defaultInstance = nullptr;

void Predator::reset() {
  ill = defaultInstance->ill;
  health = defaultInstance->maxHealth;
  maxHealth = defaultInstance->maxHealth;
  healthTick = defaultInstance->healthTick;
}

void Predator::updateHealth() {
  health -= healthTick;
}

void Predator::resetHealth() {
  health = maxHealth;
}
