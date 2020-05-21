/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_VERTEXUTIL_H
#define HKANIMATION_VERTEXUTIL_H

#include <Common/SceneData/Mesh/hkxVertexBuffer.h>

/// This structure describes the memory layout and format of a vertex buffer.
/// Perhaps we should switch this for a hkClass and use the serialization framework?
class hkxVertexUtil
{
	public:
			/// Returns the byte alignment of a particular element
			/// Pass in one of the enumerated types above only
		static hkUlong HK_CALL getAlignment(hkxVertexDescription::DataUsage usage, const hkxVertexBuffer& buffer);

			/// Called by getAlignment(hkUint32 property, const hkxVertexBuffer& buffer), but
			/// can take any addr
		static hkUlong HK_CALL getAlignment(hkUlong addr);
};

#endif // HKANIMATION_VERTEXUTIL_H

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
