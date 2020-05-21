/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_LAZY_ADD_TO_WORLD_H
#define HK_UTILITIES2_LAZY_ADD_TO_WORLD_H

#include <Common/Base/hkBase.h>

class hkpWorld;
class hkpEntity;
class hkpAction;
class hkpConstraintInstance;

	/// A class that may prove useful if using a complex toolchain that does not
	/// guarantee complete construction of Havok objects e.g., hkpRigidBody with an
	/// hkpShape left as HK_NULL. It maintains hkArrays of pointers to objects that
	/// can be added to a hkpWorld using commit functions. It Asserts if any incomplete
	/// objects are committed.
	/// NB: This class is only intended for use at setup time, before simulation
	/// begins.
class hkpLazyAddToWorld : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			//
			// Public functions.
			//

			/// Constructor takes a pointer to an hkpWorld that objects will (eventually)
			/// be added to.
		hkpLazyAddToWorld(hkpWorld* world);

			/// Destructor Asserts if hkArrays are not all empty i.e., all objects committed
			/// to m_world.
		~hkpLazyAddToWorld();


			//
			// Functions to commit objects to m_world.
			//
		
			/// Add all the elements of all the hkArrays to m_world. Returns the
			/// number of objects committed or -1 for failure.
		int commitAll();

			/// Commit all remaining hkEntities. Return the number of hkEntities
			/// committed or -1 for failure.
		int commitAllEntity();

			/// Commit all remaining hkActions. Return the number of hkActions
			/// committed or -1 for failure.
		int commitAllAction();

			/// Commit all remaining hkConstraints. Return the number of hkConstraints
			/// committed or -1 for failure.
		int commitAllConstraint();

			//
			// Functions to add pointers to the commit hkArrays.
			//

			/// Add an hkpEntity to the hkArray of hkEntites to be committed. Return
			/// the size of m_entities after adding or -1 for failure.
		int addEntity(hkpEntity* entity);

			/// Add an hkpAction to the hkArray of hkActions to be committed. Return
			/// the size of m_actions after adding or -1 for failure.
		int addAction(hkpAction* action);

			/// Add an hkpConstraintInstance to the hkArray of hkConstraints to be committed. Return
			/// the size of m_constraints after adding or -1 for failure.
		int addConstraint( hkpConstraintInstance* constraint );

			//
			// Functions to ensure objects are fully constructed (before committing).
			//

			/// Return true if the hkpEntity is fully constructed.
		hkBool isValid(hkpEntity* entity);

			/// Return true if the hkpAction is fully constructed.
		hkBool isValid(hkpAction* action);

			/// Return true if the hkpConstraintInstance is fully constructed.
		hkBool isValid(hkpConstraintInstance* constraint);

	public:
			//
			// Public data.
			//

			/// The hkpWorld to which objects will be committed.
		hkpWorld* m_world;

	protected:	
			//
			// Private data.
			//

			/// hkArray holding pointers to hkEntities waiting to be committed.
		hkArray<hkpEntity*> m_entities;

			/// hkArray holding pointers to hkActions waiting to be committed.
		hkArray<hkpAction*> m_actions;

			/// hkArray holding pointers to hkConstraints waiting to be committed.
		hkArray<hkpConstraintInstance*> m_constraints;
};

#endif // HK_UTILITIES2_LAZY_ADD_TO_WORLD_H

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
