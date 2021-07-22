#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <ctime>
#include <random>
#include "manager.hpp"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  srand(time(NULL));
  Manager manager;
  manager.startServer();
  return a.exec();
}
