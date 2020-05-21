/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_PHYSICS_SYSTEM_WITH_CONTACTS_H
#define HK_DYNAMICS2_PHYSICS_SYSTEM_WITH_CONTACTS_H

#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

extern const class hkClass hkpPhysicsSystemWithContactsClass;

struct hkpSerializedAgentNnEntry;

/// A class to group collections of objects, which typically are serialized together,
/// and should be added and removed from the world together.
/// Examples are a ragdoll, and a vehicle.
class hkpPhysicsSystemWithContacts : public hkpPhysicsSystem
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_WORLD);
		HK_DECLARE_REFLECTION();

		hkpPhysicsSystemWithContacts() {}

		virtual ~hkpPhysicsSystemWithContacts();

			/// Return a copy of the system, in which all items
			/// are clones of the originals, sharing as much data
			/// as possible (e.g.: shapes, constraintData, etc)
		virtual hkpPhysicsSystem* clone() const;
		
			/// Shallow copy a system. Just copies arrays of ptrs.
		void copy(const hkpPhysicsSystemWithContacts& toCopy);

			/// Add an action to the list
		void addContact( hkpSerializedAgentNnEntry* c );

			/// Remove a contact from the list
			/// It does a quick remove, so will not preserve
			/// the order of the list
		void removeContact( int i );

			/// Get the held contact information
		inline const hkArray<hkpSerializedAgentNnEntry*>& getContacts() const;

			/// Gets non-const version of contact points information.
			/// It is recommended to use add/removeContact() instead, as they handle reference counting for the serialized entries.
		inline hkArray<hkpSerializedAgentNnEntry*>& getContactsRw();

		virtual hkBool hasContacts() { return true; }

	protected:

		/// Note, ALL these arrays contain reference counted instances.
		/// The destructor for the system will decrement the references on all held objects
		/// so it assumes that when you fill out these arrays that you give it a referenced
		/// count incremented object
		hkArray<struct hkpSerializedAgentNnEntry*> m_contacts;

	public:

		hkpPhysicsSystemWithContacts( hkFinishLoadedObjectFlag f ) : hkpPhysicsSystem(f), m_contacts(f) { } 
};

#include <Physics/Utilities/Dynamics/SaveContactPoints/hkpPhysicsSystemWithContacts.inl>

#endif	// HK_DYNAMICS2_PHYSICS_SYSTEM_WITH_CONTACTS_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
