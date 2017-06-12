#include "cell.hpp"

using std::cout;
using std::endl;

Cell::Cell() {
	this->nobj = 0;
}

Cell::Cell(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->nobj = 0;
	this->nsamples = 0;
}

Cell::~Cell() {
	 }

int Cell::get_area() {
	return h*w;
}

float Cell::get_density() {
	if (nsamples == 0)
		return -1.0;
	else
		return ((float) nobj) / nsamples;
}
void Cell::increment_nobj()
{
	this->nobj++;
}

void Cell::increment_sample()
{
	this->nsamples++;
}
