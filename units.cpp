#include "units.h"

Units::Units(QObject *parent) : QObject (parent)
{
}

Units::~Units()
{
}

float Units::meters(float x)
{
    return x / Unit2Meters;
}

float Units::cm(float x)
{
    return x * 100.0f / Unit2Meters;
}

float Units::mm(float x)
{
    return x * 1000.0f / Unit2Meters;
}

float Units::tometers(float units)
{
    return units * Unit2Meters;
}

float Units::tocm(float units)
{
    return units * Unit2Meters * 100.0f;;
}

float Units::tomm(float units)
{
    return units * Unit2Meters * 1000.0f;
}

