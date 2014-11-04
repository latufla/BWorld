#include "stdafx.h"
#include "World.h"

using std::vector;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;

namespace bl{
	World::World()
		: world({ 0.0f, 0.0f }) {
	}

	bool World::addObject(uint32_t id, ObjectType tp, const Point& pos) {
		b2BodyDef bodyDef;
		bodyDef.type = static_cast<b2BodyType>(tp);
		bodyDef.position.Set(pos.x, pos.y);
		auto body = world.CreateBody(&bodyDef);
		if (!body)
			return false;

		auto res = idToObject.emplace(id, body);
		return res.second;
	}

	void World::removeObject(uint32_t id) {
		auto body = idToObject.at(id);
		world.DestroyBody(body);

		idToObject.erase(id);
	}

	void World::attachShape(uint32_t toObj, const Point& center, float radius) {
		auto b = idToObject.at(toObj);

		b2CircleShape shape;
		shape.m_radius = radius;
		shape.m_p.Set(center.x, center.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		b->CreateFixture(&fixtureDef);
	}

	void World::attachShape(uint32_t toObj, std::vector<Point>& vertices) {
		b2Body* b = idToObject.at(toObj);
		b2PolygonShape shape;

		uint32_t vCount = vertices.size();
		b2Vec2* verticesB2 = new b2Vec2[vCount];
		for (uint32_t i = 0; i < vCount; i++) {
			Point& vertex = vertices.at(i);
			verticesB2[i] = { vertex.x, vertex.y };
		}
		shape.Set(verticesB2, vCount);
		delete[] verticesB2;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		b->CreateFixture(&fixtureDef);
	}

	DebugShapeList World::getDebugShapes(uint32_t fromObj) {
		b2Body* b = idToObject.at(fromObj);
	
		DebugShapeList res;
		b2Fixture* fixture = b->GetFixtureList();
		while (fixture) {
			b2Shape* shapeB2 = fixture->GetShape();
			b2Shape::Type shapeType = shapeB2->GetType();
			if (shapeType == b2Shape::e_circle){
				b2CircleShape* circleB2 = (b2CircleShape*)shapeB2;
				b2Vec2& pos = circleB2->m_p;

				auto circle = make_unique<DebugCircle>(Point{ pos.x, pos.y }, circleB2->m_radius);
				res.push_back(std::move(circle));
			}
			else if (shapeType == b2Shape::e_polygon) {
 				b2PolygonShape* polyB2 = (b2PolygonShape*)shapeB2;
				vector<Point> vertices;
				uint32_t vCount = polyB2->GetVertexCount();
				for (char i = 0; i < vCount; i++) {
					const b2Vec2& bVx = polyB2->GetVertex(i);
					vertices.push_back({ bVx.x, bVx.y });
				}

				auto poly = make_unique<DebugPolygon>(vertices);
				res.push_back(std::move(poly));
			}
			fixture = fixture->GetNext();
		}
		return res;
	}
}

