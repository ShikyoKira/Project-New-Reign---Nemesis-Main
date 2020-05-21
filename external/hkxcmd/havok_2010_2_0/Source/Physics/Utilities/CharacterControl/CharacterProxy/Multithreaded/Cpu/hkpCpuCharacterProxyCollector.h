/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CPU_CHARACTER_PROXY_COLLECTOR_H
#define HK_CPU_CHARACTER_PROXY_COLLECTOR_H

#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>

/// When multithreading, it wouldn't be thread safe to cast the original collidable around the
/// world, so we use a separate collidable. This collector is used to ignore collisions between
/// the new collidable and the original collidable.
class hkpCpuCharacterProxyCollector : public hkpAllCdPointCollector
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CHARACTER, hkpCpuCharacterProxyCollector );

			// Implements the hkpCdPointCollector interface.
		virtual void addCdPoint( const hkpCdPoint& event );
	
			/// The collector is reused for each command, so its character's collidable is updated
			/// using this method.
		inline void setCharactersCollidable( const hkpCollidable* collidable ) { m_charactersCollidable = collidable; }

	protected:
			/// The original collidable of the character.
		const hkpCollidable* m_charactersCollidable;
};

#endif	//HK_CPU_CHARACTER_PROXY_COLLECTOR_H

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
