//
// pedsim - A microscopic pedestrian simulation system. 
// Copyright (c) 2003 - 2012 by Christian Gloor
//                              

#ifndef _scene_h_
#define _scene_h_ 1

#include "ped_scene.h"

#include "tree.h"

class QGraphicsScene;
class QTimer;
class Grid;

using namespace std;

/// Class that descripts an scene object
/// \author  chgloor
/// \date    2012-01-28
class Scene : public QObject, public Ped::Tscene {
	Q_OBJECT

 private:
  QGraphicsScene *scene;
  QTimer *movetimer;
  QTimer *cleanuptimer;
  QTimer *heattimer;
  Grid *grid;
  vector<vector<QGraphicsItem*>> realheatmap;
  vector<vector<QGraphicsItem*>> obsheatmap;
  vector<vector<int>> obscount;
  vector<vector<int>> obsnsamples;
  vector<vector<int>> realcount;

 public slots:
  void moveAllAgents();
  void renderRealDensities();
  void renderObservedDensities();
  void incrementTicks();
  void getPointCell(const int pointpos[2], int indices[2]);
  void cleanupSlot();
  
 public:
//  Scene();
  Scene(QGraphicsScene *scene);

  double getGridValue(double x, double y, int value);

};

#endif
