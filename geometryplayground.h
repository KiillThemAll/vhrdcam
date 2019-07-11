#ifndef GEOMETRYPLAYGROUND_H
#define GEOMETRYPLAYGROUND_H

#include <QObject>
#include "vhapi/d2entityrendererentitiesioportvhapi.h"

class ThickLine;
class ThickLineGeometry;


class GeometryPlayground : public QObject
{
    Q_OBJECT
public:
    explicit GeometryPlayground(QObject *parent = nullptr);

    Q_INVOKABLE void setTargetThickLine(ThickLine *line);

    D2EntityRendererEntitiesIOPortVHAPI *rxport;

signals:

public slots:
    void onDrawSmthClicked();
    void onDrawPlusClicked();

private:
    ThickLine *m_line;
    ThickLineGeometry *m_geometry;
};

#endif // GEOMETRYPLAYGROUND_H
