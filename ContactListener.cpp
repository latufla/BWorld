#include "stdafx.h"
#include "ContactListener.h"

namespace bl {
	ContactListener::ContactListener() {
	}


	ContactListener::~ContactListener() {
	}

	void ContactListener::BeginContact(b2Contact* contact) {
		if (!contact->IsTouching())
			return;

		b2Body* b1 = contact->GetFixtureA()->GetBody();
		b2Body* b2 = contact->GetFixtureB()->GetBody();
		Contact c{ b1, b2 };
		contacts.push_back(c);
	}

	void ContactListener::EndContact(b2Contact* contact) {
		b2Body* b1 = contact->GetFixtureA()->GetBody();
		b2Body* b2 = contact->GetFixtureB()->GetBody();
		Contact c{ b1, b2 };
		finishedContacts.push_back(c);
	}

	void ContactListener::clearFinishedContacts() {
		for (auto& i : finishedContacts) {
			auto it = find(contacts.cbegin(), contacts.cend(), i);
			if (it != contacts.cend())
				contacts.erase(it);
		}
		finishedContacts.clear();
	}

}