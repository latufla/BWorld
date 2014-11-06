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
	world.setRestitution(1, 1.0f);


	world.addObject(2, bl::DYNAMIC_OBJECT, { 1.0f, 2.3f }, (float)M_PI / 2.0f);
	bl::VertexList poly2{
			{ .1f, -.1f },
			{ .1f, .1f },
			{ -.1f, .1f },
			{ -.1f, -.1f }
	};
	world.attachShape(2, poly2);
	world.setRestitution(2, 1.0f);
		
	world.applyLinearImpulse(1, { 0.0f, 10.0f });

	float stepSec = 1.0f / 60.0f;

	for (uint32_t i = 0; i < 6; i++) {
		world.doStep(stepSec);
		std::cout << world.objectToString(1) << std::endl;
		std::cout << world.objectToString(2) << std::endl << std::endl;
	}
	return 0;
}

