#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //propojení gui se sloty v třídě main window

    //propojí tlačítko + s funkcí pro přiblížení scény
    connect(ui->btn_plus, &QPushButton::clicked, this, &MainWindow::plus);
    //propojí tlačítko - s funkci pro oddálení scény
    connect(ui->btn_minus, &QPushButton::clicked, this, &MainWindow::minus);
    //propojení slideru s funkcí pro změnu zoomu scény
    connect(ui->slide_zoom, &QSlider::valueChanged, this, &MainWindow::Zoom);
    //propojení tlačítka otočení vpravo
    connect(ui->btn_right, &QPushButton::clicked, this, &MainWindow::right);
    //propojení tlačítka otočení vlevo
    connect(ui->btn_left, &QPushButton::clicked, this, &MainWindow::left);
    //propojení tlačítka pro změnu rychlosti s funkcí pro změnu rychlosti běhu času
    connect(ui->btn_speed, SIGNAL(valueChanged(double)), this, SLOT(speedos(double)));
    //propojení tlačítka play se spuštěním běhu
    connect(ui->actionPlay, &QAction::triggered, this, &MainWindow::play);
    //propojení tlačítka play se zastavením běhu
    connect(ui->actionStop, &QAction::triggered, this, &MainWindow::stop);

    scene = new GraphicScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

   /* line* linka = new line("linka 1");
    line* linka2 = new line("linka 2");
    Coordinate* c1 = new Coordinate(20,20);
    Coordinate* c4 = new Coordinate(60,-60);
    Coordinate* c5 = new Coordinate(40,90);
    Coordinate* c6 = new Coordinate(200,200);
    Coordinate* c2 = new Coordinate(130,70);

    Street* ulice2 = new Street(nullptr,"Tercina",c5,c4);
    Street* ulice3 = new Street(nullptr,"Petrova",c4,c6);
    Street* ulice4 = new Street(nullptr,"Petrova3",c5,c6);
    Street* ulice5 = new Street(nullptr,"Petrova1",c1,c4);
    Street* ulice6 = new Street(nullptr,"Petrova2",c1,c5);*/

    timer = new QTimer();
    timer->setInterval(1000.0/ui->btn_speed->value());
    connect(timer, &QTimer::timeout, this, &MainWindow::tick);

    this->stops = this->addStops();
    int len = this->getStops().size();
    for(int i = 0;i < len;++i)
    {
        scene->addItem(this->getStops().at(i));
    }


    this->streets = this->addStreets();
    len = this->getStreets().size();
    for(int i = 0;i< len;++i)
    {
        scene->addItem(this->getStreets().at(i));
    }

    this->addLine();

    this->addTraf();
    this->addBus();


    /*Street* ulice = new Street(nullptr,"Palackeho",c4,c1);
    auto* casy = new vector<QTime*>();
    auto* casy2 = new vector<QTime*>();
    auto* ulic = new vector<Street*>();
    auto* ulic2 = new vector<Street*>();
    auto* stopky =new vector<Stop*>();
    auto* stopky2 =new vector<Stop*>();
    casy->push_back(new QTime(12,0,0,0));
    casy->push_back(new QTime(12,10,59,0));
    casy2->push_back(new QTime(12,0,0,0));
    casy2->push_back(new QTime(12,1,59,0));
    casy2->push_back(new QTime(12,3,59,0));
    casy2->push_back(new QTime(12,4,59,0));
    casy2->push_back(new QTime(12,7,59,0));
    ulic->push_back(ulice2);
    ulic->push_back(ulice);
    ulic2->push_back(ulice3);
    ulic2->push_back(ulice4);
    ulic2->push_back(ulice2);
    ulic2->push_back(ulice3);
    stopky2->push_back(s3);
    stopky2->push_back(s5);
    stopky2->push_back(s4);
    stopky2->push_back(s1);
    stopky2->push_back(s3);
    stopky->push_back(s1);
    stopky->push_back(s2);
    bus *b = new bus(nullptr, ui->traf_info);
    bus *b2 = new bus(nullptr, ui->traf_info);

    traffic_t* traff2 = new traffic_t(b2, *ulic2,*casy2, *stopky2);
    traffic_t* traff = new traffic_t(b,*ulic,*casy,*stopky);
    linka->addTraf(traff);
    linka2->addTraf(traff2);
    b2->add_T(traff2);
    b->add_T(traff);
    doprava.push_back(linka);
    doprava.push_back(linka2);


    linka->addStreet(ulice);
    linka->addStreet(ulice2);
    linka->addStreet(ulice3);
    linka->addStreet(ulice4);
    linka->addStreet(ulice5);
    linka->addStreet(ulice6);

    linka->addStop(s1);
    linka->addStop(s2);
    linka->addStop(s3);
    linka->addStop(s4);
    linka->addStop(s5);
    scene->showLine(linka);
    scene->addItem(b);
    b2->hide();
    b->hide();
    scene->addItem(b2);*/


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

vector<Stop*> MainWindow::addStops()
{
    io::CSVReader<3> inpt("../stop.txt");
    inpt.read_header(io::ignore_extra_column,"stop_name","stop_lat","stop_lon");
    if(!inpt.has_column("stop_name") || !inpt.has_column("stop_lat") || !inpt.has_column("stop_lon"))
        exit(1);
    string name;
    double x,y;
    vector<Stop*> vec;
    while(inpt.read_row(name,x,y))
    {
        Stop* s = new Stop(nullptr,name,new Coordinate(x,y));
        vec.push_back(s);
    }
    return vec;
}

vector<Street *> MainWindow::addStreets()
{
    io::CSVReader<5> inpt("../street.txt");
    inpt.read_header(io::ignore_extra_column,"street_name","street_x1","street_y1","street_x2","street_y2");
    if(!inpt.has_column("street_name") || !inpt.has_column("street_x1") || !inpt.has_column("street_y1") || !inpt.has_column("street_x2") || !inpt.has_column("street_y2"))
        exit(1);
    string name;
    double x1,y1, x2, y2;
    vector<Street*> vec;
    while(inpt.read_row(name,x1,y1,x2,y2))
    {
        Street* s = new Street(nullptr, name, new Coordinate(x1,y1), new Coordinate(x2,y2));
        vec.push_back(s);
    }
    return vec;
}

void MainWindow::addBus()
{
    io::CSVReader<2> inpt("../bus.txt");
    inpt.read_header(io::ignore_extra_column,"bus_name","traf_id");
    if(!inpt.has_column("bus_name") || !inpt.has_column("traf_id"))
        exit(1);
    string name;
    string id;
    while(inpt.read_row(name,id))
    {
        int len = this->doprava.size();
        for(int i = 0;i < len;++i)
        {
            vector<traffic_t*> t = this->doprava.at(i)->getTraf();
            int llen = t.size();
            bool added = false;
            bus* b = nullptr;
            for(int g = 0;g < llen;++g)
            {
                if(t.at(g)->getName() == id)
                {

                    b = new bus(nullptr,ui->traf_info,t.at(g));
                    scene->addItem(b);
                    added = true;
                    t.at(g)->addBus(b);
                    this->doprava.at(i)->addBus(b);
                    break;
                }
            }
            if(added == false) exit(2);
        }
    }
}


void MainWindow::addTraf()
{
    io::CSVReader<5> inpt("../traffic.txt");
    inpt.read_header(io::ignore_missing_column,"traf_id","Time_h","Time_m","Time_s","line");
    if(!inpt.has_column("traf_id") || !inpt.has_column("Time_s") || !inpt.has_column("Time_m") || !inpt.has_column("Time_h") || !inpt.has_column("line"))
        exit(1);
    string id;
    int h;
    int m;
    int s;
    string l_name;
    //projdi všechny řádky
    while(inpt.read_row(id,h,m,s,l_name))
    {
        //získej počet linek
        int len = doprava.size();

        //najdi správnou linku pro řád
        for(int i = 0; i < len;++i)
        {

            line* lin = doprava.at(i);
            //nalezl správnou linku
            if(lin->getId() == l_name)
            {
                traffic_t* traf = nullptr;
                vector<traffic_t*> traff = lin->getTraf();
                int llen = lin->getTraf().size();
                //kontrola, jestli se jízdní řád vyskytuje
                for(int e = 0;e < llen;++e)
                {
                    if(traff.at(e)->getName() == id)
                    {
                        traf = traff.at(e);
                        break;
                    }
                }
                if(traf == nullptr)
                {
                    traf = new traffic_t(id);
                    lin->addTraff(traf);
                    traf->addStop(lin->getStops());
                    traf->addStreet(lin->getRoute());
                }
                traf->addT(new QTime(h,m,s));




            }
        }
    }
}

void MainWindow::addLine()
{
    io::CSVReader<3> inpt("../line.txt");
    inpt.read_header(io::ignore_missing_column,"line","street","stop");
    if(!inpt.has_column("line") || !inpt.has_column("street") || !inpt.has_column("stop"))
        exit(1);
    string l_name;
    string street;
    optional<string> stop(inpt.has_column("stop") ? make_optional(string()) : nullopt);
    int cislo = 0;
    //projdi všechny řádky
    while(inpt.read_row(l_name,street,*stop))
    {
        //získej počet linek
        int len = doprava.size();
        line* linn = nullptr;
        //najdi správnou linku
        for(int i = 0; i < len;++i)
        {

            line* lin = doprava.at(i);
            //nalezl správnou linku
            if(lin->getId() == l_name)
            {
                linn = lin;
                break;
            }
        }
        if(linn == nullptr) linn = new line(l_name);
        //přidání ulic do linky podle jména
        len = this->getStreets().size();
        for(int i = 0;i < len;++i)
        {
            if(this->getStreets().at(i)->getId() == street)
            {
                //přidání zastávek do linky
                len = this->getStops().size();
                int e,prr;
                for(int i = 0;i < len;++i)
                {

                    if(this->getStops().at(i)->getId() == stop)
                    {
                        if(linn->getRoute().size() > 0 && street == linn->getRoute().at(linn->getRoute().size()-1)->getId()) prr = linn->stop_on(i-1);
                        else prr = cislo;
                        e = i;
                        break;
                    }
                }
                linn->addStreet(this->getStreets().at(i));
                linn->addStop(this->getStops().at(e),prr);
                break;
            }
        }


        len = this->doprava.size();
        bool add = true;
        for(int i = 0;i < len;++i)
        {
            if(this->doprava.at(i)->getId() == linn->getId()) add = false;
        }
        if(add == true) this->doprava.push_back(linn);
    ++cislo;
    }
}

void MainWindow::tick()
{
    ui->my_timer->setTime(ui->my_timer->time().addSecs(1));
    this->update_traf();
}

void MainWindow::update_traf()
{

    //získáme počet spojů
    int len = this->doprava.size();
    //projdeme každý spoj
    for(int i = 0; i < len;++i)
    {

        int len2 = this->doprava.at(i)->getTraf().size();
        for(int r = 0;r < len2;++r)
        {
            traffic_t* traf = this->doprava.at(i)->getTraf().at(r);
            //získáme počet časů
            int length = traf->getT().size();

            //pokud je čas mezi začátkem a koncem linky
            if(ui->my_timer->time().secsTo(*traf->getT().at(0)) <= 0 && ui->my_timer->time().secsTo(*traf->getT().at(traf->getT().size()-1)) > 0)
            {

                //ziskame prvni a posledni zastavku
                Stop* s1 = traf->getStop().at(0);
                Stop* s2 = traf->getStop().at(traf->getStop().size()-1);
                //ziskame prvni a posledni cas
                QTime* t1 = traf->getT().at(0);
                QTime* t2 = traf->getT().at(traf->getT().size()-1);
                int start = 0, end = 0;
                //projdeme všechny časy/zastávky a najdeme ty, které nejvíce vyhovují
                for(int y = 1;y<length;++y)
                {
                    //hledáme první zastávku/čas, který je největší, ale zároveň není větší než aktuální čas (čas předchozí zastávky)
                    if( *traf->getT().at(y) <= ui->my_timer->time() && *traf->getT().at(y) >= *t1)
                    {
                        t1 = traf->getT().at(y);
                        s1 = traf->getStop().at(y);
                        start = this->doprava.at(i)->stop_on(y);
                    }
                    //hledáme čas, který je nejmenší, ale zároveň není menší než aktuální čas( čas další zastávky)
                    else if(*traf->getT().at(y) > ui->my_timer->time() && *traf->getT().at(y) <= *t2)
                    {
                        t2 = traf->getT().at(y);
                        s2 = traf->getStop().at(y);
                        end = this->doprava.at(i)->stop_on(y);
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
                    if(traf->getS().at(q+1)->begin()->equals(c1))
                    {
                        c3 = traf->getS().at(q+1)->begin();
                        c1 = c2;
                    }
                    else if(traf->getS().at(q+1)->end()->equals(c1))
                    {
                        c3 = traf->getS().at(q+1)->end();
                        c1 = c2;
                    }
                    else if(traf->getS().at(q+1)->begin()->equals(c2))
                    {
                        c3 = traf->getS().at(q+1)->begin();
                    }
                    else if(traf->getS().at(q+1)->end()->equals(c2))
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
                    int x1 = vzdalenosti->at(t)->getX();
                    int x2 = vzdalenosti->at(t+1)->getX();
                    int y1 = vzdalenosti->at(t)->getY();
                    int y2 = vzdalenosti->at(t+1)->getY();
                    step = sqrt((pow(x1-x2,2.0),pow(y1-y2,2.0)));
                    vzdal.push_back(step);
                    celkem += step;
                }

                double secs = abs(t1->secsTo(*t2));
                double to_stop = ui->my_timer->time().secsTo(*t2);

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

                pomer = celkem/vzdal.at(index/2);
                double dx = (c1->getX() - c2->getX()) * pomer;
                double dy = (c1->getY() - c2->getY()) * pomer;

                double addX = dx + c2->getX();
                double addY = dy + c2->getY();
                bus* b = traf->getB();
                b->start();
                traf->getB()->update(addX,addY);
                }

            else traf->getB()->end();
            scene->busses(traf->getB());
        }
    }
}

vector<Stop *> MainWindow::getStops()
{
    return this->stops;
}

vector<Street *> MainWindow::getStreets()
{
    return this->streets;
}



