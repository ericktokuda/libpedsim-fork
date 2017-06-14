#include "polygon.hpp"

Polygon::Polygon(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
	lines.push_back(new Ped::Tobstacle(x1, y1, x2, y2));
	lines.push_back(new Ped::Tobstacle(x2, y2, x3, y3));
	lines.push_back(new Ped::Tobstacle(x3, y3, x4, y4));
	lines.push_back(new Ped::Tobstacle(x4, y4, x1, y1));
}

Polygon::Polygon(vector<Ped::Tvector> p) {
	for (int i = 0; i < p.size() - 1; i++)
		lines.push_back(new Ped::Tobstacle(p[i].x, p[i].y, p[i+1].x, p[i+1].y));

	lines.push_back(new Ped::Tobstacle(p.back().x, p.back().y, p[0].x, p[0].y));
}
