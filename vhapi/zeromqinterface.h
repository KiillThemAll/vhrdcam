#ifndef ZEROMQINTERFACE_H
#define ZEROMQINTERFACE_H

#include <QObject>

class ZeroMQInterfaceWorker;
class QThread;

class ZeroMQInterface : public QObject
{
    Q_OBJECT
public:
    explicit ZeroMQInterface(QObject *parent = nullptr);

    ZeroMQInterfaceWorker *worker() const;

signals:
    void stopped();


public slots:
    void stop();

private:
    ZeroMQInterfaceWorker *m_worker;
    QThread *m_workerThread;
};

#endif // ZEROMQINTERFACE_H
