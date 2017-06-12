#include "grid.hpp"
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include <sstream>

Grid::Grid(int x0, int y0, int cellw, int cellh, int nxcells, int nycells) {
	init(x0, y0, cellw, cellh, nxcells, nycells);
}

Grid::Grid (const Grid *grid) {
	this->init(grid->x0, grid-> y0, grid-> cellw, grid-> cellh,
			grid->nxcells, grid->nycells);
}

Grid::~Grid () {
}

/**
	Initializer of the class. Created to post-initialize a static object
	@param x0 x-coordinate
	@param y0 y-coordinate
	@param cellw Cell width
	@param cellh Cell height
	@param nxcells Number of x-axis cells
	@param nycells Number of y-axis cells
*/
void Grid::init(int x0, int y0, int cellw, int cellh, int nxcells, int nycells) {
	this->cells = vector<vector<Cell>>(nxcells, vector<Cell>(nycells));

	this->x0 = x0;
	this->y0 = y0;
	this->cellw = cellw;
	this->cellh = cellh;
	this->nxcells = nxcells;
	this->nycells = nycells;

	this->xcoords = vector<int> (nxcells);
	this->ycoords = vector<int> (nycells);
	this->right = x0 + cellw*nxcells;
	this->bottom = y0 + cellh*nycells;

	for (int i = 0; i < nxcells; i++) {
		xcoords[i] = x0 + i*cellw;
		for (int j = 0; j < nycells; j++) cells[i][j].x = xcoords[i];
	}
	for (int j = 0; j < nycells; j++) {
		ycoords[j] = y0 + j*cellh;
		for (int i = 0; i < nxcells; i++) cells[i][j].y = ycoords[j];
	}
}

/**
    Update grid according to the existence of an agent
	@param pos Position
*/
void Grid::update_point(const Tvector pos) {
	Tvector idx =  get_enclosing_cell_idx(pos);
	this->cells[idx.x][idx.y].increment_nobj();
}

/**
	Increment number of samples of all cells
*/
void Grid::increment_samples() {
	for (int i = 0; i < xcoords.size(); i++)
		for (int j = 0; j < ycoords.size(); j++)
			cells[i][j].increment_sample();
}

/**
	Increment number of samples of the cell enclosing the \p pos
	@param pos Position of the point
*/
void Grid::increment_samples(const Tvector pos) {
	Tvector idx = get_enclosing_cell_idx(pos);
	cells[idx.x][idx.y].increment_sample();
}


/**
	Check if a point is inside the boundaries of the grid
	@return bool True if point is inside the grid
*/
bool Grid::contains(const Tvector pos) {
	if (pos.x >= x0 && pos.y >= y0 && pos.x <= right && pos.y <= bottom)
		return true;
	else
		return false;
}

/**
	Get index of the enclosing cell
	@param pos Position of the point
	@return Tvector Index
*/
Tvector Grid::get_enclosing_cell_idx(const Tvector pos) {
	Tvector cellidx;
	cellidx.x = bin_search(xcoords, pos.x);
	cellidx.y = bin_search(ycoords, pos.y);
	//cellidx.x = bin_search(x, pos.x);
	return cellidx;
}

/**
	Binary search
	@param row The list of numbers
	@param element Element being search
	@return int Lower boundary of the bin containing \element. -1 if not found.
*/
int Grid::bin_search(const vector<int> row, const int element) {
	int a = 0;
	int b = row.size() - 1;
	int m;
	int end = row[b] + (row[b]- row[b-1]);

	if (element < row[0] || element > end)
		return -1;
	else if (row[b] <= element && element < row[end])
		return b;

	int it = 0;
	int x;
	while (b - a > 1) {
		m = (b + a)/2;
		if (row[m] == element)
			return m;
		else if (row[m] < element)
			a = m;
		else
			b = m;
		it++;
	}

	return a;
}


/**
	Get our cells that intersect the input rectangle
	@param p1 NW corner of the rectangle
	@param p2 SE corner of the rectangle
	@return vector<Tvector*> Set of the indices of the rectangles
*/
vector<Tvector *> Grid::get_cells_intersecting_rect(Tvector p1, Tvector p2) {
	vector<Tvector *> cellsids;
	Tvector cell1idx, cell2idx;

	bool found = false;
	for (int xx = p1.x; xx <= p2.x; xx+=cellw) {
		for (int yy = p1.y; yy <= p2.y; yy+=cellh) {
			if (this->contains(p1)) {
				cell1idx = get_enclosing_cell_idx(Tvector(xx, yy));
				found = true;
				break;
			}
		}
		if (found) break;
	}

	found = false;
	for (int xx = p2.x; xx >= p1.x; xx-=cellw) {
		for (int yy = p2.y; yy >= p1.y; yy-=cellh) {
			if (this->contains(p1)) {
				cell2idx = get_enclosing_cell_idx(Tvector(xx, yy));
				found = true;
				break;
			}
		}
		if (found) break;
	}

	for (int i = cell1idx.x; i <= cell2idx.x; i++)
		for (int j = cell1idx.y; j <= cell2idx.y; j++)
			cellsids.push_back(new Tvector(i, j));
	return cellsids;
}

/**
	Output the grid in ASCII to stdout
*/
void Grid::print_density_ascii() {

	for (int j = 0; j < ycoords.size(); j++) {
		for (int i = 0; i < xcoords.size(); i++) {
			double dens = cells[i][j].get_density();
			if (dens < 0) {
				cout << std::fixed << std::setw(4) << std::setprecision(2) << "     ";
			} else {
				string space = "";
				if (dens <= 10) space = " ";
				cout << space << std::fixed << std::setw(4) << std::setprecision(2) << dens;
			}
			if (i < xcoords.size() - 1) cout << ",";
		}
			cout << endl;
	}
			cout << endl;
}

/**
	Output the grid in ASCII to stdout
*/
string Grid::get_linearized_csv() {

	ostringstream csv("");
	for (int j = 0; j < ycoords.size(); j++) {
		for (int i = 0; i < xcoords.size(); i++) {
			double dens = cells[i][j].get_density();
			csv << dens << ",";
		}
	}
	string ret = csv.str();

	return ret.substr(0, ret.size()-1);
}

double Grid::compute_difference(Grid *grid) {
	double sum = 0;
	int m = 0;
	for (int i = 0; i < nxcells; i++) {
		for (int j = 0; j < nycells; j++) {
			double d1 = cells[i][j].get_density();
			double d2 = grid->cells[i][j].get_density();
			if (d1 != -1 && d2 != -1) {
				sum += std::abs(d1-d2);
				m++;
			}
		}
	}

	if (m == 0) return -1;

	return sum / m;
}
