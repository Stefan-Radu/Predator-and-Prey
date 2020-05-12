#pragma once

#include <iostream>

namespace stef {

  template < typename T >
  class vector {
  public:

    // default constructor
    vector (): containerSize(0), containerCapacity(0)  {}

    // constructor with size
    vector (unsigned int size): containerSize(size), containerCapacity(size) {
      container = new T[size]();
    }

    // constructor with size and default value
    vector (unsigned int size, const T &value): containerSize(size), containerCapacity(size) {
      container = new T[size]();
      for (unsigned int i = 0; i < size; ++ i) {
        container[i] = value;
      }
    }

    vector (const vector& v): containerSize(v.containerSize), containerCapacity(v.containerCapacity) {
      container = new T[containerSize]();
      for (unsigned int i = 0; i < containerSize; ++ i) {
        container[i] = v[i];
      }
    }

    vector& operator= (vector v) {
      swap(*this, v);
      return *this;
    }

    ~vector() {
      containerSize = 0;
      containerCapacity = 0;
      delete[] container;
    }

    T* begin() const {
      return &container[0];
    }

    T* end() const {
      return &container[containerSize];
    };

    unsigned int size() const {
      return containerSize;
    }

    unsigned int capacity() const {
      return containerCapacity;
    }

    bool empty() const {
      return containerSize == 0;
    }

    T& operator[] (unsigned int index) const {
      return container[index];
    }

    T& first() const {
      return container[0];
    };

    T& back() const {
      return container[containerSize - 1];
    }

    void push_back(const T& value) {

      if (containerSize == containerCapacity) {
        containerCapacity = containerCapacity << 1 | 1;
        T *aux = new T[containerCapacity]();
        for (unsigned int i = 0; i < containerSize; ++ i) {
          aux[i] = container[i];
        }
        std::swap(container, aux);
        delete[] aux;
      }

      container[containerSize ++] = value; 
    }

    void pop_back() {
      -- containerSize;
    };

    void clear() {
      containerSize = 0;
    };

  private:

    unsigned int containerSize;
    unsigned int containerCapacity;
    T *container;

    friend void swap(vector &v1, vector &v2) {
      using std::swap;
      swap(v1.containerSize, v2.containerSize);
      swap(v1.containerCapacity, v2.containerCapacity);
      swap(v1.container, v2.container);
    }
  };
}
