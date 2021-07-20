#ifndef SERVER_HPP
#define SERVER_HPP

#include <QObject>
#include <QThread>
#include <QJsonObject>
#include <QQueue>
#include <QTcpServer>
#include <QJsonDocument>
#include "worker.hpp"

class Server : public QTcpServer
{
    Q_OBJECT
private:
 void incomingConnection(qintptr socketDescriptor);
 QVector<QThread*> workerThreads;
 int players;
 QQueue<QJsonObject> commandQueue;
public:
 Server(QObject *parent = 0);
 ~Server();
 void startServer(void);
 QQueue<QJsonObject> &getCommandQueue();

signals:
 void write(QByteArray);
 void run(void);
public slots:
 void read(QByteArray data,qintptr descriptor);
};

#endif // SERVER_HPP
