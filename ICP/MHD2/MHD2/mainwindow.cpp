#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_plus, &QPushButton::clicked, this, &MainWindow::plus);
    connect(ui->btn_minus, &QPushButton::clicked, this, &MainWindow::minus);
    connect(ui->slide_zoom, &QSlider::valueChanged, this, &MainWindow::Zoom);
    connect(ui->btn_right, &QPushButton::clicked, this, &MainWindow::right);
    connect(ui->btn_left, &QPushButton::clicked, this, &MainWindow::left);
    connect(ui->my_timer, &QTimeEdit::timeChanged, this, &MainWindow::whats_time);
    connect(ui->btn_speed, SIGNAL(valueChanged(double)), this, SLOT(speedos(double)));
    connect(ui->actionPlay, &QAction::triggered, this, &MainWindow::play);
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::stop);

    scene = new GraphicScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    doprava = new vector<traffic_t*>();
    line* linka = new line("linka 1");
    Coordinate* c1 = new Coordinate(20,20);
    Coordinate* c2 = new Coordinate(-80,-50);
    Coordinate* c3 = new Coordinate(-70,-50);
    Coordinate* c4 = new Coordinate(40,-60);
    Coordinate* c5 = new Coordinate(40,90);
    Coordinate* c6 = new Coordinate(200,80);


    Street* ulice2 = new Street("Tercina",c5,c4);
    Street* ulice3 = new Street("Petrova",c4,c6);
    Street* ulice4 = new Street("Petrova",c5,c6);
    Street* ulice5 = new Street("Petrova",c1,c4);
    Street* ulice6 = new Street("Petrova",c1,c5);

    timer = new QTimer();
    timer->setInterval(1000.0/ui->btn_speed->value());
    connect(timer, &QTimer::timeout, this, &MainWindow::tick);
    time = new QTime();
    *time = ui->my_timer->time();
    Stop* s1 = new Stop("first",c5);
    Stop* s2 = new Stop("second",c1);
    Street* ulice = new Street("Palackeho",c4,c1);
    auto* casy = new vector<QTime*>();
    auto* casy2 = new vector<QTime*>();
    auto* ulic = new vector<Street*>();
    auto* stopky =new vector<Stop*>();
    casy->push_back(new QTime(12,0,0,0));
    casy->push_back(new QTime(12,10,59,0));
    casy2->push_back(new QTime(12,1,0,0));
    casy2->push_back(new QTime(12,11,59,0));
    ulic->push_back(ulice2);
    ulic->push_back(ulice);
    stopky->push_back(s1);
    stopky->push_back(s2);
    bus *b = new bus;
    bus *b2 = new bus;
    b2->update(20.0,20.0);
    b->update(20.0,20.0);
    traffic_t* traff2 = new traffic_t(b2, *ulic,*casy2, *stopky);
    traffic_t* traff = new traffic_t(b,*ulic,*casy,*stopky);
    linka->addBus(traff);
    //linka->addBus(traff2);
    doprava->push_back(traff2);
    doprava->push_back(traff);

    linka->addStreet(ulice);
    linka->addStreet(ulice2);
   /* linka->addStreet(ulice3);
    linka->addStreet(ulice4);
    linka->addStreet(ulice5);
    linka->addStreet(ulice6);*/
    linka->addStop(s1);
    linka->addStop(s2);
    scene->showLine(linka);
    ui->graphicsView->setScene(scene);


    //auto line = scene->addLine(0,0,100,100);
    //auto stop = new Stop("Nejaka",new Coordinate(0,0));
    //auto circle = scene->addEllipse(s1->getCoordinate()->getX(),s1->getCoordinate()->getY(),10,10);
    //line->setPen(QPen({Qt::red}, 4));

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::plus()
{
    ui->slide_zoom->setValue(ui->slide_zoom->value() + 10);
}

void MainWindow::minus()
{
    ui->slide_zoom->setValue(ui->slide_zoom->value() - 10);
}

void MainWindow::Zoom(int z)
{
    auto scale = z / 100.00;
    auto past = ui->graphicsView->transform();
    ui->graphicsView->setTransform(QTransform(scale, past.m12(), past.m21(), scale, past.dx(), past.dy()));
}

void MainWindow::left()
{
    auto transform = ui->graphicsView->transform();
    ui->graphicsView->setTransform(transform.rotate(-10));
}

void MainWindow::right()
{
    auto transform = ui->graphicsView->transform();
    ui->graphicsView->setTransform(transform.rotate(10));
}



void MainWindow::whats_time(QTime t)
{
    *time = t;
}

void MainWindow::speedos(double)
{
    timer->setInterval(1000.0/ui->btn_speed->value());
}

void MainWindow::play()
{
    timer->start();
}

void MainWindow::stop()
{
    timer->stop();
}

void MainWindow::tick()
{
    ui->my_timer->setTime(ui->my_timer->time().addSecs(1));
    this->update_traf();
    qDebug()<< "time:" << *time;
}

void MainWindow::update_traf()
{

    int len = this->doprava->size();
    for(int i = 0; i < len;++i)
    {

        traffic_t* traf = this->doprava->at(i);

        int length = traf->getT().size();
        qDebug() << this->time->secsTo(*traf->getT().at(0)) << this->time->secsTo(*traf->getT().at(traf->getT().size()-1));
        if(this->time->secsTo(*traf->getT().at(0)) <= 0 && this->time->secsTo(*traf->getT().at(traf->getT().size()-1)) > 0)
        {

            traf->getB()->start();
            Stop* s1 = traf->getStop().at(0);
            Stop* s2 = traf->getStop().at(traf->getT().size()-1);
            QTime* t1 = traf->getT().at(0);
            QTime* t2 = traf->getT().at(traf->getT().size()-1);

            for(int y = 1;y<length;++y)
            {
                if(*traf->getT().at(y) > *t1 && *traf->getT().at(y) <= *this->time)
                {
                    t1 = traf->getT().at(y);
                    s1 = traf->getStop().at(y);
                }
                else if(*traf->getT().at(y) < *t2 && *traf->getT().at(y) > *this->time)
                    t2 = traf->getT().at(y);
                    s2 = traf->getStop().at(y);
            }

            int start, end;
            for(int q = 0;q< traf->getS().size();++q)
            {
                if(s1->getStreet()->getId() == traf->getS().at(q)->getId())
                {
                    start = q;
                }
                else if(s2->getStreet()->getId() == traf->getS().at(q)->getId())
                {
                    end = q;
                }
            }
            Coordinate* c1;
            Coordinate* c2;
            Coordinate* c3;

            vector<Coordinate*>* vzdalenosti = new vector<Coordinate*>();
            for(int q = start; q < end;++q)
            {

                c1 = traf->getS().at(q)->begin();
                c2 = traf->getS().at(q)->end();
                if(traf->getS().at(q+1)->begin() == c1)
                {
                    c3 = traf->getS().at(q+1)->begin();
                    c1 = c2;
                }
                else if(traf->getS().at(q+1)->end() == c1)
                {
                    c3 = traf->getS().at(q+1)->end();
                    c1 = c2;
                }
                else if(traf->getS().at(q+1)->begin() == c2)
                {
                    c3 = traf->getS().at(q+1)->begin();
                }
                else if(traf->getS().at(q+1)->end() == c2)
                {
                    c3 = traf->getS().at(q+1)->end();
                }
                if(q == start) vzdalenosti->push_back(s1->getCoordinate());
                else vzdalenosti->push_back(c1);
                vzdalenosti->push_back(c3);
                if(q+1 == end)
                {
                    vzdalenosti->push_back(c3);
                    vzdalenosti->push_back(s2->getCoordinate());
                }
            }
            int lenn = (vzdalenosti->size() - 1);
            vector<double> vzdal;
            double celkem = 0;
            double step = 0;
            for(int t = 0;t < lenn;t += 2)
            {

                step = sqrt((pow(vzdalenosti->at(t)->getX()-vzdalenosti->at(t+1)->getX(),2.0),pow(vzdalenosti->at(t)->getY()-vzdalenosti->at(t+1)->getY(),2.0)));
                vzdal.push_back(step);
                celkem += step;
            }

            double secs = abs(t1->secsTo(*t2));
            double to_stop = time->secsTo(*t2);

            double pomer = to_stop/secs;
            celkem = celkem * pomer;

            lenn = vzdal.size() - 1;
            int index;
            for(int e = lenn; e >= 0;--e)
            {
                if(celkem - vzdal.at(e) <= 0)
                {
                    index = e*2;
                    break;
                }
                celkem -= vzdal.at(e);
            }
            c1 = vzdalenosti->at(index);
            c2 = vzdalenosti->at(index+1);
            qDebug() << c1->getX() << c1->getY();
            qDebug() << c2->getX() << c2->getY();
            pomer = celkem/vzdal.at(index/2);
            double dx = (c1->getX() - c2->getX()) * pomer;
            double dy = (c1->getY() - c2->getY()) * pomer;
            qDebug() << dx << dy;
            double addX = dx + c2->getX();
            double addY = dy + c2->getY();
            qDebug() << addY << addX;
            traf->getB()->update(addX,addY);
            scene->busses(traf->getB());
        }
        else traf->getB()->end();
    }
}



