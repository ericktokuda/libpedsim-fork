#ifndef _SCENE_H_
#define _SCENE_H_ 1

#include <iostream>
#include <sstream>
#include "ped_includes.h"
#include "grid.hpp"
#include "car.hpp"
#include "person.hpp"
#include "polygon.hpp"

class FileOutWriter;
class Grid;
class Car;
class Person;

using namespace Ped;
//using namespace std;
using std::vector;
using std::cout;

class Scene : public Tscene {
	// Attributes
	private:
	public:
		int a;
		// These attr are actually from the Grid obj
		int x0, y0, cellw, cellh, nxcells, nycells;
		Grid *grid;

	// Methods
	private:
		//void update_grid();
	public:
		Scene() : Tscene() {};
		void setOutputWriter(OutputWriter *ow);
		void draw_scenario_params(int x0, int y0, int cellw, int cellh, int nxcells, int nycells);
		vector<Car*> get_cars();
		vector<Person*> get_people();
		//FileOutWriter* get_fileoutwriter();
		Scene(int x0, int y0, int cellw, int cellh, int nxcells,
				int nycells);
		void draw_grid(Grid *grid, string name);
		void update_scene_grid();
		void update_sensed_grid();
		void update();
		const vector<Tagent *> get_agents(int x0, int y0, int x1, int y1);
		void add_polygon(Polygon polygon);
		vector<const Person*> get_people_nearby(Tvector pos, double rad);
		void move_agents();
};

#endif
