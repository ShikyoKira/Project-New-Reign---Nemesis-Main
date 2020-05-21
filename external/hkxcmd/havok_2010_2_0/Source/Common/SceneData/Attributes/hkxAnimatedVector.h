/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCENEDATA_HKX_ANIMATED_VECTOR_H
#define HK_SCENEDATA_HKX_ANIMATED_VECTOR_H

#include <Common/SceneData/Attributes/hkxAttribute.h>

extern const hkClass hkxAnimatedVectorClass;

	/// An hkxAnimatedVector stores the values associated with an hkxAttribute of type vector.
class hkxAnimatedVector : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
public: 
	
	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
	HK_DECLARE_REFLECTION();

	hkxAnimatedVector() {}
	hkxAnimatedVector(hkFinishLoadedObjectFlag f) :  hkReferencedObject(f), m_vectors(f) {}

		/// The array of sampled vectors. Array can be of length 1 for not animated,
		/// or one per frame so assumes that the value is then fully sampled.
	hkArray<hkVector4> m_vectors;
	
		/// Usage hint so that automatic transform filters can attempt
		/// to change the value in a sensible way.
	hkEnum< hkxAttribute::Hint,hkUint8> m_hint;
};


#endif // HK_SCENEDATA_HKX_ANIMATED_VECTOR_H

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
