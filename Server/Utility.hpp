#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <QVector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <random>

template <typename T>
void append(QVector<T>& a, const QVector<T>& b) {
  for (auto element : b) {
    a.append(element);
  }
}
template <typename T>
void shuffle(QVector<T>& a) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);
  std::shuffle(a.begin(), a.end(), e);
}

int randomNumber(int start, int end);
#endif  // UTILITY_HPP
