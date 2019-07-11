#ifndef RXPORT_H
#define RXPORT_H

#include <QObject>

class RxPort : public QObject
{
    Q_OBJECT
public:
    explicit RxPort(QObject *parent = nullptr);

    // VhApi vhapi() const;

signals:
    void frameStarted();
    void substreamItemReceived(const QVariant &item);
    void frameEnded();

public slots:
};

#endif // RXPORT_H
