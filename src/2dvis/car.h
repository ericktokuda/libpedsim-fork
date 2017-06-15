//
// pedsim - A microscopic pedestrian simulation system.
// Copyright (c) by Christian Gloor
//

#ifndef _2DVIS_CAR_H
#define _2DVIS_CAR_H

#include "agent.h"
#include <QGraphicsItem>

class Car : public Agent {

public:
    Car();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};

#endif
