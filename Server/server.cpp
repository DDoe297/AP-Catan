#include "server.hpp"

void Server::incomingConnection(qintptr socketDescriptor)
{
    if(players<=3){
    qDebug() << socketDescriptor << " Connecting...";
    Worker *worker=new Worker(players++,socketDescriptor);
    QThread *workerThread=new QThread;
    workerThreads.append(workerThread);
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this,SIGNAL(write(QByteArray)),worker,SLOT(writeToSocket(QByteArray)));
    connect(worker,SIGNAL(readFromSocket(QByteArray,qintptr)),this,SLOT(read(QByteArray,qintptr)));
    connect(worker,SIGNAL(readFromSocket(QByteArray,qintptr)),parent(),SLOT(gameTurn()));
    connect(worker,SIGNAL(sendPlayerName(QString)),parent(),SLOT(addToPlayerNames(QString)));
    connect(worker,SIGNAL(startGame()),parent(),SLOT(prepareGame()));
    connect(this, &Server::run, worker, &Worker::start);
    workerThread->start();
    emit run();
    }
}

QQueue<QJsonObject> &Server::getCommandQueue()
{
    return commandQueue;
}

Server::Server(QObject *parent) : QTcpServer(parent)
{
    players=0;
}

Server::~Server()
{
    for(auto &workerThread:workerThreads){
        workerThread->quit();
        workerThread->wait();
        delete workerThread;
    }
}

void Server::startServer()
{  int port = 9000;
    if (!this->listen(QHostAddress::Any, port)) {
      qDebug() << "Could not start server";
    } else {
      qDebug() << "Listening to port " << port << "...";
    }
}

void Server::read(QByteArray data, qintptr descriptor)
{
    QJsonObject json=QJsonDocument::fromJson(data).object();
    commandQueue.enqueue(json);
}