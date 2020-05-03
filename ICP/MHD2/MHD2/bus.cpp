#include "bus.h"



bus::bus(QGraphicsItem *parent, QTableWidget* table) :
    QGraphicsLineItem(parent)
{
    this->tab = table;
    this->setPen(QPen({Qt::black}, 8));
    this->setFlag(QGraphicsItem::ItemIsSelectable);

}

void bus::update(double Xx, double Yy)
{
    x = Xx;
    y = Yy;
}

void bus::start()
{
    this->garage = false;
}

void bus::end()
{
    this->garage = true;
}

bool bus::status()
{
    return this->garage;
}

void bus::add_T(traffic_t *t)
{
    this->traf = t;
}

void bus::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    this->tab->setRowCount(0);
    int len = this->traf->getStop().size();
    for(int i = 0;i < len;++i)
    {
        this->tab->insertRow(i);
        this->tab->setItem(i,0,new QTableWidgetItem(*new QString(this->traf->getStop().at(i)->getId().c_str()),1));
        this->tab->setItem(i,1,new QTableWidgetItem(this->traf->getT().at(i)->toString()));
    }
    len = this->traf->getS().size();
   /* for(int i = 0;i < len;++i)
    {
        this->traf->getS().at(i)->
    }*/
    QGraphicsLineItem::mouseDoubleClickEvent(event);
}

double bus::getX()
{
    return x;
}

double bus::getY()
{
    return y;
}



