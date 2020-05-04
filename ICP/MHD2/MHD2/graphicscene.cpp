#include "graphicscene.h"

#include <QDebug>


GraphicScene::GraphicScene(QObject *parent) : QGraphicsScene(parent)
{

}
bool GraphicScene::showLine(line *linka)
{
    vector<Street*>* lnks = linka->getRoute();
    if(lnks == nullptr) return false;
    int len = lnks->size();
    for(int i = 0; i < len;++i)
    {
        if(lnks->at(i))
        {
            vector<Coordinate*>* crds = lnks->at(i)->getCoordinates();
            if(crds)
            {
                int length = crds->size();
                for(int y = 1; y < length;++y)
                {
                    if(lnks->at(i)->status() == false)
                    {
                        this->addItem(lnks->at(i));
                        lnks->at(i)->setLine(crds->at(y-1)->getX(),crds->at(y-1)->getY(),crds->at(y)->getX(),crds->at(y)->getY());
                    }
                    else
                    {
                        lnks->at(i)->setPen(QPen({Qt::red}, 1));
                    }
                }
            }

            vector<Stop*>* stps = lnks->at(i)->getStops();
            if(stps)
            {
                int length = stps->size();
                for(int y = 0; y < length;++y)
                {
                    auto circle = this->addEllipse(stps->at(y)->getCoordinate()->getX()-3,stps->at(y)->getCoordinate()->getY()-3,6,6);
                    circle->setPen(QPen({Qt::green}, 6));
                }

            }
        }
    }

    return true;

}

void GraphicScene::busses(bus* b)
{
    if(b->status() == false)
    {
        b->show();
        b->setLine(b->getX()-5,b->getY()-5,b->getX()+5,b->getY()+5);
    }
    else b->hide();
}



