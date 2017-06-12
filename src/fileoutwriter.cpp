#include "fileoutwriter.hpp"
#include <sstream>
#include <ostream>
#include <fstream>
#include "ped_includes.h"
#include "grid.hpp"

using namespace std;
using namespace Ped;


/**
	Overwrites parent class' method
	@param a Agent to be drawn
*/

//void FileOutWriter::drawAgent(Tagent &a) {
  //std::ostringstream msg;
  //string agtype;
  //if (dynamic_cast<Car*>(&a) != NULL)
	  //agtype = "car";
  //else
	  //agtype = "person";

  //msg << "<position type=\"agent\" ";
  //msg << "agtype=\""<< agtype << "\" ";
  //msg << "id=\"" << a.getid() << "\" ";
  //msg << "x=\"" << a.getPosition().x << "\" ";
  //msg << "y=\"" << a.getPosition().y << "\" ";
  //msg << "/>" << endl;
  //write(msg.str());
//}

//void FileOutWriter::draw_grid(Grid *grid, string name) {
  //std::ostringstream msg;

  //msg << "<grid ";
  //msg << "name=\"" << name << "\" ";
  //msg << grid->get_linearized_csv() << "\" ";
  //msg << "/>" << endl;
  //write(msg.str());

//}
