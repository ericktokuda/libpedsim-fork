#ifndef _POLYGON_H_
#define _POLYGON_H_ 1

#include "ped_includes.h"


class Polygon {
	public:
		vector<Ped::Tobstacle *> lines;

	public:
		Polygon(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
		Polygon(vector<Ped::Tvector> points);
};
#endif
