/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CPU_CHARACTER_PROXY_UTIL_H
#define HK_CPU_CHARACTER_PROXY_UTIL_H

#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>

class hkpCharacterProxy;
class hkpShapePhantom;
struct hkpLinearCastInput;

/// Contains some useful methods for integrating the hkpCharacterProxy on CPU.
class hkpCpuCharacterProxyUtil
{
	public:
			/// Used when integrating the character single-threaded.
		static inline void linearCastSetPositionAndCast( hkpShapePhantom* phantom
			, const hkpLinearCastInput& input
			, hkpAllCdPointCollector& castCollector
			, hkpAllCdPointCollector* startCollector
			, const hkVector4& position
			);

			/// Used when multithreading the character for the first linear cast.
			/// This is an optimization: we can use the phantom's set of overlapping collidables
			/// to skip the broadphase query required by a world cast.
		static void HK_CALL linearCastPhantomCast( const hkpCharacterProxy* character
			, const hkpLinearCastInput& input
			, hkpAllCdPointCollector& castCollector
			, hkpAllCdPointCollector* startCollector 
			);

			/// Used when multithreading the character for the second and subsequent linear casts.
		static inline void linearCastWorldCast( const hkpShapePhantom* phantom
			, const hkpLinearCastInput& input
			, hkpCollidable* collidable
			, hkpAllCdPointCollector& castCollector
			, hkpAllCdPointCollector* startCollector
			);
};

#include<Physics/Utilities/CharacterControl/CharacterProxy/Multithreaded/Cpu/hkpCpuCharacterProxyUtil.inl>

#endif	//HK_CPU_CHARACTER_PROXY_UTIL_H

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
