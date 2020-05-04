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


private:
    Ui::MainWindow *ui;
    QTimer *timer;
    vector<line*> doprava;
    GraphicScene* scene;
    void update_traf();
};

#endif // MAINWINDOW_H
