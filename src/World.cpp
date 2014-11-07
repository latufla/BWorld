#include "stdafx.h"
#include "World.h"

#include "Utils.h"
#include <iostream>

using std::vector;
using std::array;
using std::pair;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;
using std::string;
using std::to_string;

namespace bw{
	World::World()
		: world({ 0.0f, 0.0f }) {
		world.SetContactListener(&contactListener);
	}

	void World::doStep(float stepSec) {
		world.Step(stepSec, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

		doContactsStep();
	}

	void World::doContactsStep() {
		contacts.clear();
		contactListener.clearFinishedContacts();

		auto& contactsB2 = contactListener.getContacts();
		for (auto& i : contactsB2) {
			uint32_t id1 = objectToId.at(i.first);
			uint32_t id2 = objectToId.at(i.second);
			pair<uint32_t, uint32_t> c{ id1, id2 };
			contacts.push_back(c);
		}
	}

	bool World::addObject(uint32_t id, ObjectType type, const Point& position, float rotationYRad) {
		b2BodyDef bodyB2Def;
		bodyB2Def.type = static_cast<b2BodyType>(type);
		bodyB2Def.position.Set(position.x, position.y);
		bodyB2Def.angle = rotationYRad;
		b2Body* bodyB2 = world.CreateBody(&bodyB2Def);
		if (!bodyB2)
			return false;
		
		auto iToO = idToObject.emplace(id, bodyB2);
		auto oToI = objectToId.emplace(bodyB2, id);

		return iToO.second && oToI.second;
	}

	void World::removeObject(uint32_t id) {
		b2Body* bodyB2 = idToObject.at(id);
		idToObject.erase(id);
		objectToId.erase(bodyB2);

		world.DestroyBody(bodyB2);
	}

	void World::attachShape(uint32_t toObj, const Point& center, float radius) {
		b2Body* bodyB2 = idToObject.at(toObj);

		b2CircleShape shape;
		shape.m_radius = radius;
		shape.m_p.Set(center.x, center.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		bodyB2->CreateFixture(&fixtureDef);
	}

	void World::attachShape(uint32_t toObj, std::vector<Point>& vertices) {
		b2Body* bodyB2 = idToObject.at(toObj);
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
		bodyB2->CreateFixture(&fixtureDef);
	}

	Point World::getPosition(uint32_t id) {
		b2Body* bodyB2 = idToObject.at(id);
		const b2Vec2& posB2 = bodyB2->GetPosition();
		return{ posB2.x, posB2.y };
	}
	
	float World::getRotation(uint32_t id) {
		b2Body* bodyB2 = idToObject.at(id);
		return bodyB2->GetAngle();
	}

	array<float, 16> World::getTransform(uint32_t id) {
		b2Body* bodyB2 = idToObject.at(id);
		array<float, 16> res = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		return Utils::fromBox2d(bodyB2->GetTransform(), res);
	}

	Point World::getGlobalCoM(uint32_t id) {
		b2Body* bodyB2 = idToObject.at(id);
		const b2Vec2& centerB2 = bodyB2->GetWorldCenter();
		return {centerB2.x, centerB2.y};
	}

	Point World::getLocalCoM(uint32_t id) {
		b2Body* bodyB2 = idToObject.at(id);
		const b2Vec2& centerB2 = bodyB2->GetLocalCenter();
		return{ centerB2.x, centerB2.y };
	}

	void World::setDensity(uint32_t id, float val) {
		if (val < 0)
			throw std::exception("World::setDensity nonpositive density");
		
		b2Body* bodyB2 = idToObject.at(id);
		b2Fixture* fixture = bodyB2->GetFixtureList();
		if (!fixture)
			throw std::exception("World::setDensity no shapes");
		
		while (fixture) {
			fixture->SetDensity(val);
			fixture = fixture->GetNext();
		}
	}

	void World::setRestitution(uint32_t id, float val) {
		if (val < 0)
			throw std::exception("World::setRestitution nonpositive restitution");

		b2Body* bodyB2 = idToObject.at(id);
		b2Fixture* fixture = bodyB2->GetFixtureList();
		if (!fixture)
			throw std::exception("World::setRestitution no shapes");

		while (fixture) {
			fixture->SetRestitution(val);
			fixture = fixture->GetNext();
		}
	}

	void World::setFriction(uint32_t id, float val) {
		if (val < 0)
			throw std::exception("World::setFriction nonpositive friction"); 
		
		b2Body* bodyB2 = idToObject.at(id);
		b2Fixture* fixture = bodyB2->GetFixtureList();
		if (!fixture)
			throw std::exception("World::setFriction no shapes"); 
		
		while (fixture) {
			fixture->SetFriction(val);
			fixture = fixture->GetNext();
		}
	}

	void World::setLinearDamping(uint32_t id, float val) {
		if (val < 1)
			throw std::exception("World::setLinearDamping damping less 1");
		
		b2Body* bodyB2 = idToObject.at(id);
		bodyB2->SetLinearDamping((val - 1));
	}

	void World::setSensor(uint32_t id, bool isSensor) {
		b2Body* bodyB2 = idToObject.at(id);
		b2Fixture* fixture = bodyB2->GetFixtureList();
		if (!fixture)
			throw std::exception("World::setSensor no shapes");

		while (fixture) {
			fixture->SetSensor(isSensor);
			fixture = fixture->GetNext();
		}
	}

	
	void World::applyLinearImpulse(uint32_t toObj, const Point& impulse) {
		b2Body* bodyB2 = idToObject.at(toObj);
		b2Vec2 impulseB2{ impulse.x, impulse.y };
		bodyB2->ApplyLinearImpulse(impulseB2, bodyB2->GetWorldCenter(), true);
	}

	
	vector<uint32_t> World::checkContacts(uint32_t id) {
		vector<uint32_t> res;
		for (auto& i : contacts) {
			if (i.first == id)
				res.push_back(i.second);
			else if (i.second == id)
				res.push_back(i.first);
		}
		return res;
	}

	bool World::hasContact(uint32_t id, const Point& withPoint) {
		b2Body* bodyB2 = idToObject.at(id);
		b2Fixture* fixture = bodyB2->GetFixtureList();
		if (!fixture)
			throw std::exception("World::hasContact no shapes");

		b2Vec2 pointB2{ withPoint.x, withPoint.y };
		const b2Transform& transform = bodyB2->GetTransform();
		while (fixture) {
			b2Shape* shape = fixture->GetShape();
			if (shape->TestPoint(transform, pointB2))
				return true;

			fixture = fixture->GetNext();
		}
		return false;
	}


	string World::objectToString(uint32_t id) {
		auto lCoM = getLocalCoM(id);
		auto gCoM = getGlobalCoM(id);
		auto transform = getTransform(id);

		b2Body* bodyB2 = idToObject.at(id);
		auto mass = bodyB2->GetMass();
		auto velocity = bodyB2->GetLinearVelocity();

 		string res = 
			"{lCoM: " + static_cast<string>(lCoM)
			+"\ngCoM: " + static_cast<string>(gCoM)
			+"\ntransform:\n" + Utils::toString(getTransform(id))
			+ "\nmass: " + to_string(mass)
 			+ "\nvelocity: { x: " + to_string(velocity.x) + " y: " + to_string(velocity.y) + "}"
 			+ "\ncontacts: {";
		for (auto& i : checkContacts(id)) {
			res += " " + to_string(i);
		}

		res += "}}";
 		return res;
	}

	DebugShapeList World::getDebugShapes(uint32_t fromObj) {
		b2Body* bodyB2 = idToObject.at(fromObj);

		DebugShapeList res;
		b2Fixture* fixture = bodyB2->GetFixtureList();
		while (fixture) {
			b2Shape* shapeB2 = fixture->GetShape();
			b2Shape::Type shapeType = shapeB2->GetType();
			if (shapeType == b2Shape::e_circle) {
				b2CircleShape* circleB2 = (b2CircleShape*)shapeB2;
				b2Vec2& pos = circleB2->m_p;

				auto circle = make_unique<DebugCircle>(Point{ pos.x, pos.y }, circleB2->m_radius);
				res.push_back(std::move(circle));
			}
			else if (shapeType == b2Shape::e_polygon) {
				b2PolygonShape* polyB2 = (b2PolygonShape*)shapeB2;
				vector<Point> vertices;
				uint32_t vCount = polyB2->GetVertexCount();
				for (uint32_t i = 0; i < vCount; i++) {
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

