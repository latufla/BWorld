#pragma once
#include <Box2D\Box2D.h>
#include <vector>

namespace bw {
	class ContactListener : public b2ContactListener {
	public:
		ContactListener();
		~ContactListener();

		void BeginContact(b2Contact*);
		void EndContact(b2Contact*);

		// all contacts, even finished, but uncleared
		std::vector<std::pair<b2Body*, b2Body*>>& getContacts() { return contacts; }
		
		// remove all finished contacts
		void clearFinishedContacts();

	private:
		typedef std::pair<b2Body*, b2Body*> Contact;
		std::vector<Contact> contacts; // begun but not ended
		std::vector<Contact> finishedContacts;
	};
}
