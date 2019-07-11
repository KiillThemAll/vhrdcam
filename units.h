#ifndef UNITS_H
#define UNITS_H

//#include <Eigen/Dense>

//using Eigen::Matrix;
//typedef Matrix<float, 2, Eigen::Dynamic> PathX2f;
//typedef Matrix<float, 2, 1> Point2f;

#include <QQmlEngine>

// How many meters in one opengl unit
const float Unit2Meters = 10.0f;

class Units : public QObject
{
    Q_OBJECT
public:
    explicit Units(QObject *parent = nullptr);
    virtual ~Units();

    Q_INVOKABLE static float meters(float x);
    Q_INVOKABLE static float cm(float x);
    Q_INVOKABLE static float mm(float x);

    Q_INVOKABLE static float tometers(float units);
    Q_INVOKABLE static float tocm(float units);
    Q_INVOKABLE static float tomm(float units);
};

static QObject *qobject_units_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Units *units = new Units();
    return units;
}

#endif // UNITS_H
