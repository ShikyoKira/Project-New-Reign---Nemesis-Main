/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_PHYSICS_SYSTEM_H
#define HK_DYNAMICS2_PHYSICS_SYSTEM_H

extern const hkClass hkpPhysicsSystemClass;

class hkpRigidBody;
class hkpConstraintData;
class hkpConstraintInstance;
class hkpAction;
class hkpPhantom;

/// A class to group collections of objects, which typically are serialized together,
/// and should be added and removed from the world together.
/// Examples are a ragdoll, and a vehicle.
class hkpPhysicsSystem : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_WORLD);
		HK_DECLARE_REFLECTION();

		hkpPhysicsSystem();

		virtual ~hkpPhysicsSystem();

			/// Return a copy of the system, in which all items
			/// are clones of the originals, sharing as much data
			/// as possible (e.g.: shapes, constraintData, etc)
		virtual hkpPhysicsSystem* clone() const;

			/// removes all elements
		void removeAll();
		
			/// Shallow copy a system. Just copies arrays of ptrs.
		void copy(const hkpPhysicsSystem& toCopy);

			/// Add a rigidbody to the list
		void addRigidBody( hkpRigidBody* rb );

			/// Add a phantom to the list
		void addPhantom(  hkpPhantom* p );

			/// Add a constraint to the list
		void addConstraint( hkpConstraintInstance* c );

			/// Add an action to the list
		void addAction( hkpAction* a );

			/// Remove a rigidbody from the list
			/// It does a quick remove, so will not preserve
			/// the order of the list
		void removeRigidBody( int i );

			/// Remove a phantom from the list
			/// It does a quick remove, so will not preserve
			/// the order of the list
		void removePhantom( int i );

			/// Remove an constraint from the list
			/// It does a quick remove, so will not preserve
			/// the order of the list
		void removeConstraint( int i );

			/// Remove an action from the list
			/// It does a quick remove, so will not preserve
			/// the order of the list
		void removeAction( int i );

			/// Removes all null pointers from the phantoms array.
			/// This is handy when loading a snapshot where hkpPhantom-derived classes have not been serialized.
		void removeNullPhantoms();

			/// Get the held array of bodies
		inline const hkArray<hkpRigidBody*>& getRigidBodies() const;

			/// Get the held array of phantoms
		inline const hkArray<hkpPhantom*>& getPhantoms() const;

			/// Get the held array of constraints
		inline const hkArray<hkpConstraintInstance*>& getConstraints() const;

			/// Get the held array of actions
		inline const hkArray<hkpAction*>& getActions() const;

			/// Get the name of the system
		inline const char* getName() const;

			/// Set the name of the system
		inline void setName(const char* name);

			/// Get the user data for the system
		inline hkUlong getUserData() const;

			/// Set the user data for the system
		inline void setUserData(hkUlong d);

			/// 
		inline hkBool isActive() const;

			/// 
		inline void setActive(hkBool active);

		virtual hkBool hasContacts() { return false; }


			/// This function transforms the physics system.
		void transform(const hkTransform& transformation);

	protected:

		/// Note, ALL these arrays contain reference counted instances.
		/// The destructor for the system will decrement the references on all held objects
		/// so it assumes that when you fill out these arrays that you give it a referenced
		/// count incremented object
		hkArray<class hkpRigidBody*> m_rigidBodies;

		hkArray<class hkpConstraintInstance*> m_constraints;

		hkArray<class hkpAction*> m_actions;

		hkArray<class hkpPhantom*> m_phantoms;

		hkStringPtr m_name;

		hkUlong	m_userData; //+default(0)

		hkBool m_active; //+default(true)

	public:

		hkpPhysicsSystem( hkFinishLoadedObjectFlag f ) :
		  hkReferencedObject(f), m_rigidBodies(f), m_constraints(f), m_actions(f), m_phantoms(f), m_name(f) { }
};

#include <Physics/Dynamics/World/hkpPhysicsSystem.inl>

#endif	// HK_DYNAMICS2_PHYSICS_SYSTEM_H

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
