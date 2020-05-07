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
    connect(ui->Delay, SIGNAL(valueChanged(int)), this, SLOT(check_sel(int)));

    //propojení tlačítka open stop s přidáním zastávek
    connect(ui->actionStop_3, &QAction::triggered, this, &MainWindow::addStops);
    //propojení tlačítka open Street s přidáním zastávek
    connect(ui->actionStreet_2, &QAction::triggered, this, &MainWindow::addStreets);
    connect(ui->actionLine_2, &QAction::triggered, this, &MainWindow::addLine);
    connect(ui->actionTraffic, &QAction::triggered, this, &MainWindow::addTraf);
    connect(ui->actionBus_2, &QAction::triggered, this, &MainWindow::addBus);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);

    scene = new GraphicScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    timer = new QTimer();
    timer->setInterval(1000.0/ui->btn_speed->value());
    connect(timer, &QTimer::timeout, this, &MainWindow::tick);
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

void MainWindow::addStops()
{
    QString f_name = QFileDialog::getOpenFileName(this, "STOP");
    io::CSVReader<3> inpt(f_name.toStdString());
    inpt.read_header(io::ignore_extra_column,"stop_name","stop_lat","stop_lon");
    if(!inpt.has_column("stop_name") || !inpt.has_column("stop_lat") || !inpt.has_column("stop_lon"))
        exit(1);
    string name;
    double x,y;
    while(inpt.read_row(name,x,y))
    {
        Stop* s = new Stop(nullptr,name,new Coordinate(x,y));
        this->stops.push_back(s);
        scene->addItem(s);
    }
    return;
}

void MainWindow::addStreets()
{
    QString f_name = QFileDialog::getOpenFileName(this, "STREET");
    io::CSVReader<5> inpt(f_name.toStdString());
    inpt.read_header(io::ignore_extra_column,"street_name","street_x1","street_y1","street_x2","street_y2");
    if(!inpt.has_column("street_name") || !inpt.has_column("street_x1") || !inpt.has_column("street_y1") || !inpt.has_column("street_x2") || !inpt.has_column("street_y2"))
        exit(1);
    string name;
    double x1,y1, x2, y2;
    while(inpt.read_row(name,x1,y1,x2,y2))
    {
        Street* s = new Street(nullptr, ui->Delay, name, new Coordinate(x1,y1), new Coordinate(x2,y2));

        this->streets.push_back(s);
        scene->addItem(s);
    }
    return;
}

void MainWindow::addBus()
{
    QString f_name = QFileDialog::getOpenFileName(this, "BUS");
    io::CSVReader<2> inpt(f_name.toStdString());
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
    return;
}


void MainWindow::addTraf()
{
    QString f_name = QFileDialog::getOpenFileName(this, "TRAFFIC");
    io::CSVReader<5> inpt(f_name.toStdString());
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
    return;
}

void MainWindow::addLine()
{
    QString f_name = QFileDialog::getOpenFileName(this, "LIN");
    io::CSVReader<3> inpt(f_name.toStdString());
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

void MainWindow::quit()
{
    exit(0);
}

void MainWindow::check_sel(int)
{
    vector<Street*> streeets = this->getStreets();
    int lenn = streeets.size();
    for(int i = 0;i < lenn;++i)
    {
        if(streeets.at(i)->isSelected() == true)
        {
            streeets.at(i)->setDelay(ui->Delay->value());
            break;
        }
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
    //projdeme každou linku
    for(int i = 0; i < len;++i)
    {

        int len2 = this->doprava.at(i)->getTraf().size();
        //projdeme každý řád v lince
        for(int r = 0;r < len2;++r)
        {
            line* llline = this->doprava.at(i);
            traffic_t* traf = llline->getTraf().at(r);
            //získáme počet časů
            int length = traf->getT().size();

            int street_cnt = traf->getS().size();
            vector<int> delays;
            int max_delay = 0;
            //získáme delaye
            for(int delay_cnt = 0;delay_cnt < street_cnt;++delay_cnt)
            {
                int c = traf->getS().at(delay_cnt)->getDelay();
                max_delay += c;
                delays.push_back(c);
            }
            //pokud má být autobus na cestě first_stop_time <= actual_time && last_stop_time >= actual_time
            if(ui->my_timer->time().secsTo(*traf->getT().at(0)) <= 0 && ui->my_timer->time().secsTo(*traf->getT().at(traf->getT().size()-1)) + max_delay > 0)
            {

                //ziskame prvni a posledni zastavku
                Stop* s1 = traf->getStop().at(0);
                Stop* s2 = traf->getStop().at(traf->getStop().size()-1);
                //ziskame prvni a posledni cas
                QTime t1 = *traf->getT().at(0);
                QTime t2 = *traf->getT().at(traf->getT().size()-1);
                int start = 0, end = this->doprava.at(i)->stop_on(traf->getStop().size()-1);
                int dly = 0;
                int dly2 = 0;
                //projdeme všechny časy/zastávky a najdeme ty, které nejvíce vyhovují
                for(int y = 1;y<length;++y)
                {
                    dly = 0;
                    dly2 = 0;
                    for(int pls = 0; pls <= llline->stop_on(y);++pls)
                    {
                        dly += delays.at(pls);
                        if(y > 0 && pls < llline->stop_on((y-1)))
                            dly2 += delays.at(pls);
                    }
                    //hledáme první zastávku/čas, který je největší, ale zároveň není větší než aktuální čas (čas předchozí zastávky)
                    if( traf->getT().at(y)->addSecs(dly) <= ui->my_timer->time() && traf->getT().at(y)->addSecs(dly2) >= t1.addSecs(dly))
                    {
                        t1 = traf->getT().at(y)->addSecs(dly2);
                        s1 = traf->getStop().at(y);
                        start = this->doprava.at(i)->stop_on(y);
                    }
                    //hledáme čas, který je nejmenší, ale zároveň není menší než aktuální čas( čas další zastávky)
                    else if(traf->getT().at(y)->addSecs(dly) >= ui->my_timer->time() && traf->getT().at(y)->addSecs(dly) <= t2.addSecs(dly2))
                    {
                        t2 = *traf->getT().at(y);
                        s2 = traf->getStop().at(y);
                        end = this->doprava.at(i)->stop_on(y);
                    }
                }


                Coordinate* c1;
                Coordinate* c2;
                Coordinate* c3;

                vector<Coordinate*> vzdalenosti;
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
                    if(q == start) vzdalenosti.push_back(s1->getCoordinate());
                    else vzdalenosti.push_back(c1);
                    vzdalenosti.push_back(c3);
                    if(q+1 == end)
                    {
                        vzdalenosti.push_back(c3);
                        vzdalenosti.push_back(s2->getCoordinate());
                    }
                }

                int lenn = (vzdalenosti.size() - 1);
                vector<double> vzdal;
                double celkem = 0;
                double step = 0;
                for(int t = 0;t < lenn;t += 2)
                {

                    int x1 = vzdalenosti.at(t)->getX();
                    int x2 = vzdalenosti.at(t+1)->getX();
                    int y1 = vzdalenosti.at(t)->getY();
                    int y2 = vzdalenosti.at(t+1)->getY();

                    step = sqrt((pow(x1-x2,2.0) + pow(y1-y2,2.0)));
                    vzdal.push_back(step);
                    celkem += step;
                }
                lenn = vzdal.size();
                vector<QTime> times;
                times.push_back(t1);
                double speed = celkem / t1.secsTo(t2);
                qDebug() << "speed:" << speed << t1 << t2;

                double x, y;
                for(int tr = 0; tr < lenn;++tr)
                {
                    //delay na ulici
                    double delayik = traf->getS().at(start+tr)->getDelay();
                    double vzdalenost = vzdal.at(tr);
                    delayik += vzdalenost/speed;
                    times.push_back(times.at(tr).addSecs(delayik));
                    if(times.at(tr+1) >= ui->my_timer->time())
                    {

                        int x1 = vzdalenosti.at(tr*2)->getX();
                        int x2 = vzdalenosti.at(tr*2+1)->getX();
                        int y1 = vzdalenosti.at(tr*2)->getY();
                        int y2 = vzdalenosti.at(tr*2+1)->getY();
                        qDebug() << "x" << x1 << x2 << "y" << y1 << y2;
                        double cas = times.at(tr).secsTo(times.at(tr+1));
                        double cas2 = times.at(tr).secsTo(ui->my_timer->time());
                        double pomer = cas2/cas;
                        qDebug() << pomer;
                        x = x1 + (x2 - x1)*pomer;
                        y = y1 + (y2 - y1)*pomer;
                        break;
                    }
                }
                bus* b = traf->getB();
                b->update(x,y);
                b->start();
                /*times.push_back(t2.addSecs(dly));
                QTime* testik = new QTime(0,0,0);
                for(int sr = start; sr <= end;++sr)
                {
                    int seconds = testik->secsTo(times.at(times.size()-1)) - delays.at(end-sr);
                    int hour = seconds/3600;
                    seconds -= hour * 3600;
                    int minutes = seconds/60;
                    seconds -= minutes * 60;
                    QTime* r = new QTime(hour,minutes,seconds);
                    qDebug() << *r;
                    times.push_back(*r);
                }
                int pocet_ulic = end - start;
                for(int t_cnt = 0;t_cnt < pocet_ulic;++t_cnt)
                {
                    qDebug() << "t2" << t2;
                    double traveled = 0;
                    for(int po = 0; po <= t_cnt;++po)
                    {
                        traveled += vzdal.at(po);
                    }
                    int time_to_travel = t1.secsTo(t2);
                    // km/s
                    double speed = celkem/time_to_travel;
                    int actual_t = t1.secsTo(ui->my_timer->time());
                    int act_traveled = actual_t*speed;
                    double pomer = double(act_traveled)/double(traveled);
                    qDebug() << pomer;
                    int secondds = double(t2.secsTo(times.at(times.size()-1-t_cnt)))*pomer;
                    int hours = t2.hour() + secondds/3600;
                    secondds = secondds%3600;
                    int minutes = t2.minute() +  secondds/60;
                    secondds = secondds%60;
                    int seconds = t2.second() + secondds;
                    QTime* t3 = new QTime(hours,minutes,seconds);
                    t2 = *t3;
                    qDebug() << t2;
                    if(traveled > act_traveled) break;
                }
                qDebug() << t2;
                double secs = t1.secsTo(t2);
                double to_stop = ui->my_timer->time().secsTo(t2);

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
                    qDebug() << e << vzdal.at(e);
                }
                c1 = vzdalenosti->at(index);
                c2 = vzdalenosti->at(index+1);

                pomer = celkem/vzdal.at(index/2);
                double dx = (c1->getX() - c2->getX()) * pomer;
                double dy = (c1->getY() - c2->getY()) * pomer;

                double addX = dx + c2->getX();
                double addY = dy + c2->getY();*/
                //traf->getB()->update(addX,addY);
                }

            else traf->getB()->end();
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



