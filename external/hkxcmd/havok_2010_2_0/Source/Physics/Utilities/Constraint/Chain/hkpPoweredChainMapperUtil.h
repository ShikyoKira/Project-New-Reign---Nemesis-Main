/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES_POWERED_CHAIN_MAPPER_UTIL_H
#define HK_UTILITIES_POWERED_CHAIN_MAPPER_UTIL_H

#include <Common/Base/hkBase.h>


class hkpPoweredChainMapperUtil
{
public:
		/// Adds all chains and limit constraints to the world, asserts if the entities are not there yet.
	static void HK_CALL addToWorld(class hkpWorld* world, class hkpPoweredChainMapper* mapper);

		/// removes all constraints from the world, if they're still in it.
	static void HK_CALL removeFromWorld(class hkpWorld* world, class hkpPoweredChainMapper* mapper);

};


#endif // HK_UTILITIES_POWERED_CHAIN_MAPPER_UTIL_H

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
