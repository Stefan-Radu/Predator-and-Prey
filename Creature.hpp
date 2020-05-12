#pragma once

#include <iostream>

int mutate(int k);

enum class CreatureType { PREDATOR, PREY, NOTHING };

class Creature {
public:

  Creature(CreatureType, bool, int, int, int);
  virtual ~Creature();

  const CreatureType type; 
  static int getCount();

  void updateHealth(int);
  void makeIll();
  int getHealth() const;
  bool isDead() const; 
  bool isIll() const;
  bool canReproduce() const;

  virtual void reset() = 0;
  virtual void updateHealth() = 0;
  virtual void resetHealth() = 0;

protected:

  bool ill;
  int health, maxHealth, healthTick;

  friend std::ostream& operator << (std::ostream&, Creature&);

private:
  static int howMany;
};

class Prey: public Creature {
public:

  Prey();
  Prey(int, int);

  static Prey* defaultInstance;

  void reset() override;
  void updateHealth() override;
  void resetHealth() override;
};

class Predator: public Creature {
public:

  Predator();
  Predator(int, int);

  static Predator* defaultInstance;

  void reset() override;
  void updateHealth() override;
  void resetHealth() override;
};
