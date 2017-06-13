#include "ped_includes.h"
#include "scene.hpp"
#include "car.hpp"
#include "person.hpp"

#include <iostream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <fstream>

using std::cout;
using namespace Ped;

const int MAPRAD = 80;
const int BLOCKRAD = 60;
const int INNBLOCKRAD = 40;
const int L = -BLOCKRAD;
const int R = -L, T = L, B = -L;
const int LL = -INNBLOCKRAD;
const int RR = -LL, TT = LL, BB = -LL;
const int RAD = 2;
const int STREETW = 20;
const int HSTREETW = STREETW / 2;
const float PEDMAXSPEED = 1.5;
const int CARNUM = 3;
const float CARMAXSPEED = 2.0;
const int CARSAMPFREQ = 10;
const int CARRAD = 10;
const int PAUSE = 5;
const int CELLW = 10;
const int CELLH = 10;
const int NXCELLS = 20;
const int NYCELLS = 20;

/**
	Get pseudo-random number between a and b. It assumes b>=a
	@param a Lower boundary
	@param b Higher boundary
	@return int Random element
*/
int get_random_integer(int a, int b) {
	srand((unsigned) time(NULL));
	int mod = b - a + 1;
	int ret = (rand()%mod) + a;
	return ret;
}

double get_random_real(double a, double b) {
	srand((unsigned) time(NULL));
	float ret = a + (double) (rand()) /( (double) (RAND_MAX/(b-a)));
	return ret;
}

/**
	Create a scene including the obstacles and outputwriters
	@return Scene*
*/
Scene* build_scene() {
	OutputWriter *ow = new UDPOutputWriter();
	//ow->setScenarioName("Density Estimation");

	Scene *scene = new Scene(-MAPRAD, -MAPRAD, CELLW, CELLH, NXCELLS, NYCELLS);
	scene->setOutputWriter(ow);
	FileOutputWriter *ow2 = new FileOutputWriter();
	scene->setOutputWriter(ow2);

	//scene->add_polygon(Polygon(L, B, L, T, R, T, R, B));
	scene->add_polygon(Polygon(LL, BB, LL, TT, RR, TT, RR, BB));

	return scene;
}

/**
	Insert a set of people in the inner part
	@param scene Where people will be inserted
*/
void insert_inner_people(Scene *scene) {
	int shift = (int) (0.8 * STREETW);
	Twaypoint *w1 = new Twaypoint( L + shift, T + shift, RAD);
	Twaypoint *w2 = new Twaypoint( R - shift, T + shift, RAD);
	Twaypoint *w3 = new Twaypoint( R - shift, B - shift, RAD);
	Twaypoint *w4 = new Twaypoint( L + shift, B - shift, RAD);

	for (int i = 0; i < 30; i++) {
		float maxspeed = PEDMAXSPEED;
		float agmaxspeed = (float)(rand())/RAND_MAX * maxspeed + 0.01;
		Tagent *a = new Person();
		a->setWaypointBehavior(Tagent::BEHAVIOR_CIRCULAR);
		a->setVmax(agmaxspeed); // same speed for all agents
		a->setfactorsocialforce(10.0);
		a->setfactorobstacleforce(1.0);

		a->addWaypoint(w1);
		a->addWaypoint(w2);
		a->addWaypoint(w3);
		a->addWaypoint(w4);

		if (a->getid() % 2 == 0) {
			a->setPosition(-80, -25 + a->getid(), 0);
		} else {
			a->setPosition( 80, -25 + a->getid(), 0);
		}

		scene->addAgent(a);
	}
}

/**
	Insert a set of people in the outer part
	@param scene Where people will be inserted
*/
void insert_outer_people(Scene *scene) {
	int shift = (int) (0.2 * STREETW);
	Twaypoint *w1 = new Twaypoint( L + shift, B - shift, RAD);
	Twaypoint *w2 = new Twaypoint( R - shift, B - shift, RAD);
	Twaypoint *w3 = new Twaypoint( R - shift, T + shift, RAD);
	Twaypoint *w4 = new Twaypoint( L + shift, T + shift, RAD);

	// create agents
	for (int i = 0; i < 30; i++) {
		float agmaxspeed = (float)(rand())/RAND_MAX * PEDMAXSPEED + 0.01;
		Tagent *a = new Person();
		a->setWaypointBehavior(Tagent::BEHAVIOR_CIRCULAR);
		a->setVmax(agmaxspeed); // same max speed for all agents
		a->setfactorsocialforce(10.0);
		a->setfactorobstacleforce(1.0);

		a->addWaypoint(w1);
		a->addWaypoint(w2);
		a->addWaypoint(w3);
		a->addWaypoint(w4);

		if (a->getid() % 2 == 0) {
			a->setPosition(-80, -25 + a->getid(), 0);
		} else {
			a->setPosition( 80, -25 + a->getid(), 0);
		}

		scene->addAgent(a);
	}

}

/**
	Insert people in the scene
	@param scene Where people will be inserted
*/
void insert_people(Scene *scene) {
	insert_inner_people(scene);
	insert_outer_people(scene);
}

/**
	Insert cars in the scene
	@param scene Where cars will be inserted
*/
void insert_cars(Scene *scene) {
	int shift = HSTREETW;
	Twaypoint *w1 = new Twaypoint( L + shift, B - shift, RAD);
	Twaypoint *w2 = new Twaypoint( R - shift, B - shift, RAD);
	Twaypoint *w3 = new Twaypoint( R - shift, T + shift, RAD);
	Twaypoint *w4 = new Twaypoint( L + shift, T + shift, RAD);

	for (int i = 0; i < CARNUM; i++) {
		float carmaxspeed = get_random_real(1.0, CARMAXSPEED);

		Car *car = new Car(scene, CARSAMPFREQ, CARRAD);
		car->setWaypointBehavior(Tagent::BEHAVIOR_CIRCULAR);  // only once
		car->setVmax(carmaxspeed); // same max speed for all agents
		car->setfactorsocialforce(10.0);
		car->setfactorobstacleforce(1.0);

		car->addWaypoint(w1);
		car->addWaypoint(w2);
		car->addWaypoint(w3);
		car->addWaypoint(w4);

		car->setPosition(L + shift, T + shift + i*get_random_integer(2, 50), 0);
		scene->addAgent(car);
	}
}

/**
	Insert all the agents in the scene
	@param scene Place to insert them
*/
void populate_scene(Scene *scene) {
	insert_people(scene);
	insert_cars(scene);
}

/**
	Deletes the scene and all the objects
	@param scene 
*/
void cleanup(Scene *scene)
{
	for (auto a : scene->getAllAgents()) { delete a; };
	for (auto w : scene->getAllWaypoints()) { delete w; };
	for (auto o : scene->getAllObstacles()) { delete o; };
	delete scene->grid;
	delete scene;
}

/**
	Where action happens
	@param scene
*/
void main_loop(Scene *scene) {
	long int tick = 0;

	while (true) {
		cout << "Tick " << tick << endl;
		tick++;
		//scene->moveAgents(1);
		scene->move_agents();
		this_thread::sleep_for(chrono::milliseconds(PAUSE));
		scene->update();
	}
	//cout << "# " << h << " " << timestep << endl;
	//ow->writeMetrics({
	//{"Max Timestep", std::to_string(timestep)},
	//{"Bottleneck Width", std::to_string(2*(5-h))} // int for now
	//});
}

int main(int argc, char *argv[]) {
	Scene *scene = build_scene();
	populate_scene(scene);
	main_loop(scene);
	cleanup(scene);
	return EXIT_SUCCESS;   
}


