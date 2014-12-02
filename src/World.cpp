#include "SharedHeaders.h"
#include "World.h"
#include "Utils.h"
#include "exceptions\Exception.h"

using std::vector;
using std::array;
using std::pair;
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
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		
		idToObject.erase(id);
		objectToId.erase(bodyB2);

		world.DestroyBody(bodyB2);
	}

	void World::attachShape(uint32_t toObj, const Point& center, float radius) {
		b2Body* bodyB2;
		try { 
			bodyB2 = idToObject.at(toObj);
		}
		catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, toObj); 
		}
		
		b2CircleShape shape;
		shape.m_radius = radius;
		shape.m_p.Set(center.x, center.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		bodyB2->CreateFixture(&fixtureDef);
	}

	void World::attachShape(uint32_t toObj, std::vector<Point>& vertices) {
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(toObj);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, toObj);
		}
		
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
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		
		const b2Vec2& posB2 = bodyB2->GetPosition();
		return{ posB2.x, posB2.y };
	}

	float World::getRotation(uint32_t id) {
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}

		return bodyB2->GetAngle();
	}

	array<float, 16> World::getTransform(uint32_t id) {
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}

		array<float, 16> res = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		return Utils::fromBox2d(bodyB2->GetTransform(), res);
	}

	Point World::getGlobalCoM(uint32_t id) {
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		
		const b2Vec2& comB2 = bodyB2->GetWorldCenter();
		Point com = { comB2.x, comB2.y };
		return com;
	}

	Point World::getLocalCoM(uint32_t id) {
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		
		const b2Vec2& comB2 = bodyB2->GetLocalCenter();
		Point com = { comB2.x, comB2.y };
		return com;
	}

	void World::setDensity(uint32_t id, float val) {
		if(val < 0)
			throw InvalidArgumentException(EXCEPTION_INFO, id, "density is non - positive");

		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}

		b2Fixture* fixture = bodyB2->GetFixtureList();
		if(!fixture)
			throw LogicException(EXCEPTION_INFO, id, "no shapes");

		while (fixture) {
			fixture->SetDensity(val);
			fixture = fixture->GetNext();
		}
	}

	void World::setRestitution(uint32_t id, float val) {
		if (val < 0)
			throw InvalidArgumentException(EXCEPTION_INFO, id, "restitution is non-positive");

		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}

		b2Fixture* fixture = bodyB2->GetFixtureList();
		if(!fixture)
			throw LogicException(EXCEPTION_INFO, id, "no shapes");

		while (fixture) {
			fixture->SetRestitution(val);
			fixture = fixture->GetNext();
		}
	}

	void World::setFriction(uint32_t id, float val) {
		if(val < 0)
			throw InvalidArgumentException(EXCEPTION_INFO, id, "friction is non-positive");

		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		
		b2Fixture* fixture = bodyB2->GetFixtureList();
		if(!fixture)
			throw LogicException(EXCEPTION_INFO, id, "no shapes");

		while (fixture) {
			fixture->SetFriction(val);
			fixture = fixture->GetNext();
		}
	}

	void World::setLinearDamping(uint32_t id, float val) {
		if(val < 1)
			throw InvalidArgumentException(EXCEPTION_INFO, id, "damping less than 1");

		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		
		bodyB2->SetLinearDamping((val - 1));
	}

	void World::setSensor(uint32_t id, bool isSensor) {
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		
		b2Fixture* fixture = bodyB2->GetFixtureList();
		if(!fixture)
			throw LogicException(EXCEPTION_INFO, id, "no shapes");

		while (fixture) {
			fixture->SetSensor(isSensor);
			fixture = fixture->GetNext();
		}
	}


	void World::applyLinearImpulse(uint32_t toObj, const Point& val) {
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(toObj);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, toObj);
		}
		
		b2Vec2 impulseB2{val.x, val.y};
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
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(id);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, id);
		}
		
		b2Fixture* fixture = bodyB2->GetFixtureList();
		if(!fixture)
			throw LogicException(EXCEPTION_INFO, id, "no shapes");

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

	// DEBUG
	vector<VertexList> World::getDebugLocalShapes(uint32_t fromObj) {
		b2Body* bodyB2;
		try {
			bodyB2 = idToObject.at(fromObj);
		} catch(std::out_of_range&) {
			throw InvalidObjectIdException(EXCEPTION_INFO, fromObj);
		}
		
		vector<VertexList> res;
		const double delta = 2.0f * M_PI / DEBUG_CIRCLE_SHAPE_STEPS;
		b2Fixture* fixture = bodyB2->GetFixtureList();
		while (fixture) {
			b2Shape* shapeB2 = fixture->GetShape();
			b2Shape::Type shapeType = shapeB2->GetType();
			VertexList vertices;
			switch (shapeType) {
			case b2Shape::e_circle:
				vertices = createDebugCircle((b2CircleShape*)shapeB2, (float)delta);
				break;
			case b2Shape::e_polygon:
				vertices = createDebugPolygon((b2PolygonShape*)shapeB2);
				break;
			default: // do nothing
				;
			}

			res.push_back(vertices);
			fixture = fixture->GetNext();
		}
		return res;
	}

	VertexList World::createDebugCircle(b2CircleShape* shape, float delta) {
		VertexList res;
		const b2Vec2& posB2 = shape->m_p;
		float radiusB2 = shape->m_radius;
		for (uint32_t i = 0; i < DEBUG_CIRCLE_SHAPE_STEPS; ++i) {
			Point v{
				posB2.x + (radiusB2 * cos(i * delta)),
				posB2.y + (radiusB2 * sin(i * delta))
			};
			res.push_back(v);
		}
		return res;
	}

	VertexList World::createDebugPolygon(b2PolygonShape* shape) {
		VertexList res;
		uint32_t vCount = shape->GetVertexCount();
		for (uint32_t i = 0; i < vCount; ++i) {
			const b2Vec2& vB2 = shape->GetVertex(i);
			Point v{ vB2.x, vB2.y };
			res.push_back(v);
		}
		return res;
	}
	// END DEBUG
}

