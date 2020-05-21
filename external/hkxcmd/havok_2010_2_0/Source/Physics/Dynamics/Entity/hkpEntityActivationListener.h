/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_ENTITY_ACTIVATION_LISTENER_H
#define HK_DYNAMICS2_ENTITY_ACTIVATION_LISTENER_H

class hkpEntity;

/// Inherit from this class to receive activation and deactivation events from a given entity
/// You must add your listener class to the entity with hkpEntity::addEntityActivationListener() to receive these events
class hkpEntityActivationListener
{
	//+hk.MemoryTracker(ignore=True)
	public:
		virtual ~hkpEntityActivationListener() {}

			/// Called when an entity is deactivated.
			/// NOTE: Activation and Deactivation callbacks are not symmetrical,
			/// and neither is called directly upon entity addition to or removal
			/// from the hkpWorld. Similarly activation callbacks are not fired
			/// when entities are moved between fixed and dynamic islands.
		virtual void entityDeactivatedCallback(hkpEntity* entity) = 0;

			/// Called when an entity is activated.
			/// NOTE: Activation and Deactivation callbacks are not symmetrical,
			/// and neither is called directly upon entity addition to or removal
			/// from the hkpWorld. Similarly activation callbacks are not fired
			/// when entities are moved between fixed and dynamic islands.
		virtual void entityActivatedCallback(hkpEntity* entity) = 0;

};


#endif // HK_DYNAMICS2_ENTITY_ACTIVATION_LISTENER_H

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
