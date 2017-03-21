//
// pedsim - A microscopic pedestrian simulation system. 
// Copyright (c) 2003 - 2012 by Christian Gloor
//                              

#include "scene.h"
#include "config.h"
#include "tree.h"
#include "grid.h"

#include <QTimer>

#include <iostream>

using namespace std;

extern Config config;


Scene::Scene(QGraphicsScene *pscene) : Tscene(), scene(pscene)  {
  tree = new Tree(pscene, this, 0, -200, -160, 400, 320);
  grid = new Grid(-200, -160, 400, 320, scene);

  movetimer = new QTimer();
  QObject::connect(movetimer, SIGNAL(timeout()), this, SLOT(moveAllAgents()));
  movetimer->start(1000/20);

  //movetimer = new QTimer();
  QObject::connect(movetimer, SIGNAL(timeout()), this, SLOT(printNearToCar()));
  //movetimer->start(1000/20);

  cleanuptimer = new QTimer();
  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(cleanupSlot()));
  cleanuptimer->start(1000);
};

//int countNearToCar(double a){}

void Scene::printNearToCar() {
	int n = countNearToCar(config.simh);
	printf("n: %d\n", n);
}

void Scene::moveAllAgents() {
	moveAgents(config.simh);
	movetimer->setInterval(config.simSpeed);
}


void Scene::cleanupSlot() {
	cleanup();
}


double Scene::getGridValue(double x, double y, int value) {
	return grid->getValue(x, y, value);
}
