/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This header creates the enum used to define the memory classes
// and which is subsequently extended for the sub-classes

#ifndef HK_MEMORY_CLASS_DEFINITIONS_H
#define HK_MEMORY_CLASS_DEFINITIONS_H

#define HK_MEMORY_CLASS_DEFINITION_START enum HK_MEMORY_CLASS {
#define HK_MEMORY_CLASS_DEFINITION_END HK_MEMORY_CLASS_MAX };
#define HK_MEMORY_CLASS(A,B) HK_MEMORY_CLASS_ ## A,

#include <Common/Base/Memory/MemoryClasses/hkMemoryClasses.h>

#undef HK_MEMORY_CLASS_DEFINITION_START
#undef HK_MEMORY_CLASS_DEFINITION_END
#undef HK_MEMORY_CLASS

#endif //HK_MEMORY_CLASS_DEFINITIONS_H

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
