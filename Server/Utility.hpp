#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <QVector>
#include <algorithm>
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
  auto rd = std::random_device{};
  auto rng = std::default_random_engine{rd()};
  std::shuffle(a.begin(), a.end(), rng);
}

int randomNumber(int start, int end);
#endif  // UTILITY_HPP
