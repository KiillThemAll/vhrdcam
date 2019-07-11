#ifndef ENTITYPLAYGROUND_H
#define ENTITYPLAYGROUND_H

#include <QObject>
#include "vhapi/2dentities/pointentity.hpp"

class EntityPlaygroundVhApi;

class EntityPlayground : public QObject
{
    Q_OBJECT
public:
    explicit EntityPlayground(QObject *parent = nullptr);

    EntityPlaygroundVhApi *api() const;

signals:

private slots:
    void onEntity(const QVariant &entity);

private:
    EntityPlaygroundVhApi *m_api;

    void calculateLeads(const QList<PointEntity> &moves, PointEntity &leadIn, PointEntity &leadOut);
};

#endif // ENTITYPLAYGROUND_H
