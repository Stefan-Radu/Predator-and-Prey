#pragma once

#include <list>

template < typename T >
class ObjectPool {
public:

  ObjectPool& getInstance() {
    static ObjectPool instance = ObjectPool();
    return instance;
  }

  T* getResource(const T& refference) {
    if (objects.empty()) {
      return new T(refference);
    }

    T *object = objects.front();
    objects.pop_front();

    object->reset(refference);
    return object;
  }

  void returnResource(T *object) {
    objects.push_back(object);
  }

private:

  ObjectPool() {};

  std::list < T* > objects;
};
