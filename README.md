# Pedestrian simulation
Pedestrians flow simulation

## Synopsis
Work in progress of a simulation of pedestrians flow, based on the [work](http://pedsim.silmaril.org/) of Christian Gloor.

## Demo
- ![Screenshot](https://raw.githubusercontent.com/tokudaek/pedestrian-simulation/master/demo.gif)

## Installation
You will need
 * Qt5
 * C++11

In Ubuntu/Debian, it can be done by:

`
aptitude install qt5-default qt5-qmake qt5-style-plugins build-essential
`

## Run
To run it, the easiest way is to just open the `sample/pedsim.pro` in QtCreator.
Your will need to compile it:

~~~~
cd sample
qmake pedsim.pro -r -spec linux-g++-64
make
~~~~

Once compiled, 


`./simulation`



## API Reference

The documentation of this code is available [here](http://pedsim.silmaril.org/documentation/).

## Tests

Tested only on Linux 3.16.0-4-amd64, Debian Jessie.


## License

The core library is licensed under the terms of of the GNU Lesser General Public License (LGPL), according to the [authors](http://pedsim.silmaril.org/download/).
