#ifndef ENTITYPLAYGROUNDVHAPI_H
#define ENTITYPLAYGROUNDVHAPI_H

#include <QObject>

class EntityPlaygroundVhApi : public QObject
{
    Q_OBJECT
public:
    explicit EntityPlaygroundVhApi(QObject *parent = nullptr);

signals:
    // flat ioports layout test below:
    void rxentitiesFrameStarted();
    void rxentitiesFrameEnded(const QVariantList &items);
    void rxentitiesSubstreamItemReceived(const QVariant &item);

    void txentitiesFrameStarted();
    void txentitiesFrameEnded(const QVariantList &items);
    void txentitiesSubstreamItemReceived(const QVariant &item);

public slots:
};

#endif // ENTITYPLAYGROUNDVHAPI_H
