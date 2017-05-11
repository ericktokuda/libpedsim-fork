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
unsigned int nticks = 0;

Scene::Scene(QGraphicsScene *pscene) : Tscene(), scene(pscene)  {
  tree = new Tree(pscene, this, 0, -200, -160, 400, 320);
  grid = new Grid(-120, -130, 450, 250, scene);
  realheatmap  = vector<vector<QGraphicsItem*>>(100 , vector<QGraphicsItem*>(100, (QGraphicsItem*) NULL));
  obsheatmap  = vector<vector<QGraphicsItem*>>(100 , vector<QGraphicsItem*>(100, (QGraphicsItem*) NULL));
  realcount = vector<vector<int>>(100 , vector<int>(100, 0));
  obsnsamples = vector<vector<int>>(100 , vector<int>(100, 0));
  obscount = vector<vector<int>>(100 , vector<int>(100, 0));

  movetimer = new QTimer();
  QObject::connect(movetimer, SIGNAL(timeout()), this, SLOT(moveAllAgents()));
  movetimer->start(10000/20);

  cleanuptimer = new QTimer();
  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(incrementTicks()));
  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(renderRealDensities()));
  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(renderObservedDensities()));

  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(cleanupSlot()));
  cleanuptimer->start(1000);
};

//void Scene::printNearToCar() {
	//int n = countNearToCar();
	//printf("Around the car: %d\t", n);
//}

void Scene::incrementTicks() {
	nticks += 1;
}

void Scene::getPointCell(const int pointpos[2], int indices[2]) { //very naive way
	int cellsize = 10;
	int i, j;
	const int *p = pointpos;
	int minind[] = {0, 0};
	int min = 9999;

	for (i = -9; i < 9; i++) {
		int cellx = i*cellsize;

		if (cellx > p[0]) continue;

		for (j = -9; j < 9; j++) {
			int celly = j*cellsize;

			if (celly > p[1]) continue;

			int manh = (p[0] - cellx) + (p[1] - celly);
			if (manh < min) {
				min = manh;
				minind[0] = i;
				minind[1] = j;
			}
		}
	}
	indices[0] = minind[0];
	indices[1] = minind[1];
}

void Scene::renderObservedDensities() {
	int xrad = 5;
	int yrad = 5;;
	int alpha = 100;
	int cellsize = 10;
	int maxdensity = 3;
	float colorfactor = 255 / maxdensity ;
	//int xshift = 200;

	int i, j;

	int carpos[] = {0, 0};
	int carcell[] = {0, 0};

	getCarPos(carpos);
	getPointCell(carpos, carcell);

	for (i = carcell[0] - 1; i <= carcell[0] + 1 ; i++) {
		int ii = i + 9;
		for (j = carcell[1] - 1; j <= carcell[1] + 1 ; j++) {
			int jj = j + 9;
			int xcenter = i * cellsize + cellsize/2;
			int ycenter = j * cellsize + cellsize/2;

			int  kkk = getRealCount(xcenter, ycenter, xrad, yrad);
			obscount[ii][jj] += kkk;
			obsnsamples[ii][jj] ++;
			//printf("i:%d, j:%dcount:%d\n", i, j, kkk);
		}
	}

	//return;

	for (i = -9; i < 9; i++) {
		int ii = i + 9;

		for (j = -9; j < 9; j++) {
			int jj = j + 9;

			int xcenter = i * cellsize + xrad;
			int ycenter = j * cellsize + xrad;

			if (obsheatmap[ii][jj] != NULL)
				scene->removeItem(obsheatmap[ii][jj]);

			float density = 0;
			if (obsnsamples[ii][jj] == 0) continue;

			density = (float) obscount[ii][jj] / obsnsamples[ii][jj];

			int c = (int)(density * colorfactor);
			if (c > 255) c = 255;

			QColor color = QColor(c, c, c, alpha);

			QGraphicsItem *rect = (QGraphicsItem*) scene->addRect(
					QRectF(xcenter - xrad, ycenter - yrad, 2*xrad,2*yrad),
					QPen(color, 0.1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
					QBrush(color));

			obsheatmap[ii][jj] = rect;
		}
	}
}

void Scene::renderRealDensities() {
	int xrad = 5;
	int yrad = 5;;
	int alpha = 100;
	int cellsize = 10;
	int maxdensity = 3;
	float colorfactor = 255 / maxdensity ;
	int xshift = 200;


	int i, j;

	for (i = -9; i < 9; i++) {
		int ii = i + 9;

		for (j = -9; j < 9; j++) {
			int jj = j + 9;

			int xcenter = i * cellsize + xrad;
			int ycenter = j * cellsize + xrad;

			int  kkk = getRealCount(xcenter, ycenter, xrad, yrad);
			realcount[ii][jj] += kkk;
			int cellcount = realcount[ii][jj];
			float density = cellcount / (float) nticks;

			if (realheatmap[ii][jj] != NULL)
				scene->removeItem(realheatmap[ii][jj]);

			int c = (int)(density * colorfactor);
			if (c > 255) c = 255;

			QColor color = QColor(c, c, c, alpha);

			QGraphicsItem *rect = (QGraphicsItem*) scene->addRect(
					QRectF(xcenter - xrad + xshift, ycenter - yrad, 2*xrad,2*yrad),
					QPen(color, 0.1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
					QBrush(color));

			realheatmap[ii][jj] = rect;
		}
	}
	printf("Tick %u\n", nticks);
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
