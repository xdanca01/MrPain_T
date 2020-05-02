#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include "line.h"
#include "mainwindow.h"
#include <QMainWindow>
#include <math.h>

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicScene(QObject *parent = 0);
    bool showLine(line* linka);
    void busses(bus* b);


signals:

public slots:
};

#endif // GRAPHICSCENE_H
