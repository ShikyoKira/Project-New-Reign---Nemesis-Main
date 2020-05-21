/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GEOMETRY_UTILITIES_SPU_CONFIG_H
#define HK_GEOMETRY_UTILITIES_SPU_CONFIG_H

// Skinning
#define SPU_SUPPORT_SKIN_PNTB		// Whether to support skinning of tangents and bitangents.
//#define SPU_SUPPORT_SKIN_PNT		// Whether to support skinning of tangents only.
#define SPU_SUPPORT_SKIN_PARTIAL	// Whether partial skinning (only some vertices modified) is supported

#define SPU_BATCH_SIZE_SKIN_P		512
#define SPU_BATCH_SIZE_SKIN_PN		256
#define SPU_BATCH_SIZE_SKIN_PNT		256
#define SPU_BATCH_SIZE_SKIN_PNTB	128

#endif	//	HK_GEOMETRY_UTILITIES_SPU_CONFIG_H

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
