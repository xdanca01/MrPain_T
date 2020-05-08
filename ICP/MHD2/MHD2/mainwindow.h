#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsLineItem>
#include <QTime>
#include "graphicscene.h"
#include "line.h"
#include "bus.h"
#include <QTimer>
#include <vector>
#include <iostream>
#include <QDebug>
#include <optional>
#include <QFileDialog>
#include "csv.h"

class GraphicScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void plus();
    void minus();
    void Zoom(int);
    void left();
    void right();
    void speedos(double);
    void play();
    void tick();
    void stop();
    void addStops();
    void addStreets();
    void addBus();
    void addTraf();
    void addLine();
    void quit();
    //zkontroluje jaká ulice je selectnutá pro změnu hodnoty delaye
    void check_sel(int);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    vector<line*> doprava;
    vector<Stop*> stops;
    vector<Street*> streets;
    GraphicScene* scene;
    //hlavní funkce pro update dopravy za tick
    void update_traf();
    vector<Stop*> getStops();
    vector<Street*> getStreets();
};

#endif // MAINWINDOW_H
