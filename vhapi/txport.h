#ifndef TXPORT_H
#define TXPORT_H

#include <QObject>

class TxPort : public QObject
{
    Q_OBJECT
public:
    explicit TxPort(QObject *parent = nullptr);

signals:
    void frameStarted();
    void substreamItemReceived(const QVariant &item);
    void frameEnded(const QVariantList &items);

public slots:
};

#endif // TXPORT_H
