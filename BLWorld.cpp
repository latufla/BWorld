// BLWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src\World.h"
#include <vector>
#include <iostream>
#include "src\Utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace bl;

int _tmain(int argc, _TCHAR* argv[]) {
	bl::World world;
	world.addObject(1, bl::DYNAMIC_OBJECT, { 1.0f, 1.0f }, (float)M_PI / 2.0f);

	bl::VertexList poly{
			{ 1.0f, -1.0f },
			{ 1.0f, 1.0f },
			{ -1.0f, 1.0f },
			{ -1.0f, -1.0f }
	};
	world.attachShape(1, poly);

	std::cout << world.objectToString(1) << std::endl << std::endl;

	world.applyLinearImpulse(1, {0.0f, 1.0f});
	world.setLinearDamping(1, 1.0f);

	auto ctacts = world.checkContacts(1);

	float stepMSec = 1000.0f;
	world.doStep(stepMSec);
	
	std::cout << world.objectToString(1) << std::endl << std::endl;

	world.doStep(stepMSec);

	std::cout << world.objectToString(1) << std::endl << std::endl;

	return 0;
}

