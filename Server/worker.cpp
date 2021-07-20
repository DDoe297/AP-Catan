#include "worker.hpp"

Worker::Worker(int PlayerID,qintptr ID,QObject *parent) : QObject(parent)
{
    socketDescriptor = ID;
    socket=nullptr;
    playerID=PlayerID;
}

void Worker::start()
{
    if(socket==nullptr){
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(this->socketDescriptor)) {
      emit error(socket->error());
      return;
    }
    socket->write(QString::number(playerID).toStdString().c_str());
    socket->waitForReadyRead();
    QByteArray data = socket->readAll();
    emit sendPlayerName(QString(data));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()),
            Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }
    if(playerID==3){
        emit startGame();
    }
}

void Worker::writeToSocket(QByteArray data)
{
    socket->write(data);
}

void Worker::readyRead()
{
    QByteArray Data = socket->readAll();
    emit readFromSocket(Data,socketDescriptor);
}

void Worker::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
}
