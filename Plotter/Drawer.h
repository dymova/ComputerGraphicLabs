#pragma once
#include "Lemniscate.h"
#include <QObject>

class Drawer : public QObject
{
	Q_OBJECT
private:
        const int BYTES_IN_PIXEL = 3;

public:
	Drawer(QObject *parent = 0);
	void drawLemniscate(QImage* pBackBuffer, Lemniscate* c);
    void drawFocus(QImage* pBackBuffer, int y0, int x0);
    void drawAxis(QImage* pBackBuffer);
	void drawPoint(QImage* image, int x, int y, QColor c);

    QPair<int, int> getFirstPoint(Lemniscate* l);
};

