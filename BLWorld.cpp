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
	world.addObject(1, bl::DYNAMIC_OBJECT, { 1.0f, 1.0f }, M_PI / 2.0f);

	bl::VertexList poly{
			{ 0.5f, -0.5f },
			{ 0.5f, 0.5f },
			{ -0.5f, 0.5f },
			{ -0.5f, -0.5f }
	};
	world.attachShape(1, poly);

	world.addObject(2, bl::DYNAMIC_OBJECT, { 1.0f, 3.0f }, M_PI / 2.0f);
	world.attachShape(2, poly);


	bl::DebugShapeList shapes = world.getDebugShapes(1);
	
	bl::DebugPolygon shape = *(DebugPolygon*)(shapes[0].get());

	std::cout << world.objectToString(1) << std::endl << std::endl;

	world.applyLinearImpulse(1, {0.0f, 1.0f});
	
	float stepMSec = 1000.0f / 60.0f;
	world.doStep(500);
	
	world.doStep(500);
	
	world.doStep(500);

	return 0;
}

