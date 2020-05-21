/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_SERIALIZE_DISPLAYMARKER_LIST_H
#define HK_UTILITIES2_SERIALIZE_DISPLAYMARKER_LIST_H

#include <Physics/Utilities/Serialize/Display/hkpSerializedDisplayMarker.h>

extern const class hkClass hkpSerializedDisplayMarkerListClass;

class hkpSerializedDisplayMarkerList : public hkReferencedObject
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);
	HK_DECLARE_REFLECTION();

	inline hkpSerializedDisplayMarkerList();

	inline virtual ~hkpSerializedDisplayMarkerList();

		/// Get the number of markers mapped by this class.
	inline int getNumMarkers() const;
	
		/// Make sure that pi is with range.
	inline hkpSerializedDisplayMarker* getMarker(int pi) const;

		/// Add a marker to the list
	inline void addMarker( hkpSerializedDisplayMarker* m );

public:
	
	hkArray<hkpSerializedDisplayMarker*> m_markers; // the transform for this marker.

	hkpSerializedDisplayMarkerList(hkFinishLoadedObjectFlag flag) : hkReferencedObject(flag), m_markers(flag) { }
};

#include <Physics/Utilities/Serialize/Display/hkpSerializedDisplayMarkerList.inl>

#endif // HK_UTILITIES2_SERIALIZE_DISPLAYMARKER_LIST_H

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
