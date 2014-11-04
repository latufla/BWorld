// BLWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src\World.h"
#include <vector>

using namespace bl;

int _tmain(int argc, _TCHAR* argv[]) {
	bl::World world;
	world.addObject(1, bl::DYNAMIC_OBJECT, { 1.0f, 1.0f });

	bl::VertexList poly{
			{ 1.0f, 0.0f },
			{ 1.0f, 2.0f },
			{ 0.0f, 1.0f },
			{ 0.0f, 0.0f }
	};
	world.attachShape(1, poly);

	bl::Point center { 0.0f, 0.0f };
	world.attachShape(1, center, 2.0f);

	auto shapes = world.getDebugShapes(1);
	
	return 0;
}

