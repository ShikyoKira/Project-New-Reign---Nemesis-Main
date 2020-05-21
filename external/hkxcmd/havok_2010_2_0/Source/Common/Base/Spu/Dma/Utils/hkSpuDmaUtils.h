/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SPU_DMA_UTILS_H
#define HK_SPU_DMA_UTILS_H


#include <Common/Base/hkBase.h>

enum
{
	HK_SPU_DMA_GROUP_STALL	= 14,
	HK_SPU_DMA_GROUP_IDLE	= 15, // DO NOT use this dma group for any get/put operations! use it only for 'idle' waiting.
};

	/// this utility allows simple access to variables in main memory and can be used either from the SPU, PPU or CPU
struct hkSpuDmaUtils
{
		/// Convenient and slow function to increment a value, returns the new value
	static hkInt32 HK_CALL incrementInt32InMainMemory( HK_CPU_PTR(hkInt32*) variable, int increment, int dmaGroupId = HK_SPU_DMA_GROUP_STALL );

		/// Convenient and slow function to set a variable
	static void HK_CALL setInt32InMainMemory( HK_CPU_PTR(hkInt32*) variable, hkInt32 value, int dmaGroupId = HK_SPU_DMA_GROUP_STALL );

		/// Convenient and slow function to get a variable
	static hkInt32 HK_CALL getInt32FromMainMemory( HK_CPU_PTR(hkInt32*) variable, int dmaGroupId = HK_SPU_DMA_GROUP_STALL );

		/// Convenient way to get a single pointer
	static void* HK_CALL getPntrFromMainMemory( HK_CPU_PTR(void**) variable, int dmaGroupId = HK_SPU_DMA_GROUP_STALL );

		/// put a float to main memory & wait for completion
	static void HK_CALL setFloat32InMainMemory( HK_CPU_PTR(float*) dstInMainMemory, hkReal f, int dmaGroupId = HK_SPU_DMA_GROUP_STALL );

		/// put a half float to main memory & wait for completion
	static void HK_CALL setFloat16InMainMemory( HK_CPU_PTR(hkHalf*) dstInMainMemory, hkReal f, int dmaGroupId = HK_SPU_DMA_GROUP_STALL );

		/// put a char to main memory & wait for completion
	static void HK_CALL setChar8InMainMemory( HK_CPU_PTR(hkChar*) dstInMainMemory, hkChar c, int dmaGroupId = HK_SPU_DMA_GROUP_STALL );

		/// copy an array to main memory, without a restriction on the size being a multiple of 16 bytes
		/// Note that this is no waitForCompletion inside this function; you must do the wait yourself.
	static void HK_CALL putToMainMemory32( HK_CPU_PTR(hkUint32*) dstInMainMemory, const hkUint32* srcOnSpu, int numWords, int dmaGroupId = HK_SPU_DMA_GROUP_STALL );
};


#	include <Common/Base/Spu/Dma/Utils/hkDefaultSpuDmaUtils.inl>


#endif // HK_SPU_DMA_UTILS_H

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
