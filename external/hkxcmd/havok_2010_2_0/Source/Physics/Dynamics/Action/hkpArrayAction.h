/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_ARRAY_ACTION_H
#define HK_DYNAMICS2_ARRAY_ACTION_H

#include <Physics/Dynamics/Action/hkpAction.h>

extern const hkClass hkpArrayActionClass;

/// You can use this as a base class for hkActions that operate on a large number of entities.
/// In addition to the hkpAction interface, this class provides some useful basic functionality for your
/// actions, such as an array to store the entity references and a callback that removes the action from the simulation
/// if one of the entities is removed.
class hkpArrayAction : public hkpAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();
	
			/// Creates a new hkpArrayAction that operates on the specified entities.
		hkpArrayAction( const hkArray<hkpEntity*>& entities, hkUlong userData = 0);

			/// Constructor, which allows to add entities later on.
			/// Note: make sure you add the rigid bodies to the action before you add the
			/// action to the world
		hkpArrayAction() : hkpAction(0) { }
		
			/// Adds an entity to the action.
		void addEntity(hkpEntity* entity);

			/// Removes an entity from the action.
			/// Note: If the action is added to the world, then it must contain at least one
			/// entity. Removing the last one will raise an assert.
		void removeEntity(hkpEntity* entity);
	
			/// Gets the action's entities.
		virtual void getEntities( hkArray<hkpEntity*>& entitiesOut );

			/// Removes the action from the hkpWorld if any of its entities are removed from the simulation.
		virtual void entityRemovedCallback(hkpEntity* entity);
		
			/// The applyAction() method does the actual work of the action, and is called at every simulation step.
		virtual void applyAction( const hkStepInfo& stepInfo ) = 0;

	protected:

		~hkpArrayAction();

		void _referenceBodies(); 

		// The action's entities.
		hkArray<hkpEntity*> m_entities;

	public:

		hkpArrayAction( class hkFinishLoadedObjectFlag flag ) : hkpAction(flag), m_entities(flag) {}
};


#endif // HK_DYNAMICS2_ARRAY_ACTION_H

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
