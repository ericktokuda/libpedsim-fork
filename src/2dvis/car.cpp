//
// pedsim - A microscopic pedestrian simulation system.
// Copyright (c) by Christian Gloor
//

#include "car.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

static const double SCALE = 10.0;


//Agent::Agent() : color(qrand() % 256, qrand() % 256, qrand() % 256) {}

Car::Car() : Agent() {
}


//QRectF Car::boundingRect() const {
    //qreal adjust = 0.5;
    //return QRectF(SCALE * (-18 - adjust), SCALE * (-22 - adjust), SCALE * (36 + adjust), SCALE * (60 + adjust));
//}


//QPainterPath Car::shape() const {
    //QPainterPath path;
    //path.addRect(SCALE * -10, SCALE * -20, SCALE * 20, SCALE * 20);
    //return path;
//}


void Car::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

  painter->setBrush(Qt::green);
  painter->setPen(QPen(QBrush(Qt::white), .1));
  QRectF rectangle(SCALE*-0.4, SCALE*-0.4, SCALE*0.8, SCALE*0.8);
  painter->drawRect(rectangle);


}


//void Car::advance(int step) {
    //if (!step) return;
    //setPos(SCALE * x, SCALE * y);
//}
