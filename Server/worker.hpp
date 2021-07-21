#ifndef WORKER_HPP
#define WORKER_HPP

#include <QObject>
#include <QTcpSocket>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(int PlayerID ,qintptr ID, QObject *parent = nullptr);
private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    int playerID;
signals:
    void error(QTcpSocket::SocketError socketerror);
    void readFromSocket(const QByteArray,const qintptr);
    void sendPlayerName(QString);
    void startGame();
public slots:
    void start(void);
    void writeToSocket(QByteArray data);
    void readyRead(void);
    void disconnected(void);
};

#endif // WORKER_HPP
