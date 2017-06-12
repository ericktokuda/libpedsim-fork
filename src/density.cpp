//
// pedsim - A microscopic pedestrian simulation system.
// Copyr (c) by Christian Gloor
//
// To collect the output in a file::
// ./example05 > out.dat
//
// Process output in gnuplot:
// gnuplot> plot "out.dat"

#include "ped_includes.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <thread>
using namespace std;

enum AgentType { PERSON, CAR };

int mapwidth = 100;
int l = -mapwidth;
int r = -l, t = l, b = -l;
int rad = 2;
int streetw = 30;
int hstreetw = streetw / 2;


Ped::Tscene* build_scene()
{
	cout << "# PedSim Example using libpedsim version " << Ped::LIBPEDSIM_VERSION << endl;



	Ped::Tscene *pedscene = new Ped::Tscene(); // no quadtree

	// create an output writer which will send output to a visualizer

	Ped::OutputWriter *ow = new Ped::UDPOutputWriter();
	//Ped::OutputWriter *ow = new Ped::FileOutputWriter();
	ow->setScenarioName("Example 05 / Dynamic Obstacles");
	pedscene->setOutputWriter(ow);


	// add one waypoint (=destination) with a small radius of 10 at the r end.

	//Ped::Twaypoint *w1 = new Ped::Twaypoint(l + hstreetw, t + hstreetw, rad);
	//Ped::Twaypoint *w2 = new Ped::Twaypoint(r - hstreetw, b - hstreetw, rad);

	// create and add obstacles

	pedscene->addObstacle(new Ped::Tobstacle(l, b, l, t));
	pedscene->addObstacle(new Ped::Tobstacle(l, t, r, t));
	pedscene->addObstacle(new Ped::Tobstacle(r, t, r, b));
	pedscene->addObstacle(new Ped::Tobstacle(r, b, l, b));

	int ll = l + streetw;
	int rr = -ll, tt = ll, bb = -ll;

	//dynamic obstacles
	Ped::Tobstacle *do1 = new Ped::Tobstacle(ll, bb, ll, tt);
	pedscene->addObstacle(do1);
	Ped::Tobstacle *do2 = new Ped::Tobstacle(ll, tt, rr, tt);
	pedscene->addObstacle(do2);
	Ped::Tobstacle *do3 = new Ped::Tobstacle(rr, tt, rr, bb);
	pedscene->addObstacle(do3);
	Ped::Tobstacle *do4 = new Ped::Tobstacle(rr, bb, ll, bb);
	pedscene->addObstacle(do4);
	return pedscene;
}

void insert_inner_people(Ped::Tscene *pedscene) {
	int shift = (int) (0.8 * streetw);
	Ped::Twaypoint *w1 = new Ped::Twaypoint( l + shift, t + shift, rad);
	Ped::Twaypoint *w2 = new Ped::Twaypoint( r - shift, t + shift, rad);
	Ped::Twaypoint *w3 = new Ped::Twaypoint( r - shift, b - shift, rad);
	Ped::Twaypoint *w4 = new Ped::Twaypoint( l + shift, b - shift, rad);

	// create agents
	for (int i = 0; i<30; i++) {
		float maxspeed = 1.5;
		float agmaxspeed = (float)(rand())/RAND_MAX * maxspeed + 0.01;
		Ped::Tagent *a = new Ped::Tagent();
		a->setWaypointBehavior(Ped::Tagent::BEHAVIOR_CIRCULAR);  // only once
		a->setVmax(agmaxspeed); // same speed for all agents
		a->setfactorsocialforce(10.0);
		a->setfactorobstacleforce(1.0);
		a->setType((int) PERSON);

		a->addWaypoint(w1);
		a->addWaypoint(w2);
		a->addWaypoint(w3);
		a->addWaypoint(w4);

		if (a->getid() % 2 == 0) {
			a->setPosition(-80, -25 + a->getid(), 0);
		} else {
			a->setPosition( 80, -25 + a->getid(), 0);
		}

		pedscene->addAgent(a);
	}

	// convenience
	//const vector<Ped::Tagent*>& myagents = pedscene->getAllAgents();
	//const vector<Ped::Tobstacle*>& myobstacles = pedscene->getAllObstacles();

	// reset agents
	//for (vector<Ped::Tagent*>::const_iterator it = myagents.begin(); it != myagents.end(); ++it) {
		//(*it)->addWaypoint(w1);
		//(*it)->addWaypoint(w2);
		//(*it)->addWaypoint(w3);
		//(*it)->addWaypoint(w4);
		//if ((*it)->getid() % 2 == 0) {
			//(*it)->setPosition(-80, -25 + (*it)->getid(), 0);
		//} else {
			//(*it)->setPosition( 80, -25 + (*it)->getid(), 0);
		//}
	//}

}

void insert_outer_people(Ped::Tscene *pedscene) {
	int shift = (int) (0.2 * streetw);
	Ped::Twaypoint *w1 = new Ped::Twaypoint( l + shift, b - shift, rad);
	Ped::Twaypoint *w2 = new Ped::Twaypoint( r - shift, b - shift, rad);
	Ped::Twaypoint *w3 = new Ped::Twaypoint( r - shift, t + shift, rad);
	Ped::Twaypoint *w4 = new Ped::Twaypoint( l + shift, t + shift, rad);

	// create agents
	for (int i = 0; i<50; i++) {
		float maxspeed = 1.5;
		float agmaxspeed = (float)(rand())/RAND_MAX * maxspeed + 0.01;
		Ped::Tagent *a = new Ped::Tagent();
		a->setWaypointBehavior(Ped::Tagent::BEHAVIOR_CIRCULAR);  // only once
		a->setVmax(agmaxspeed); // same speed for all agents
		a->setfactorsocialforce(10.0);
		a->setfactorobstacleforce(1.0);
		a->setType((int) PERSON);


		a->addWaypoint(w1);
		a->addWaypoint(w2);
		a->addWaypoint(w3);
		a->addWaypoint(w4);

		if (a->getid() % 2 == 0) {
			a->setPosition(-80, -25 + a->getid(), 0);
		} else {
			a->setPosition( 80, -25 + a->getid(), 0);
		}


		pedscene->addAgent(a);
	}


}
void insert_people(Ped::Tscene *pedscene) {
	insert_inner_people(pedscene);
	insert_outer_people(pedscene);
}

void insert_cars(Ped::Tscene *pedscene) {

	int shift = hstreetw;
	Ped::Twaypoint *w1 = new Ped::Twaypoint( l + shift, b - shift, rad);
	Ped::Twaypoint *w2 = new Ped::Twaypoint( r - shift, b - shift, rad);
	Ped::Twaypoint *w3 = new Ped::Twaypoint( r - shift, t + shift, rad);
	Ped::Twaypoint *w4 = new Ped::Twaypoint( l + shift, t + shift, rad);

	// create agents
	for (int i = 0; i<1; i++) {
		float maxspeed = 2.0;
		float agmaxspeed = (float)(rand())/RAND_MAX * maxspeed + 1.5;
		//cout << rand();
		Ped::Tagent *a = new Ped::Tagent();
		a->setWaypointBehavior(Ped::Tagent::BEHAVIOR_CIRCULAR);  // only once
		a->setVmax(agmaxspeed); // same speed for all agents
		a->setfactorsocialforce(10.0);
		a->setfactorobstacleforce(1.0);
		a->setType((int) CAR);

		a->addWaypoint(w1);
		a->addWaypoint(w2);
		a->addWaypoint(w3);
		a->addWaypoint(w4);

			//a->setPosition(-80, -25 + a->getid(), 0);
			a->setPosition(l + shift, t + shift, 0);

		pedscene->addAgent(a);
	}

	return;
}

void populate_scene(Ped::Tscene *pedscene) {
	insert_people(pedscene);
	insert_cars(pedscene);
}

void cleanup(Ped::Tscene *pedscene)
{
	for (auto a : pedscene->getAllAgents()) { delete a; };
	for (auto w : pedscene->getAllWaypoints()) { delete w; };
	for (auto o : pedscene->getAllObstacles()) { delete o; };
	delete pedscene;
}

void main_loop(Ped::Tscene *pedscene) {
	long int timestep = 0;
	const vector<Ped::Tagent*>& myagents = pedscene->getAllAgents();

	int notreached = myagents.size();
	//while (notreached > 0) {
	while (true) {
		timestep++;
		notreached = myagents.size();
		pedscene->moveAgents(0.8);

		for (auto a : myagents) {
			if (a->reachedDestination()) {
				notreached--;
			}
		}
		if (timestep >= 200000) notreached = 0; // seems to run forever.
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	//cout << "# " << h << " " << timestep << endl;
	//ow->writeMetrics({
	//{"Max Timestep", std::to_string(timestep)},
	//{"Bottleneck Width", std::to_string(2*(5-h))} // int for now
	//});

	//}
}

int main(int argc, char *argv[]) {
	Ped::Tscene *pedscene = build_scene();
	populate_scene(pedscene);
	main_loop(pedscene);
	cleanup(pedscene);
	return EXIT_SUCCESS;   
}


