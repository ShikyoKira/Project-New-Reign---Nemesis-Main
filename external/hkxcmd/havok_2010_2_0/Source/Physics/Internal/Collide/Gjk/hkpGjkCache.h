/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_GJK_CACHE_H
#define HK_COLLIDE2_GJK_CACHE_H

#include <Common/Internal/Collide/Gsk/hkCdVertex.h>

//: A structure to hold GJK caching information between calls.
class hkpGjkCache
{
	public:
	
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkpGjkCache);

		inline hkpGjkCache();

		inline void reset ();
		inline void swap (hkVector4* M, int i, int j);
		
		hkpCdVertex Ra[4];				//: Vertices on simplex A.
		hkpCdVertex Rb[4];				//: Vertices on simplex B.
		
		int		rSize;						//: Size of simplices.

};



#include <Physics/Internal/Collide/Gjk/hkpGjkCache.inl>


#endif // HK_COLLIDE2_GJK_CACHE_H

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
