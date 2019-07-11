#include "zeromqinterfaceworker.h"

#include "zmq.h"

#include <QCoreApplication>
#include <QDebug>

ZeroMQInterfaceWorker::ZeroMQInterfaceWorker(QObject *parent) : QObject(parent),
    m_running(false)
{
}

void ZeroMQInterfaceWorker::handleIoPortSubscription(void *subsocket)
{
    zmq_msg_t msg;
    int rc = zmq_msg_init(&msg);
    if (rc != 0) {
        return;
    }
    int size = zmq_msg_recv(&msg, subsocket, 0);
    QByteArray marker;
    if (size != -1) {
        marker = QByteArray((const char *)zmq_msg_data(&msg), size);
    }
    zmq_msg_close(&msg);
    if (!zmq_msg_more(&msg)) {
        qDebug() << "non mp, sync";
        emit syncFromDxfIOReceived(marker);
        return;
    }

    rc = zmq_msg_init(&msg);
    if (rc != 0) {
        return;
    }
    size = zmq_msg_recv(&msg, subsocket, 0);
    QByteArray block;
    if (size != -1) {
        block = QByteArray((const char *)zmq_msg_data(&msg), size);
        emit multipartFromDxfIOReceived(marker, block);
    }
    zmq_msg_close(&msg);
}

void ZeroMQInterfaceWorker::run()
{
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");

    void *subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5556");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "a", 1);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "entities", 1);

    m_running = true;
    while (m_running) {
        zmq_msg_t msg;

        while (!m_txqueue.isEmpty()) {
            qDebug() << "zmq tx queue send";
            zmq_worker_txitem txitem = m_txqueue.front();
            m_txqueue.removeFirst();
            // hack, need bidirectional pub/sub, send as req for now
            zmq_send(requester, txitem.data.data(), txitem.data.length(), 0);
        }

        zmq_pollitem_t items[] = {
            { requester,    0,  ZMQ_POLLIN, 0 },
            { subscriber,   0,  ZMQ_POLLIN, 0 }
        };
        zmq_poll(items, 2, 1); // 1ms poll or 0 wait?

        if (items[0].revents & ZMQ_POLLIN) {
            int rc = zmq_msg_init(&msg);
            int size = zmq_msg_recv(&msg, requester, 0);
            if (size != -1) {
                QByteArray resp((const char *)zmq_msg_data(&msg), size);
                qDebug() << "response:" << resp;
                zmq_msg_close(&msg);
            }
        }

        if (items[1].revents & ZMQ_POLLIN) {
            handleIoPortSubscription(subscriber);
        }

        QCoreApplication::processEvents();
    }
    zmq_close(requester);
    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    qDebug() << "ZeroMQ Worker about to finish";
    emit finished();
}

void ZeroMQInterfaceWorker::stop()
{
    m_running = false;
}

void ZeroMQInterfaceWorker::sendBlockToDxfIO(const QString &destination, const QByteArray &block)
{
    // need registry containing sockets associated with remote blocks
    m_txqueue.append( { 0, destination, block } );
}
