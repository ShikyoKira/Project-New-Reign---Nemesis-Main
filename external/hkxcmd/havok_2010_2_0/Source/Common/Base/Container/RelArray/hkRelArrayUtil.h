/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_RELARRAYUTIL_H
#define HK_RELARRAYUTIL_H

// Utility to build offsets for objects containing hkRelArrays
//
// create one hkArrayBuilder per object,
// as hkRelArrayBuilder<objType> arrayBuilder(&newObject);
//
// Then for each hkRelArray, call:
// arrayBuilder.addRelArray(newObject.relArray, relArraySize);
//

#include <Common/Base/Container/RelArray/hkRelArray.h>

template<typename EnclosingClass>
class hkRelArrayBuilder
{
	//+hk.MemoryTracker(ignore=True)
public:
	hkRelArrayBuilder(EnclosingClass* enclosingObject);

	template<typename RelArrayType>
	RelArrayType* addRelArray(hkArray<RelArrayType>& relArray);

protected:
	EnclosingClass* m_enclosingObject;
	int m_offsetSoFar;
};

class hkRelArrayUtil
{
public:
	enum
	{
		RELARRAY_ALIGNMENT = 16
	};
};

#include <Common/Base/Container/RelArray/hkRelArrayUtil.inl>

#endif //HK_RELARRAYUTIL_H

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
