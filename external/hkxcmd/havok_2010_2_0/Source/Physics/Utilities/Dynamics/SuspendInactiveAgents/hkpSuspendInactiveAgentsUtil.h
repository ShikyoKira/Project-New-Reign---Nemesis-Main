/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_SUSPEND_INACTIVE_AGENTS_H
#define HK_UTILITIES2_SUSPEND_INACTIVE_AGENTS_H

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/World/Listener/hkpIslandActivationListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldDeletionListener.h>


	/// A class which tries to free collision cache memory for inactive islands
	/// Note: this class works because of the current implementation of hkpBvTreeStreamAgent::updateShapeCollectionFilter
class hkpSuspendInactiveAgentsUtil : public hkReferencedObject, hkpIslandActivationListener, hkpWorldDeletionListener
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		enum OperationMode
		{
				/// Remove all collision agents which are a subagent of the bvtreestream agent.
				/// Typically this are the agents between moving bodies and the triangles of the
				/// landscape. Note: This type of agents account for most collision agent memory in most games.
				/// Using this mode you will see small artifacts when objects wake up.
			SUSPEND_1N_AGENT_TRACKS,

				/// Removes all child agents for shape collections. This performs another explicit call to updateCollisionFilter on collections
				/// when the island is activated.
				/// Note: this call is not safe if that update call would remove agents, or in multithreaded simulation.
			SUSPEND_ALL_COLLECTION_AGENTS,

				/// Removes all caches between convex shapes and convex shapes in a shape collection if the cache
				/// holds no contact point data.
				/// This mode is completely artifact free, you only get a very tiny CPU spike when these agents wake up.
			SUSPEND_UNUSED_CACHES
		};

			/// This determines how contacts are found when an island activates
		enum InitContactsMode
		{
				/// This is the default. When an island activates, it does extra work to try to find more contact points between colliding
				/// pairs whose agents were deleted.
				/// This flag is ignored if OperationMode==SUSPEND_UNUSED_CACHES
			INIT_CONTACTS_FIND,

				/// This setting disables the extra work on activation. It is faster, but may result in some extra initial jitter as objects
				/// activate.
			INIT_CONTACTS_DO_NOT_FIND
		};

			//
			// Public functions.
			//

			/// Constructor takes a pointer to an hkpWorld and a flag indicating
			/// how aggressively you want to remove agents.
			/// It automatically adds itself to the world and increases its own reference count.
			/// When the world is deleted, it decreases its own reference count.
		hkpSuspendInactiveAgentsUtil(hkpWorld* world, OperationMode mode = SUSPEND_1N_AGENT_TRACKS, InitContactsMode initContactsMode = INIT_CONTACTS_FIND );

		~hkpSuspendInactiveAgentsUtil();

	public:
			/// Called when an island is activated. Simply does nothing.
			/// ###ACCESS_CHECKS###( [island->getWorld(),HK_ACCESS_RW] );
		virtual void islandActivatedCallback( hkpSimulationIsland* island );

			/// Called when an island is deactivated
			/// ###ACCESS_CHECKS###( [island->getWorld(),HK_ACCESS_RW] );
		virtual void islandDeactivatedCallback( hkpSimulationIsland* island );

			/// Called when the hkpWorld is deleted.
		virtual void worldDeletedCallback( hkpWorld* world);

	public:
		hkpWorld* m_world;
		OperationMode m_mode;
		InitContactsMode m_initContactsMode;
};

#endif // HK_UTILITIES2_SUSPEND_INACTIVE_AGENTS_H

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
