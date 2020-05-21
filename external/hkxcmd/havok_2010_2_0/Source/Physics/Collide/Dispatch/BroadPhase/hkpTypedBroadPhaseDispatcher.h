/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TYPED_BROAD_PHASE_HANDLER
#define HK_TYPED_BROAD_PHASE_HANDLER

#include <Physics/Collide/Filter/hkpCollidableCollidableFilter.h>
#include <Physics/Collide/Dispatch/BroadPhase/hkpNullBroadPhaseListener.h>

class hkpTypedBroadPhaseHandlePair;
class hkpBroadPhaseHandlePair;
class hkpBroadPhaseListener;

#define HK_MAX_BROADPHASE_TYPE 8

class hkpTypedBroadPhaseDispatcher
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkpTypedBroadPhaseDispatcher);

		hkpTypedBroadPhaseDispatcher();
		
		~hkpTypedBroadPhaseDispatcher();

		/// sorts and removes duplicates
		static void HK_CALL removeDuplicates( hkArray<hkpBroadPhaseHandlePair>& newPairs, hkArray<hkpBroadPhaseHandlePair>& delPairs );

		///// swap each pair's internal data so A > B, then sort
		//static void HK_CALL sortPairs( hkpTypedBroadPhaseHandlePair* pairs, int numPairs );

			/// update the dispatcher by merging
		void addPairs( hkpTypedBroadPhaseHandlePair* newPairs, int numNewPairs, const hkpCollidableCollidableFilter* filter ) const;
		
		void removePairs( hkpTypedBroadPhaseHandlePair* deletedPairs, int numDeletedPairs ) const;


		inline hkpBroadPhaseListener* getBroadPhaseListener( int typeA, int typeB );

		inline void setBroadPhaseListener( hkpBroadPhaseListener* listener, int typeA, int typeB );

		inline hkpBroadPhaseListener* getNullBroadPhaseListener();

	protected:
		hkpBroadPhaseListener*    m_broadPhaseListeners[HK_MAX_BROADPHASE_TYPE][HK_MAX_BROADPHASE_TYPE];
		hkpNullBroadPhaseListener m_nullBroadPhaseListener;
};

#include <Physics/Collide/Dispatch/BroadPhase/hkpTypedBroadPhaseDispatcher.inl>

#endif // HK_TYPED_BROAD_PHASE_HANDLER

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
