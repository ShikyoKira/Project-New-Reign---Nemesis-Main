/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DYNAMICS2_ISLAND_ACTIVATION_LISTENER_H
#define HK_DYNAMICS2_ISLAND_ACTIVATION_LISTENER_H

class hkpSimulationIsland;

	/// Derive from this class, and add to hkpWorld using hkpWorld::addIslandActivationListener()
	/// You will then receive callbacks when simulation islands are activated and deactivated
class hkpIslandActivationListener
{
	//+hk.MemoryTracker(ignore=True)
	public:

		virtual ~hkpIslandActivationListener() {}

			/// Called when an island is activated.
			/// NOTE: Activation and Deactivation callbacks are not symmetrical,
			/// and neither is called upon island creation and destruction.
		virtual void islandActivatedCallback( hkpSimulationIsland* island ) = 0;

			/// Called when an island is deactivated.
			/// NOTE: Activation and Deactivation callbacks are not symmetrical,
			/// and neither is called upon island creation and destruction.
		virtual void islandDeactivatedCallback( hkpSimulationIsland* island ) = 0;
};


#endif // HK_DYNAMICS2_WORLD_ACTIVATION_LISTENER_H

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
