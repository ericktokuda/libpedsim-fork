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

#define REAL 0
#define OBSERVED 1
using namespace std;

extern Config config;
unsigned int nticks = 0;
	int xrad = 5;
	int yrad = 5;;
	int alpha = 100;
	int cellsize = 10;
	int maxdensity = 3;
	float colorfactor = 255 / maxdensity ;
	int xshift = 200;

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
  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(updateAndRenderRealDensities()));
  QObject::connect(cleanuptimer, SIGNAL(timeout()), this, SLOT(updateAndRenderObsDensities()));

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

void Scene::updateAndRenderObsDensities() {
	updateCounts(OBSERVED);
	renderDensities(OBSERVED);
	return;
}

QGraphicsItem* Scene::renderRect(int x, int y, int w, int h, QColor color) {
	return (QGraphicsItem*) scene->addRect(
			QRectF(x, y, w, h),
			QPen(color, 0.1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
			QBrush(color));
}

void Scene::renderCellDensity(int ii, int jj, int option) {
	int i = ii - 9;
	int j = jj - 9;
	int rectx = i*cellsize;
	int recty = j*cellsize;
	float density;

	if (option == REAL) {
		rectx += xshift;
		int cellcount = realcount[ii][jj];
		density = (float) cellcount / nticks;
	}
	else if (option == OBSERVED) {
		if (obsnsamples[ii][jj] == 0) return;
		density = (float) obscount[ii][jj] / obsnsamples[ii][jj];
	}

	int c = (int)(density * colorfactor);
	if (c > 255) c = 255;
	QColor color = QColor(c, c, c, alpha);

	QGraphicsItem *rect = renderRect(rectx, recty,
			cellsize, cellsize, color);

	if (option == REAL)
		realheatmap[ii][jj] = rect;
	else if (option == OBSERVED)
		obsheatmap[ii][jj] = rect;
}

void Scene::updateRealCounts() {
	int ii, jj;
	for (ii = 0; ii <= 18; ii++) {
		for (jj = 0; jj <= 18; jj++) {
			int i = ii - 9;
			int j = jj - 9;

			int xcenter = i * cellsize + xrad;
			int ycenter = j * cellsize + xrad;

			int  kkk = getRealCount(xcenter, ycenter, xrad, yrad);
			realcount[ii][jj] += kkk;
		}
	}
}

bool Scene::insideWindow(int center[2], int p[2]) {
	int l = center[0] - 1;
	int r = center[0] + 1;
	int t = center[1] - 1;
	int b = center[1] + 1;
	if (p[0] >= l && p[0] <= r && p[1] >= t && p[1] <= b)
		return true;
	else
		return false;
}

void Scene::updateCounts(int option) {
	int i, j, ii, jj;
	int carpos[2], carcell[2], carind[2];

	if (option == OBSERVED) {
		getCarPos(carpos);
		getPointCell(carpos, carcell);
		carind[0] = carcell[0] + 9;
		carind[1] = carcell[1] + 9;
	}

	for (ii = 0; ii <= 18; ii++) {
		for (jj = 0; jj <= 18; jj++) {
			int p[2] = {ii, jj};

			if (option == OBSERVED && !insideWindow(carind, p))
				continue;

			int xcenter = (ii - 9) * cellsize + xrad;
			int ycenter = (jj - 9) * cellsize + xrad;

			int  realc = getRealCount(xcenter, ycenter, xrad, yrad);

			if (option == REAL) {
				realcount[ii][jj] += realc;
			} else if (option == OBSERVED){
				obscount[ii][jj] += realc;
				obsnsamples[ii][jj] ++;
			}
		}
	}
}

void Scene::renderRealDensities() {
	int i, j;
	for (i = 0; i < 18; i++) {
		for (j = 0; j < 18; j++) {
			if (realheatmap[i][j] != NULL)
				scene->removeItem(realheatmap[i][j]);
			renderCellDensity(i, j, REAL);
		}
	}
}

void Scene::renderDensities(int option) {
	int i, j;

	for (i = 0; i < 18; i++) {
		for (j = 0; j < 18; j++) {
			if (option == REAL && realheatmap[i][j])
				scene->removeItem(realheatmap[i][j]);
			else if (option == OBSERVED && obsheatmap[i][j])
				scene->removeItem(obsheatmap[i][j]);

			renderCellDensity(i, j, option);
		}
	}
}

void Scene::updateAndRenderRealDensities() {
	updateCounts(REAL);
	renderDensities(REAL);
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
