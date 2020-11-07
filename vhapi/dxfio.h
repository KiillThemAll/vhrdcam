#ifndef DXFIO_H
#define DXFIO_H

#include <QObject>
#include <QHash>
#include <QUrl>
#include "vhapi/txport.h"

class DxfIOEntitiesTxPort;

// remote Block, only api, unlike d2entityrenderer which have local implementation
class DxfIO : public QObject
{
    Q_OBJECT
public:
    explicit DxfIO(QObject *parent = nullptr);

    QHash<QString, TxPort *> txports() const;

signals:
    void serialized(const QString &destination, const QByteArray &block);

public slots:
    void syncReceived(const QByteArray &marker);
    void multipartReceived(const QByteArray &marker, const QByteArray &block); // hide somehow?
    void load(const QUrl &url);

private:
    QHash<QString, TxPort *> m_txports;
    DxfIOEntitiesTxPort *m_entitiesTxPort;
};

#endif // DXFIO_H
