#include "server.hpp"

void Server::incomingConnection(qintptr socketDescriptor) {
  if (players <= 3) {
    qDebug() << socketDescriptor << " Connecting...";
    Worker *worker = new Worker(players++, socketDescriptor);
    workers.append(worker);
    QThread *workerThread = new QThread;
    workerThreads.append(workerThread);
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, SIGNAL(write(QByteArray)), worker,
            SLOT(writeToSocket(QByteArray)));
    connect(worker, SIGNAL(readFromSocket(const QByteArray)), this,
            SLOT(read(const QByteArray)));
    connect(worker, SIGNAL(readFromSocket(const QByteArray)), parent(),
            SLOT(gameTurn()));
    connect(worker, SIGNAL(sendPlayerName(QString)), parent(),
            SLOT(addToPlayerNames(QString)));
    connect(worker, SIGNAL(startGame()), parent(), SLOT(prepareGame()));
    connect(this, &Server::run, worker, &Worker::start);
    workerThread->start();
    emit run();
  }
}

QQueue<QJsonObject> &Server::getCommandQueue() { return commandQueue; }

Server::Server(QObject *parent) : QTcpServer(parent) { players = 0; }

Server::~Server() {
  for (auto &workerThread : workerThreads) {
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
  }
  for(auto worker:workers){
      delete worker;
  }
}

void Server::startServer() {
  int port = 9000;
  if (!this->listen(QHostAddress::Any, port)) {
    qDebug() << "Could not start server";
  } else {
    qDebug() << "Listening to port " << port << "...";
  }
}

void Server::read(const QByteArray data) {
  QJsonObject json = QJsonDocument::fromJson(data).object();
  commandQueue.enqueue(json);
}
