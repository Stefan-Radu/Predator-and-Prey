#pragma once

#include <list>
#include <iostream>
#include "Creature.hpp"

template < typename T >
class ObjectPool {
public:

  ObjectPool(const ObjectPool&) = delete;
  ObjectPool& operator = (const ObjectPool&) = delete;

  static ObjectPool& getInstance() {
    static ObjectPool instance = ObjectPool();
    return instance;
  }

  T* getResource() {
    if (objects.empty()) {
      return new T();
    }

    T *object = objects.front();
    objects.pop_front();

    object->reset();
    return object;
  }

  void returnResource(T *object) {
    objects.push_back(object);
  }

  ~ObjectPool() {
    for (auto &p : objects) {
      delete p;
    }
    objects.clear();
  }

private:

  ObjectPool() {};

  std::list < T* > objects;
};
