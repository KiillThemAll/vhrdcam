#include "entityplayground.h"
#include "vhapi/entityplaygroundvhapi.h"

#include "vhapi/2dentities/groupentity.h"
#include "vhapi/2dentities/polylineentity.h"
#include "vhapi/2dentities/curveentity.h"
#include "vhapi/2dentities/markerentity.hpp"
#include "vhapi/2dentities/shapeentity.h"

#include <QVector2D>

#include <QFile>
#include <QTime>

EntityPlayground::EntityPlayground(QObject *parent) : QObject(parent)
{
    m_api = new EntityPlaygroundVhApi(this);
    connect(m_api, &EntityPlaygroundVhApi::rxentitiesSubstreamItemReceived,
            this, &EntityPlayground::onEntity);
}

EntityPlaygroundVhApi *EntityPlayground::api() const
{
    return m_api;
}

void EntityPlayground::onEntity(const QVariant &entity)
{
    if (entity.canConvert<MarkerEntity>()) {
        qDebug() << "EntityPlayground::onEntity: propagating own marker";
         MarkerEntity marker = entity.value<MarkerEntity>();
         if (!marker.m_metadata.contains("end")) {
             MarkerEntity marker;
             marker.m_metadata["source"] = "entity_playground";
             QVariant v;
             v.setValue<MarkerEntity>(marker);
             emit m_api->txentitiesFrameStarted();
             emit m_api->txentitiesSubstreamItemReceived(v);
         }
         return;
    }
    if (!entity.canConvert<GroupEntity>())
        return;
    qDebug() << "EntityPlayground: group found";

    QList<CurveEntity> closedCurves;
    GroupEntity group = entity.value<GroupEntity>();
    foreach (const QVariant &e, group.m_entities) {
        CurveEntity curve;
        curve.addCurve(e);
        if (curve.isValid() && curve.isClosed())
            closedCurves.append(curve);
    }

    qDebug() << "EntityPlayground: found" << closedCurves.length() << "closed curves";



// Render bounding boxes for curves
//    foreach (const CurveEntity &curve, closedCurves) {
//        QRectF box = curve.boundingBox();

//        PolylineEntity polyline;
//        polyline.setColor(QColor(Qt::magenta));
//        polyline.setThickness(1);
//        polyline.addPoint(box.topLeft());
//        polyline.addPoint(box.topRight());
//        polyline.addPoint(box.bottomRight());
//        polyline.addPoint(box.bottomLeft());
//        polyline.addPoint(box.topLeft());

//        QVariant v;
//        v.setValue<PolylineEntity>(polyline);
//        emit m_api->txentitiesSubstreamItemReceived(v);
//    }

    QList<ShapeEntity> shapes;
    while (!closedCurves.isEmpty()) {
        CurveEntity curve0 = closedCurves.takeFirst();
        ShapeEntity shape;
        shape.addCurve(curve0);
        QMutableListIterator<CurveEntity> it(closedCurves);
        while (it.hasNext()) {
            it.next();
            if (curve0.boundingBox().contains(it.value().boundingBox())) {
                shape.addCurve(it.value());
                it.remove();
            }
        }
        shapes.append(shape);
    }
    qDebug() << "EntityPlayground: found" << shapes.length() << "shapes";
    if (shapes.isEmpty())
        return;

    QString time = QTime::currentTime().toString("hh-mm-ss-zzz");
    QString gcodeFilename = "/Volumes/Red Volume/out" + time + ".gcode";
    QFile f(gcodeFilename);
    qDebug() << "Opening: " << gcodeFilename << f.open(QIODevice::WriteOnly);
    QTextStream gcode(&f);
    gcode.setRealNumberNotation(QTextStream::FixedNotation);
    gcode.setRealNumberPrecision(4);

    gcode << "G90 G21" << endl;


    // Render bounding boxes for shapes
//    foreach (const ShapeEntity &shape, shapes) {
//        QRectF box = shape.boundingBox().marginsAdded(QMarginsF(5, 5, 5, 5));

//        PolylineEntity polyline;
//        polyline.setColor(QColor(Qt::magenta));
//        polyline.setThickness(1);
//        polyline.setRect(box);

//        QVariant v;
//        v.setValue<PolylineEntity>(polyline);
//        emit m_api->txentitiesSubstreamItemReceived(v);
//    }

    float minx = 1000000.0f;
    float maxx = -1000000.0f;
    float miny = 1000000.0f;
    float maxy = -1000000.0f;

    foreach (const ShapeEntity &shape, shapes) {
        QRectF box = shape.boundingBox();
        if (box.bottom() > maxy)
            maxy = box.bottom();
        if (box.top() < miny)
            miny = box.top();
        if (box.right() > maxx)
            maxx = box.right();
        if (box.left() < minx)
            minx = box.left();
    }

    QRectF allShapesBox = QRectF(minx, miny, maxx - minx, maxy - miny);
    // Draw all shapes box
//    PolylineEntity polyline;
//    polyline.setColor(QColor(Qt::cyan));
//    polyline.setThickness(1);
//    polyline.setRect(allShapesBox.marginsAdded(QMarginsF(10, 10, 10, 10)));
//    v.setValue<PolylineEntity>(polyline);
//    emit m_api->txentitiesSubstreamItemReceived(v);

    PointEntity a(allShapesBox.bottomLeft().x(), allShapesBox.bottomLeft().y(), 0.0f);
    PointEntity b(allShapesBox.topLeft().x(), allShapesBox.topLeft().y(), 0.0f);
    float dx = 0.05;
    bool goingUp = true;

    PolylineEntity pl;
    pl.setThickness(1);
    PointEntity lastPosition(0.0f, 0.0f, 0.0f);
    pl.setColor(QColor(Qt::magenta));
    pl.addPoint(lastPosition);

    float cum_dx = 0;
    while (allShapesBox.right() - a.m_x > -0.1f) {
        LineEntity line;
        line.m_a = a;
        line.m_b = b;
        QList<PointEntity> points;
        foreach (const ShapeEntity &shape, shapes) {
            points << shape.intersect(line);
        }

        std::sort(points.begin(), points.end(), [goingUp](PointEntity &a, PointEntity &b) { return goingUp ? a.m_y < b.m_y : a.m_y > b.m_y; } );
        //qDebug() << "points pre" << points;
        QMutableListIterator<PointEntity> it(points);
        while (it.hasNext()) {
            PointEntity current = it.next();
            if (it.hasNext()) {
                PointEntity next = it.next();
                if (current == next) {
                    it.remove();
                    it.previous();
                    it.remove();
                }
            }
        }
        //qDebug() << "points after" << points;
        if (points.isEmpty()) {
            a.m_x += dx;
            b.m_x += dx;
            continue;
        }

        PointEntity leadIn;
        PointEntity leadOut;
        calculateLeads(points, leadIn, leadOut);

        pl.setColor(QColor(Qt::blue)); gcode << "G0 ";
        pl.addPoint(leadIn); gcode << "X" << leadIn.m_x << " Y" << leadIn.m_y << endl;
        pl.setColor(QColor(Qt::red)); gcode << "G0 ";
        pl.addPoint(points.first()); gcode << "X" << points.first().m_x << " Y" << points.first().m_y << endl;

        bool cutMove = false;
        for (qint32 i = 1; i < points.length() - 1; i += 1) {
            if (points[i] == points[i + 1])
                continue;

            pl.setColor(cutMove ? QColor(Qt::red) : QColor(Qt::cyan));
            gcode << (cutMove ? "G0 " : "G1 ");
            pl.addPoint(points[i]); gcode << "X" << points[i].m_x << " Y" << points[i].m_y << endl;

            cutMove = !cutMove;

            if (i == points.length() - 2) { // last point
                pl.setColor(QColor(Qt::yellow));
                gcode << "G1 ";
            } else {
                pl.setColor(cutMove ? QColor(Qt::red) : QColor(Qt::cyan));
                gcode << (cutMove ? "G0 " : "G1 ");
            }
            pl.addPoint(points[i + 1]); gcode << "X" << points[i + 1].m_x << " Y" << points[i + 1].m_y << endl;
        }

        if (points.length() == 2) {
            pl.setColor(QColor(Qt::yellow)); gcode << "G1 ";
            pl.addPoint(points[1]); gcode << "X" << points[1].m_x << " Y" << points[1].m_y << endl;
        }
        pl.setColor(QColor(Qt::magenta)); gcode << "G0 ";
        pl.addPoint(leadOut); gcode << "X" << leadOut.m_x << " Y" << leadOut.m_y << endl << endl;
        lastPosition = leadOut;

        a.m_x += dx;
        b.m_x += dx;
        cum_dx += dx;
        if (cum_dx > 3) {
            qDebug() << a.m_x << allShapesBox.right() - a.m_x;
            cum_dx = 0;
        }

        goingUp = !goingUp;
    }

    QVariant v;
    v.setValue<PolylineEntity>(pl);
    emit m_api->txentitiesSubstreamItemReceived(v);

    MarkerEntity marker;
    marker.m_metadata["source"] = "entity_playground";
    marker.m_metadata.insert("end", QVariant());
    v.setValue<MarkerEntity>(marker);
    emit m_api->txentitiesSubstreamItemReceived(v);

    //emit m_api->txentitiesFrameEnded(); // m_api should collect items himself
}

void EntityPlayground::calculateLeads(const QList<PointEntity> &moves, PointEntity &leadIn, PointEntity &leadOut)
{
    if (moves.length() < 2)
        return;
    const PointEntity &first = moves.first();
    const PointEntity &last = moves.last();
    QVector2D direction(last.m_x - first.m_x, last.m_y - first.m_y);
    direction.normalize();
    direction *= -10.0f;
    leadIn = PointEntity(first.m_x + direction.x(),
                              first.m_y + direction.y(),
                              0.0f);
    direction *= -1;
    leadOut = PointEntity(last.m_x + direction.x(),
                             last.m_y + direction.y(),
                             0.0f);
}