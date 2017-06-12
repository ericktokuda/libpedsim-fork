#ifndef _GRID_H__
#define _GRID_H__ 1

#include "cell.hpp"
#include "ped_vector.h"
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Ped;

class Grid {
	// Attributes
	private:
	public:
		int x0, y0, cellw, cellh, nxcells, nycells, right, bottom;
		vector<vector<Cell>> cells;
		vector<int> xcoords;
		vector<int> ycoords;

	// Methods
	protected:
		void bin_search(Tvector *pos);
	public:
		Grid() {};
		void init(int x0, int y0, int cellw, int cellh, int nxcells, int nycells);
		Grid(int x0, int y0, int cellw, int cellh, int nxcells, int nycells);
		Grid(const Grid *grid);
		virtual ~Grid ();
		void update_point(const Tvector pos);
		Tvector get_enclosing_cell_idx(const Tvector pos);
		bool contains(const Tvector pos);
		int bin_search(const vector<int> row, const int element);
		vector<Tvector *> get_cells_intersecting_rect(Tvector p1, Tvector p2);
		void print_density_ascii();
		string get_linearized_csv();
		void increment_samples();
		void increment_samples(const Tvector idx);
		double compute_difference(Grid *grid);
};

#endif
