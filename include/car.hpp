#ifndef _CAR_H_
#define _CAR_H_ 1

#include "ped_includes.h"
#include <iostream>
#include "scene.hpp"
#include "grid.hpp"

using std::cout;

class FileOutWriter;
class Scene;
//using namespace std;

class Car : public Ped::Tagent {
	// Attributes
	private:
		int sampfreq;
		int radius;
	public:
		Scene *scene;
		static Grid *grid;

	// Methods
	private:
	protected:
		void rect(int x0, int y0, int radius, int &x1, int &y1, int &x2, int &y2);
		void rect(Ped::Tvector p, int radius, Ped::Tvector &p1, Ped::Tvector &p2);
	public:
		Car(Scene *scene, int samp, int radius);
		void capture();
		void update();
};

#endif
