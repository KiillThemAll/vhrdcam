#ifndef ZEROMQINTERFACEWORKER_H
#define ZEROMQINTERFACEWORKER_H

#include <QObject>
#include <QList>

struct zmq_worker_txitem
{
    void *socket;
    QString ioport;
    QByteArray data;
};

class ZeroMQInterfaceWorker : public QObject
{
    Q_OBJECT
public:
    explicit ZeroMQInterfaceWorker(QObject *parent = nullptr);

signals:
    void finished();
    //void error();

    // should be dynamic signals in dynamic QObject returned by dqobject()?
    // no dq needed is QVariant's are passed around
    // source is ioport (signals and slots can be multiplexed here too?)
    // name of DxfIO block, blocks themselves are separated across different zmq sub sockets
    // source io port
    void syncFromDxfIOReceived(const QByteArray &marker);
    void multipartFromDxfIOReceived(const QByteArray &marker, const QByteArray &block);

public slots:
    void run();
    void stop();

    // same here, to dq
    void sendBlockToDxfIO(const QString &destination, const QByteArray &block);

private:
    void handleIoPortSubscription(void *subsocket);
    bool m_running;
    QList<zmq_worker_txitem> m_txqueue;
};

#endif // ZEROMQINTERFACEWORKER_H
