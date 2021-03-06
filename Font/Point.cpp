#include "Point.h"
#include "Utils.h"

double Point::getX() const
{
    return x;
}

double Point::getY() const
{
    return y;
}

bool Point::isOncurve() const
{
    return oncurve;
}


void Point::setX(double value)
{
    x = value;
}

void Point::setY(double value)
{
    y = value;
}

Point::Point()
{
    
}

Point::Point(double x, double y, bool oncurve) : x(x), y(y), oncurve(oncurve)
{
}

void Point::read(const QJsonObject &json)
{
    QJsonValue oncurveValue = json[KEY_ONCURVE];
    Utils::checkValue(KEY_ONCURVE, oncurveValue);
    oncurve = oncurveValue.toBool();

    QJsonValue positionValue = json[KEY_POSITION];
    Utils::checkValue(KEY_POSITION, positionValue);
    QJsonObject jsonPosition = positionValue.toObject();

    QJsonValue xValue = jsonPosition[KEY_X];
    Utils::checkValue(KEY_X, xValue);
    x = xValue.toInt();

    QJsonValue yValue = jsonPosition[KEY_Y];
    Utils::checkValue(KEY_Y, yValue);
    y = yValue.toInt();
}

void Point::write(QJsonObject &json) const
{
    QJsonObject position;
    position[KEY_X] = x;
    position[KEY_Y] = y;
    json[KEY_POSITION] = position;
    json[KEY_ONCURVE] = oncurve;
}
