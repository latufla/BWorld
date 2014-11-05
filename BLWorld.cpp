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
	world.addObject(1, bl::DYNAMIC_OBJECT, { 30.0f, 30.0f }, 2.0f * M_PI / 3.0f);

	bl::VertexList poly{
			{ 0.5f, -0.5f },
			{ 0.5f, 0.5f },
			{ -0.5f, 0.5f },
			{ -0.5f, -0.5f }
	};
	world.attachShape(1, poly);

// 	bl::Point center { 0.0f, 0.0f };
// 	world.attachShape(1, center, 2.0f);



	bl::DebugShapeList shapes = world.getDebugShapes(1);
	
	bl::DebugPolygon shape = *(DebugPolygon*)(shapes[0].get());

	auto lCoM = world.getLocalCoM(1);
	auto gCoM = world.getGlobalCoM(1);
	auto position = world.getPosition(1);
	auto rotation = world.getRotation(1);
	auto transform = world.getTransform(1);

	std::cout << lCoM.x << " " << lCoM.y << std::endl;
	std::cout << gCoM.x << " " << gCoM.y << std::endl;
	std::cout << position.x << " " << position.y << std::endl;
	std::cout << rotation << std::endl;
	std::cout << Utils::toString(transform);

	return 0;
}

