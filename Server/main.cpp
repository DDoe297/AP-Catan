#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "manager.hpp"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  Manager *manager = new Manager();
  manager->startServer();
  return a.exec();
}
