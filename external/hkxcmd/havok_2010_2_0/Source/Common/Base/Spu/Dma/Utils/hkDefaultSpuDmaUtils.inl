/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/hkBase.h>
#include <Common/Base/Spu/Dma/Utils/hkSpuDmaUtils.h>


HK_FORCE_INLINE hkInt32 hkSpuDmaUtils::incrementInt32InMainMemory( HK_CPU_PTR(hkInt32*) variable, int increment, int dmaGroupId )
{
	hkInt32 newValue = *variable + increment;
	*variable = newValue;
	return newValue;
}


HK_FORCE_INLINE void hkSpuDmaUtils::setInt32InMainMemory( HK_CPU_PTR(hkInt32*) variable, hkInt32 value, int dmaGroupId )
{
	*variable = value;
}


HK_FORCE_INLINE void* hkSpuDmaUtils::getPntrFromMainMemory( HK_CPU_PTR(void**) variable, int dmaGroupId  )
{
	return *variable;
}


HK_FORCE_INLINE void hkSpuDmaUtils::setFloat32InMainMemory( HK_CPU_PTR(float*) dstInMainMemory, hkReal value, int dmaGroupId  )
{
	*dstInMainMemory = value;
}



HK_FORCE_INLINE void hkSpuDmaUtils::setChar8InMainMemory( HK_CPU_PTR(hkChar*) dstInMainMemory, hkChar value, int dmaGroupId  )
{
	*dstInMainMemory = value;
}

HK_FORCE_INLINE void hkSpuDmaUtils::putToMainMemory32( HK_CPU_PTR(hkUint32*) dstInMainMemory, const hkUint32* srcOnSpu, int numWords, int dmaGroupId )
{
	hkString::memCpy4(dstInMainMemory, srcOnSpu, numWords);
}

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
