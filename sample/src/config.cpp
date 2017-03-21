// (c) 2011 by Christian Gloor
// See main.cpp for more information

#include "config.h"

#include <QtGui>

#include <iostream>

using namespace std;

Config::Config() {
    guiShowWaypoints = false;
    simWallForce = 50;
    simPedForce = 30;
    simSpeed = 1000/30;
    mlTendency = false;
    mlLookAhead = true;
    showForces = true;
    showDirection = true;
    showTree = false;
    simh = 0.4;
    followAgent = false;
}

void Config::setGuiShowWaypoints(bool value) {
    guiShowWaypoints = value;
}

void Config::setSimWallForce(double value) {
    simWallForce = value;
}

void Config::setSimPedForce(double value) {
    simPedForce = value;
}

void Config::setSimSpeed(int value) {
    simSpeed = value;
}
