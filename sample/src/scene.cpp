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
long counter = 0;

Scene::Scene(QGraphicsScene *pscene) : Tscene(), scene(pscene)  {
  tree = new Tree(pscene, this, 0, -200, -160, 400, 320);
  grid = new Grid(-200, -160, 400, 320, scene);
  heatcells  = vector<vector<QGraphicsItem*>>(100 , vector<QGraphicsItem*>(100, (QGraphicsItem*) NULL));

  movetimer = new QTimer();
  QObject::connect(movetimer, SIGNAL(timeout()), this, SLOT(moveAllAgents()));
  movetimer->start(100000/20);

  cleanuptimer = new QTimer();
  //heattimer = new QTimer();
  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(printCellDensity()));
  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(printNearToCar()));
  //heattimer->start(1000);

  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(cleanupSlot()));
  cleanuptimer->start(1000);
};

void Scene::printNearToCar() {
	int n = countNearToCar();
	printf("Around the car: %d\t", n);
}

void Scene::printCellDensity() {
	int xrad = 5;
	int yrad = 5;;
	int alpha = 70;
	int cellsize = 10;
	int maxped = 6;

	int i, j;
	for (i = -9; i < 9; i++) {
		int ii = i + 9;
		for (j = -9; j < 9; j++) {
			int jj = j + 9;
			int xcenter = i * cellsize + xrad;
			int ycenter = j * cellsize + xrad;
			int n = getCellDensity(xcenter, ycenter, xrad, yrad);

			if (heatcells[ii][jj] != NULL)
				scene->removeItem(heatcells[ii][jj]);

			int c = (int)( ((float)(n) / maxped) * 255);
			if (c > 255) c = 255;

			QColor color = QColor(c, c, c, alpha);

			QGraphicsItem *rect = (QGraphicsItem*) scene->addRect(QRectF(xcenter - xrad, ycenter - yrad, 2*xrad,2*yrad),
					QPen(color, 0.1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
					QBrush(color));

			heatcells[ii][jj] = rect;
		}
	}

	printf("%ld\n", counter);
	counter += 1;
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
