#include "PhysicsApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsApp();

	// initialise and loop
	app->setVSync(false);
	app->run("DIE BY THE PUTTER", 1280, 720, false);
	// deallocation
	delete app;

	return 0;
}