#include "scene.hpp"

Scene::Scene(int x0, int y0, int cellw, int cellh, int nxcells, int nycells) :
	Tscene(x0, y0, nxcells*cellw, nycells*cellh) {
	grid  = new Grid(x0, y0, cellw, cellh, nxcells, nycells);
	Car::grid->init(x0, y0, cellw, cellh, nxcells, nycells);
	//impassable = vector<vector<vector<int> > >;
}

void Scene::setOutputWriter(OutputWriter *ow) {
	outputwriters.push_back(ow);
	draw_scenario_params(x0, y0, cellw, cellh, nxcells, nycells);
}

bool Scene::is_inside_polygon(vector<int> p, vector<vector<int> > v) {
	bool ispolygon = false;
	int nvert = v.size();
	int i, j;

	for (i = 0, j = nvert-1; i < nvert; j = i++) {
		bool t = (v[i][1] > p[1]) != (v[j][1] > p[1]);
		if (!t) continue;

		double num = (v[j][0] - v[i][0]) * (p[1] - v[i][1]);
		double den = (v[j][1] - v[i][1]);
		double term = v[i][0];

		double a = p[0] < num / den + term;
		if (a) ispolygon = !ispolygon;
	}
	return ispolygon;
}

void Scene::draw_scenario_params (int x0, int y0, int cellw, int cellh, int nxcells, int nycells) {
  std::ostringstream msg;

  msg << "<scenario ";
  msg << "name=\"Density Evaluation\" ";
  msg << "x0=\"" << x0 << "\" ";
  msg << "y0=\"" << y0 << "\" ";
  msg << "cellh=\"" << cellh << "\" ";
  msg << "cellw=\"" << cellw << "\" ";
  msg << "nxcells=\"" << nxcells << "\" ";
  msg << "nycells=\"" << nycells << "\" ";
  msg << "/>" << endl;

  //cout << "hello" << endl;
  for (auto o: outputwriters)
	  o->write(msg.str());
}

vector<Car*> Scene::get_cars() {
	vector<Car*> cars;
	const vector<Tagent *> agents2 = getAllAgents();

	for (Tagent* agent : agents2) {
		if (dynamic_cast<Car*>(agent) != NULL)
			cars.push_back((Car*) agent);
	}
	return cars;
}

vector<Person*> Scene::get_people() {
	vector<Person*> people;
	const vector<Tagent *> agents2 = getAllAgents();

	for (Tagent* agent : agents2) {
		if (dynamic_cast<Person*>(agent) != NULL) //This is not the proper way of doing this
			people.push_back((Person*) agent);
	}
	return people;
}

//FileOutWriter* Scene::get_fileoutwriter() {
	//for (auto o: outputwriters) 
		//if (dynamic_cast<FileOutWriter*>(o) != NULL)
			//return (FileOutWriter*) o;
//}

/**
	It was not supposed to be here, but I couldnt make FileOutWriter class work =(
	@param grid Grid to write
	@param name Identifier of whose is the grid
*/
void Scene::draw_grid(Grid *grid, string name) {
  std::ostringstream msg;

  msg << "<grid ";
  msg << "name=\"" << name << "\" ";
  msg << "values=\"";
  msg << grid->get_linearized_csv();
  msg << "\" />" << endl;
  for (auto o: outputwriters)
	  o->write(msg.str());
}

void Scene::update_scene_grid() {
	const vector<Person *> agents = get_people();

	 for (Tagent* agent : agents) {
		 Tvector agpos = agent->getPosition();
		 if (grid->contains(agpos))
			 grid->update_point(agpos);
	 }
	 grid->increment_samples();
	 draw_grid(grid, "real");
	 //ow->draw_grid(grid);
	 //for (auto o: outputwriters) o->write("hello");

	 //cout << "Real density" << endl;
	 //grid->print_density_ascii();
}

void Scene::update_sensed_grid() {
	 vector<Car *> cars  = get_cars();
	 for (auto car: cars) car->update();
	 //ow->draw_grid(Car::grid);
	 draw_grid(Car::grid, "car");

	 //cout << "Sensed density" << endl;
	 //Car::grid->print_density_ascii();
}

void Scene::update() {
    timestep++;
	for (auto ow: outputwriters) ow->writeTimeStep(timestep);
	update_scene_grid();
	update_sensed_grid();
	double xx = grid->compute_difference(Car::grid);
	cout << xx << endl;
}

void Scene::add_polygon(vector<vector<int>> v) {
	for (int i = 0; i < v.size() - 1; i++)
		addObstacle(new Ped::Tobstacle(v[i][0], v[i][1], v[i+1][0], v[i+1][1]));

	addObstacle(new Ped::Tobstacle(v.back()[0], v.back()[1], v[0][0], v[0][1]));
}

void Scene::add_impassable_region(vector<vector<int>> v) {
	this->impassable.push_back(v);
}

bool Scene::is_passable(vector<int> p) {
	for (auto v: this->impassable) {
		if (is_inside_polygon(p, v)) return false;
	}
	return true;
}

vector<const Person*> Scene::get_people_nearby(Tvector pos, double rad) {
	set<const Tagent *>  agents = getNeighbors(pos.x, pos.y, rad);
	vector<const Person*>  people;

	for (auto agent : agents) {
		if (dynamic_cast<const Person*>(agent) != NULL) //This is not the proper way of doing this
			people.push_back((const Person*) agent);
	}
	return people;
}

void Scene::move_agents() {
    for (Tagent* agent : agents) agent->computeForces();

    for (Tagent* agent : agents) agent->move(1.0);

    // then output their new position if an OutputWriter is given.
	for (auto ow : outputwriters) {
	  for (Tagent* agent : agents) ow->drawAgent(*agent);
	  //ow->draw_cars();
	  //ow->draw_people();
	}
}
