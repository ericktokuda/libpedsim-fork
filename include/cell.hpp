#ifndef _CELL_H_
#define _CELL_H_ 1

#include <iostream>

class Cell {
	// Attributes
	private:
	public:
		int x, y, w, h, nobj, nsamples;

	// Methods
	public:
		Cell();
		Cell(int x, int y, int w, int h);
		virtual ~Cell();
		int get_area();
		float get_density();
		void increment_nobj();
		void increment_sample();
};
#endif
