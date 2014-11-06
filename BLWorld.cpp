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

// 	world.addObject(2, bl::STATIC_OBJECT, { 1.0f, 4.0f }, (float)M_PI / 2.0f);
// 	world.attachShape(2, poly);

// 	world.setDensity(1, 0.5f);
// 	world.setRestitution(1, 0.5f);
	//world.setFriction(1, 0.5f);
	
	std::cout << world.objectToString(1) << std::endl << std::endl;

	world.applyLinearImpulse(1, {0.0f, 1.0f});
	world.setLinearDamping(1, 0.0f);

	auto ctacts = world.checkContacts(1);

	float stepMSec = 1;
	world.doStep(1000);
	
	std::cout << world.objectToString(1) << std::endl << std::endl;
//	std::cout << world.objectToString(2) << std::endl << std::endl;

	world.doStep(1000);

	std::cout << world.objectToString(1) << std::endl << std::endl;
//	std::cout << world.objectToString(2) << std::endl << std::endl;

	world.doStep(stepMSec);

	std::cout << world.objectToString(1) << std::endl << std::endl;
//	std::cout << world.objectToString(2) << std::endl << std::endl;
	
	world.doStep(stepMSec);

	std::cout << world.objectToString(1) << std::endl << std::endl;
//	std::cout << world.objectToString(2) << std::endl << std::endl;

	world.doStep(stepMSec);
	std::cout << world.objectToString(1) << std::endl << std::endl;
//	std::cout << world.objectToString(2) << std::endl << std::endl;

	world.doStep(stepMSec);
	std::cout << world.objectToString(1) << std::endl << std::endl;
//	std::cout << world.objectToString(2) << std::endl << std::endl;


	return 0;
}

