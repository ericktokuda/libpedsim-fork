#include "car.hpp"

Grid* Car::grid  = new Grid();

Car::Car(Scene *scene, int sampfreq, int radius) : Ped::Tagent() {
	this->scene = scene;
	this->sampfreq = sampfreq;
	this->radius = radius;
	setType(Ped::car);
}

/**
	Samples the neighbourhood, considering the object radius
*/
void Car::capture() {
	vector<const Person*> people = scene->get_people_nearby(this->p, radius);

	for (const Ped::Tagent* agent : people) {
		Ped::Tvector agpos = agent->getPosition();
		if (Car::grid->contains(agpos))
			Car::grid->update_point(agpos);
	}

	Ped::Tvector p1, p2;
	rect(this->p, radius, p1, p2);
	vector<Ped::Tvector *> cellsids = Car::grid->get_cells_intersecting_rect(p1, p2);

	for (auto p: cellsids) Car::grid->cells[p->x][p->y].nsamples++;

	//Car::grid->print_density_ascii();
}

void Car::update() {
	capture();
}

void Car::rect(Ped::Tvector p, int radius,
		Ped::Tvector &p1, Ped::Tvector &p2) {
	p1.x = p.x - radius;
	p1.y = p.y - radius;
	p2.x = p.x + radius;
	p2.y = p.y + radius;
}
