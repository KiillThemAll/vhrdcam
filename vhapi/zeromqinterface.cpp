#include "zeromqinterface.h"
#include "zeromqinterfaceworker.h"

#include <QThread>
#include <QDebug>

ZeroMQInterface::ZeroMQInterface(QObject *parent) : QObject(parent)
{
    m_worker = new ZeroMQInterfaceWorker();
    m_workerThread = new QThread(this);
    m_worker->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::started,
            m_worker, &ZeroMQInterfaceWorker::run);
    connect(m_worker, &ZeroMQInterfaceWorker::finished,
            m_workerThread, &QThread::quit);
    connect(m_worker, &ZeroMQInterfaceWorker::finished,
            this, &ZeroMQInterface::stopped);
    connect(m_worker, &ZeroMQInterfaceWorker::finished,
            m_worker, &ZeroMQInterfaceWorker::deleteLater);
    connect(m_worker, &ZeroMQInterfaceWorker::finished,
            m_workerThread, &QThread::deleteLater);
    qDebug() << "Starting ZeroMQ Worker...";
    m_workerThread->start();
}

ZeroMQInterfaceWorker *ZeroMQInterface::worker() const
{
    return m_worker;
}

void ZeroMQInterface::stop()
{
    QMetaObject::invokeMethod(m_worker, &ZeroMQInterfaceWorker::stop, Qt::QueuedConnection);
}
