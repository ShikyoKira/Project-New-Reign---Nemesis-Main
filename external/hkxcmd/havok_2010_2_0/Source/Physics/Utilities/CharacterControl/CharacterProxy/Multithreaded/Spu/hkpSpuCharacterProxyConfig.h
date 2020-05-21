/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTER_PROXY_SPU_CONFIG_H
#define HK_CHARACTER_PROXY_SPU_CONFIG_H


#include <Common/Base/Spu/Config/hkSpuConfig.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>

// ===============================================================================================================================================================================================
//  GENERAL
// ===============================================================================================================================================================================================

// The total amount of SPU memory dedicated to the hkStackMemory in the character proxy .elf
#define HK_SPU_TOTAL_CHARACTER_PROXY_BUFFER_SIZE 57*1024


// This is the size of the global UNTYPED cache. Must be a power of 2, should be reasonably sized.
#define HK_SPU_CHARACTER_PROXY_UNTYPED_CACHE_SIZE 16384

// This is the size of the global MOPP cache. Must be a power of 2, should be reasonably sized.
// Note: this memory block will also be used for bringing in the broadphase for all jobs that use the broadphase on SPU, so it shouldn't be sized too small!
#define HK_SPU_CHARACTER_PROXY_MOPP_CACHE_SIZE 16384

// automatically set/calculated values; DO NOT CHANGE
enum { HK_SPU_CHARACTER_PROXY_UNTYPED_CACHE_NUM_ROWS	= HK_SPU_CHARACTER_PROXY_UNTYPED_CACHE_SIZE	/ (4 * HK_SPU_UNTYPED_CACHE_LINE_SIZE )	};
enum { HK_SPU_CHARACTER_PROXY_MOPP_CACHE_NUM_ROWS		= HK_SPU_CHARACTER_PROXY_MOPP_CACHE_SIZE	/ (4 * HK_MOPP_CHUNK_SIZE )	};

// This is the size of the linear cast collector buffer
#define FIXED_BUFFER_CD_POINT_COLLECTOR_CAPACTITY_ON_SPU 20

// This capacity limits the manifold, bodies and phantoms hkArraySpu arrays as we can't resize on SPU
#define HK_MANIFOLD_CAPACITY_ON_SPU 10




#endif // HK_CHARACTER_PROXY_SPU_CONFIG_H

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
