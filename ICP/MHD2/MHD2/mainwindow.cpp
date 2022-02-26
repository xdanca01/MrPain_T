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
    //propojení tlačítka open Linka s přidáním linek
    connect(ui->actionLine_2, &QAction::triggered, this, &MainWindow::addLine);
    //propojení tlačítka open Traffic s přidáním jízdních řádů
    connect(ui->actionTraffic, &QAction::triggered, this, &MainWindow::addTraf);
    //propojení tlačítka open Bus s přidáním autobusů
    connect(ui->actionBus_2, &QAction::triggered, this, &MainWindow::addBus);
    //propojení tlačítka quit s ukončením programu
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);


    scene = new GraphicScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    timer = new QTimer();
    //timer na tick
    timer->setInterval(1000.0/ui->btn_speed->value());
    connect(timer, &QTimer::timeout, this, &MainWindow::tick);
}

MainWindow::~MainWindow()
{
    delete ui;
}



//přiblížení scény
void MainWindow::plus()
{
    ui->slide_zoom->setValue(ui->slide_zoom->value() + 10);
}

//oddálení scény
void MainWindow::minus()
{
    ui->slide_zoom->setValue(ui->slide_zoom->value() - 10);
}


//přiblížení oddálení pomocí slideru
void MainWindow::Zoom(int z)
{
    auto scale = z / 100.00;
    auto past = ui->graphicsView->transform();
    ui->graphicsView->setTransform(QTransform(scale, past.m12(), past.m21(), scale, past.dx(), past.dy()));
}

//otočení scény o 10 stupňů doleva
void MainWindow::left()
{
    auto transform = ui->graphicsView->transform();
    ui->graphicsView->setTransform(transform.rotate(-10));
}

//otočení scény o 10 stupňů doprava
void MainWindow::right()
{
    auto transform = ui->graphicsView->transform();
    ui->graphicsView->setTransform(transform.rotate(10));
}

//změna rychlosti
void MainWindow::speedos(double)
{
    timer->setInterval(1000.0/ui->btn_speed->value());
}

//zapnutí běhu scény
void MainWindow::play()
{
    timer->start();
}

//zastavení scény
void MainWindow::stop()
{
    timer->stop();
}

//nahrání stopek do ui->stops
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
        //přidání grafického prvku do scény
        scene->addItem(s);
    }
    return;
}

//nahrání ulic do ui->streets
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
        //přidání ulice do grafické scény
        scene->addItem(s);
    }
    return;
}

//nahrání autobusů do scény/ jízdního řádu/ linky
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
            bus* b = nullptr;
            for(int g = 0;g < llen;++g)
            {
                if(t.at(g)->getName() == id)
                {

                    b = new bus(nullptr,ui->traf_info,t.at(g),name);
                    scene->addItem(b);
                    t.at(g)->addBus(b);
                    this->doprava.at(i)->addBus(b);
                    break;
                }
            }
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
    string l_name_bckp = "";
    int cislo = 0;
    //projdi všechny řádky
    while(inpt.read_row(l_name,street,*stop))
    {
        //pokud je linka nová začni s indexováním od 0
        if(l_name != l_name_bckp)
        {
            cislo = 0;
            l_name_bckp = l_name;
        }
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
                int e,prr = -1;
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
                if(prr != -1)
                {
                    linn->addStop(this->getStops().at(e),prr);
                }
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
            //získá linku
            line* llline = this->doprava.at(i);
            //získá dopravní řád
            traffic_t* traf = llline->getTraf().at(r);

            //počet ulic v jízdním řádu
            int street_cnt = traf->getS().size();
            //vektor pro uložení delayů linky
            vector<int> delays;
            //celkový delay linky
            int max_delay = 0;

            vector<Street*> STREETS = llline->getRoute();
            vector<Coordinate*> COORDINATES;
            vector<int> stop_end;
            //získáme delaye
            for(int delay_cnt = 0;delay_cnt < street_cnt;++delay_cnt)
            {
                //délka celé ulice
                double vzdal1 = 0;
                //délka úseku pro výpočet poměru
                double vzdal2 = 0;
                int stop_on = -1;
                int stop_on2 = -1;
                //získáme pozice stopek v lince
                for(unsigned iks = 0;iks < llline->Stops_pos().size();++iks)
                {
                    //pokud je pozice stopky odpovídá aktuální ulici vrať index v poli
                    if(llline->Stops_pos().at(iks) == delay_cnt)
                    {
                        //pozice v poli
                        stop_on = iks;
                        //dvě zastávky na stejné ulici
                        if(iks + 1 < llline->Stops_pos().size() && llline->Stops_pos().at(iks+1) == delay_cnt+1 && STREETS.at(llline->stop_on(iks + 1))->getId() == STREETS.at(llline->stop_on(stop_on))->getId())
                        {
                            stop_on2 = iks + 1;
                        }
                        break;
                    }
                    else if(delay_cnt + 1 == street_cnt && iks + 1 < llline->Stops_pos().size() && llline->Stops_pos().at(iks+1) == delay_cnt+1)
                    {
                        stop_on = iks + 1;
                        break;
                    }
                }
                double pomer = 1.0;
                int c;
                //první Cord je stop
                if(stop_on != -1 && stop_on2 == -1)
                {
                    //poslední ulice (jedna)
                    if(delay_cnt + 1 == street_cnt)
                    {
                        Coordinate* c1 = STREETS.at(delay_cnt-1)->begin();
                        Coordinate* c2 = STREETS.at(delay_cnt-1)->end();
                        Coordinate* c3 = STREETS.at(delay_cnt)->begin();
                        Coordinate* c4 = STREETS.at(delay_cnt)->end();
                        Coordinate* c5 = llline->getStops().at(stop_on)->getCoordinate();

                        vzdal1 = sqrt(pow(c1->getX()-c2->getX(),2.0) + pow(c1->getY()-c2->getY(),2.0));
                        if(c1->equals(c3) || c2->equals(c3))
                        {
                            //uložení souřadnic ve správném pořadí
                            COORDINATES.push_back(c3);
                            COORDINATES.push_back(c5);
                            vzdal2 = sqrt(pow(c5->getX()-c3->getX(),2.0) + pow(c5->getY()-c3->getY(),2.0));
                        }
                        else if(c1->equals(c4) || c2->equals(c4))
                        {
                            //uložení souřadnic ve správném pořadí
                            COORDINATES.push_back(c4);
                            COORDINATES.push_back(c5);
                            vzdal2 = sqrt(pow(c5->getX()-c4->getX(),2.0) + pow(c5->getY()-c4->getY(),2.0));
                        }
                        stop_end.push_back(delays.size());
                        pomer = abs(vzdal2/vzdal1);
                        c = traf->getS().at(delay_cnt)->getDelay() * pomer;
                        max_delay += c;
                        delays.push_back(c);

                    }
                    //Pokud je zastávka někde mezi ulicí, tak ulož oba úseky od začátku ulice po stopku, od stopky po konec ulice
                    else
                    {
                        Coordinate* c1 = STREETS.at(delay_cnt)->begin();
                        Coordinate* c2 = STREETS.at(delay_cnt)->end();
                        Coordinate* c3 = STREETS.at(delay_cnt+1)->begin();
                        Coordinate* c4 = STREETS.at(delay_cnt+1)->end();
                        Coordinate* c5 = llline->getStops().at(stop_on)->getCoordinate();
                        vzdal1 = sqrt(pow(c1->getX()-c2->getX(),2.0) + pow(c1->getY()-c2->getY(),2.0));
                        if(c1->equals(c3) || c2->equals(c3))
                        {
                            if(stop_on != 0 && c1->equals(c3))
                            {
                                COORDINATES.push_back(c2);
                                COORDINATES.push_back(c5);

                            }
                            else if(stop_on != 0)
                            {
                                COORDINATES.push_back(c1);
                                COORDINATES.push_back(c5);

                            }
                            COORDINATES.push_back(c5);
                            COORDINATES.push_back(c3);

                            vzdal2 = sqrt(pow(c5->getX()-c3->getX(),2.0) + pow(c5->getY()-c3->getY(),2.0));

                        }
                        else if(c1->equals(c4) || c2->equals(c4))
                        {
                            if(stop_on != 0 && c1->equals(c4))
                            {
                                COORDINATES.push_back(c2);
                                COORDINATES.push_back(c5);
                            }
                            else if(stop_on != 0)
                            {
                                COORDINATES.push_back(c1);
                                COORDINATES.push_back(c5);
                            }
                            COORDINATES.push_back(c5);
                            COORDINATES.push_back(c3);

                            vzdal2 = sqrt(pow(c5->getX()-c4->getX(),2.0) + pow(c5->getY()-c4->getY(),2.0));
                        }
                        if(stop_on != 0)
                        {
                            double vzdal3 = vzdal1 - vzdal2;
                            pomer = abs(vzdal3/vzdal1);
                            c = traf->getS().at(delay_cnt)->getDelay() * pomer;
                            max_delay += c;
                            delays.push_back(c);
                        }

                        stop_end.push_back(delays.size());
                        pomer = abs(vzdal2/vzdal1);
                        c = traf->getS().at(delay_cnt)->getDelay() * pomer;
                        max_delay += c;
                        delays.push_back(c);


                    }


                }
                //2 stopky na stejné ulici TODO (možná uložit víc jak 1 úsek/pár souřadnic)
                else if(stop_on2 != -1)
                {
                    Coordinate* c1 = STREETS.at(delay_cnt)->begin();
                    Coordinate* c2 = STREETS.at(delay_cnt)->end();
                    Coordinate* c5 = llline->getStops().at(stop_on)->getCoordinate();
                    Coordinate* c6 = llline->getStops().at(stop_on)->getCoordinate();
                    vzdal1 = sqrt(pow(c1->getX()-c2->getX(),2.0) + pow(c1->getY()-c2->getY(),2.0));
                    vzdal2 = sqrt(pow(c5->getX()-c6->getX(),2.0) + pow(c5->getY()-c6->getY(),2.0));
                    pomer = abs(vzdal2/vzdal1);
                    c = traf->getS().at(delay_cnt)->getDelay() * pomer;
                    max_delay += c;
                    delays.push_back(c);
                }
                //prázdná ulice
                else
                {
                    Coordinate* c1 = STREETS.at(delay_cnt)->begin();
                    Coordinate* c2 = STREETS.at(delay_cnt)->end();
                    Coordinate* c3 = STREETS.at(delay_cnt+1)->begin();
                    Coordinate* c4 = STREETS.at(delay_cnt+1)->end();
                    if(c1->equals(c3))
                    {
                        COORDINATES.push_back(c2);
                        COORDINATES.push_back(c3);
                    }
                    else if(c2->equals(c3))
                    {
                        COORDINATES.push_back(c1);
                        COORDINATES.push_back(c3);
                    }
                    else if(c1->equals(c4))
                    {
                        COORDINATES.push_back(c2);
                        COORDINATES.push_back(c4);
                    }
                    else if(c2->equals(c4))
                    {
                        COORDINATES.push_back(c1);
                        COORDINATES.push_back(c4);
                    }
                    c = traf->getS().at(delay_cnt)->getDelay();
                    max_delay += c;
                    delays.push_back(c);
                }


            }

            //získáme počet časů
            int length = stop_end.size()-1;
            //pokud má být autobus na cestě first_stop_time <= actual_time && last_stop_time >= actual_time
            if(ui->my_timer->time().secsTo(*traf->getT().at(0)) <= 0 && ui->my_timer->time().secsTo(*traf->getT().at(traf->getT().size()-1)) + max_delay > 0)
            {

                //ziskame prvni a posledni cas
                QTime t1 = *traf->getT().at(0);
                QTime t4 = t1;
                QTime t2 = *traf->getT().at(traf->getT().size()-1);
                QTime t3 = t2;
                int start = 0, end = stop_end.at(stop_end.size()-1);
                //zpoždění po zastávku
                int dly = 0;
                //zpoždění před začátkem ulice pro výpočet správného času po zpoždění z předchozích ulic
                int dly2 = 0;
                //projdeme všechny časy/zastávky a najdeme ty, které nejvíce vyhovují
                for(int y = 0;y <= length;++y)
                {

                    dly = 0;
                    dly2 = 0;
                    if(y+1 <= length)
                    {
                        for(int pls = 0; pls < stop_end.at(y+1);++pls)
                        {
                            dly += delays.at(pls);
                        }
                    }
                    else
                    {
                        for(int pls = 0; pls <= stop_end.at(y);++pls)
                        {
                            dly += delays.at(pls);
                        }
                    }

                    for(int pls = 0; pls < stop_end.at(y);++pls)
                    {
                        dly2 += delays.at(pls);
                    }
                    if(y == 0)
                    {
                        t2 = t2.addSecs(dly);
                    }
                    //qDebug() << "first cycle" << traf->getT().at(y)->addSecs(dly) << ui->my_timer->time() << traf->getT().at(y)->addSecs(dly2) << t1.addSecs(dly) << t2.addSecs(dly2);
                    //hledáme první zastávku/čas, který je největší, ale zároveň není větší než aktuální čas (čas předchozí zastávky)
                    if(y < length && traf->getT().at(y)->addSecs(dly2) <= ui->my_timer->time())
                    {
                        t1 = traf->getT().at(y)->addSecs(dly2);
                        start = stop_end.at(y);
                        t4 = *traf->getT().at(y);
                    }
                    qDebug() << "casy" << *traf->getT().at(y) << t2 << dly << dly2;
                    //hledáme čas, který je nejmenší, ale zároveň není menší než aktuální čas( čas další zastávky)
                    if(y > 0 && traf->getT().at(y)->addSecs(dly2) >= ui->my_timer->time() && traf->getT().at(y)->addSecs(dly) <= t2 && traf->getT().at(y)->addSecs(dly2) > t1)
                    {
                        t2 = traf->getT().at(y)->addSecs(dly2);
                        t3 = *traf->getT().at(y);
                        end = stop_end.at(y);
                        if(y == length)
                        {
                            end += 1;
                        }
                    }
                }
                qDebug() << t1 << t2;
                qDebug() << "start" << start << "end" << end;

                vector<Coordinate*> vzdalenosti;
                //kalibrace pomáhá zjednodušit podmínky
                if(end == stop_end.at(stop_end.size()-1)) end += 1;
                for(int j = start*2; j < end*2;j += 2)
                {
                    vzdalenosti.push_back(COORDINATES.at(j));
                    vzdalenosti.push_back(COORDINATES.at(j+1));
                }
                qDebug() << vzdalenosti.size();

                int lenn = vzdalenosti.size();
                vector<double> vzdal;
                //celková vzdálenost mezi zastávkami
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




                // VÝPOČET ČASŮ //

                vector<QTime> times;
                times.push_back(t1);
                //kalibrace času (čas bez zpoždění)
                t3 = t3.addSecs(t4.secsTo(t1));
                double speed = celkem / t1.secsTo(t3);
                //qDebug() << "speed:" << speed << t1 << t2;

                double x, y;
                for(int tr = 0; tr < lenn;++tr)
                {
                    //delay na ulici
                    double delayik = delays.at(start+tr);
                    double vzdalenost = vzdal.at(tr);
                    delayik += vzdalenost/speed;
                    times.push_back(times.at(tr).addSecs(delayik));
                    qDebug() << "times debug" << delayik << times.at(tr+1) << ui->my_timer->time();
                    //výběr souřadnic podle času
                    if(times.at(tr+1) >= ui->my_timer->time() || (tr + 1 == lenn))
                    {

                        int x1 = vzdalenosti.at(tr*2)->getX();
                        int x2 = vzdalenosti.at(tr*2+1)->getX();
                        int y1 = vzdalenosti.at(tr*2)->getY();
                        int y2 = vzdalenosti.at(tr*2+1)->getY();
                        qDebug() << "x" << x1 << x2 << "y" << y1 << y2;
                        double cas = times.at(tr).secsTo(times.at(tr+1));
                        double cas2 = times.at(tr).secsTo(ui->my_timer->time());
                        double pomer = cas2/cas;
                        if(pomer > 1.0) pomer = 1.0;
                        x = x1 + (x2 - x1)*pomer;
                        y = y1 + (y2 - y1)*pomer;
                        break;
                    }

                }
                bus* b = traf->getB();
                //aktualizace souřadnic
                b->update(x,y);
                //zobrazení na grafické scéně
                b->start();

                }

            //schová bus na scéně
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



