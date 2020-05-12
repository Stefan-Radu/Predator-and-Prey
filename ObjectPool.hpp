#pragma once

#include <vector>
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

    T *object = objects.back();
    objects.pop_back();

    object->reset();
    return object;
  }

  void returnResource(T *object) {
    objects.emplace_back(object);
  }

  ~ObjectPool() {
    for (auto &p : objects) {
      delete p;
    }
    objects.clear();
  }

private:

  ObjectPool() {};

  std::vector < T* > objects;
};
