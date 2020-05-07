#include "bus.h"



bus::bus(QGraphicsItem *parent, QTableWidget* table, traffic_t *t) :
    QGraphicsEllipseItem(-1,-1,2,2,parent)
{
    this->tab = table;
    this->setPen(QPen({Qt::black}, 2));
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->traf = t;
    t->addBus(this);
    this->hide();

}

void bus::update(double Xx, double Yy)
{
    this->setX(Xx);
    this->setY(Yy);
    y = Yy;
    x = Xx;
}

void bus::start()
{
    this->show();
}

void bus::end()
{
    this->hide();
}

void bus::add_T(traffic_t *t)
{
    this->traf = t;
}

void bus::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    int len = this->traf->getStop().size();
    for(int i = 0;i < len;++i)
    {
        this->tab->insertRow(i);
        this->tab->setItem(i,0,new QTableWidgetItem(*new QString(this->traf->getStop().at(i)->getId().c_str()),1));
        this->tab->setItem(i,1,new QTableWidgetItem(this->traf->getT().at(i)->toString()));
    }
    len = this->traf->getS().size();
    for(int i = 0;i < len;++i)
    {
        this->traf->getS().at(i)->setPen(QPen({Qt::blue}, 3));
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}

void bus::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->tab->setRowCount(0);
    int len = this->traf->getS().size();
    for(int i = 0;i < len;++i)
    {
        this->traf->getS().at(i)->setPen(QPen({Qt::blue}, 1));
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

double bus::getX()
{
    return x;
}

double bus::getY()
{
    return y;
}



