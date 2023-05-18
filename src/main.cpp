#include "main.h"
#include "echolib/graphics/GraphicalUserInterface.hpp"
#include <string>

void initialize() {
	
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {

	GraphicalUserInterface gui = GraphicalUserInterface("Echo",0);

	while (true) {
		pros::delay(30);
	}
}
