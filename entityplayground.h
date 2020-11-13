#ifndef ENTITYPLAYGROUND_H
#define ENTITYPLAYGROUND_H

#include <QObject>
#include <QUrl>
#include <QProcess>
#include "vhapi/2dentities/pointentity.hpp"

class EntityPlaygroundVhApi;

class EntityPlayground : public QObject
{
    Q_OBJECT
public:
    explicit EntityPlayground(QObject *parent = nullptr);

    EntityPlaygroundVhApi *api() const;

public slots:
    void exportEngraveFile(const QString &fileName);

private slots:
    void onEntity(const QVariant &entity);
    void onFrameEnded(const QVariant &items);
    //void onReadyRead();

private:
    EntityPlaygroundVhApi *m_api;
    PointEntity m_lastPos;

    void calculateLeads(const QList<PointEntity> &moves, PointEntity &leadIn, PointEntity &leadOut);
    void dumpBox(const QString &filename, const QRectF &box);
};

#endif // ENTITYPLAYGROUND_H
