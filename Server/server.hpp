#ifndef SERVER_HPP
#define SERVER_HPP

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include "worker.hpp"

class Server : public QTcpServer
{
    Q_OBJECT
private:
 void incomingConnection(qintptr socketDescriptor);
 QVector<QThread*> workerThreads;
 int players;
public:
 Server(QObject *parent = 0);
 ~Server();
 void startServer(void);
signals:
 void write(QByteArray);
 void run(void);
public slots:
 void read(QByteArray data,qintptr descriptor);
};

#endif // SERVER_HPP
