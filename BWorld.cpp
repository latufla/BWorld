// BLWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src\World.h"
#include <vector>
#include <iostream>
#include "src\Utils.h"
#include "src\exceptions\Exception.h"


void run();
void handleExceptions();

int _tmain(int argc, _TCHAR* argv[]) {
	try {
		run();
	} catch(...) {
		handleExceptions();
	}
	
	return 0;
}

void run() {
	bw::World world;
	world.addObject(1, bw::DYNAMIC_OBJECT, {1.0f, 1.0f}, (float)M_PI / 2.0f);
	world.attachShape(1, {0.0f, 0.0f}, 3);
	world.setRestitution(1, 0.5f);

	world.addObject(2, bw::DYNAMIC_OBJECT, {1.0f, 6.0f}, (float)M_PI / 2.0f);
	bw::VertexList poly{
			{1.0f, -1.0f},
			{1.0f, 1.0f},
			{-1.0f, 1.0f},
			{-1.0f, -1.0f}
	};
	world.attachShape(2, poly);
	world.setRestitution(2, 0.5f);

	world.applyLinearImpulse(1, {0.0f, 60.0f});

	auto shapes = world.getDebugLocalShapes(1);
	auto shapes2 = world.getDebugLocalShapes(2);

	const float stepSec = 1.0f / 60.0f;
	for(uint32_t i = 0; i < 6; i++) {
		world.doStep(stepSec);
		std::cout << world.objectToString(1) << std::endl;
		std::cout << world.objectToString(2) << std::endl << std::endl;
	}
}

void handleExceptions() {
	std::string error = "";
	try {	
		throw;
	} catch(bw::Exception& e) {
		error = e.msg();
	} catch(std::exception& e) {
		error = e.what();
	} catch(...) {
		error = "unknown exception";
	}

	if(error != "") {
		std::cout << error;
		std::exit(1);
	}
}
