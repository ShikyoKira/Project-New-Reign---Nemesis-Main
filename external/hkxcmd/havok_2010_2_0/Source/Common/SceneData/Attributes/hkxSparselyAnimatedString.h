/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCENEDATA_HKX_ANIMATED_STRING_H
#define HK_SCENEDATA_HKX_ANIMATED_STRING_H

extern const hkClass hkxSparselyAnimatedStringClass;

	/// An hkxSparselyAnimatedString stores the values associated with an hkxAttribute of type string. The animation of values is stored as
	/// pairs (time, value).
class hkxSparselyAnimatedString : public hkReferencedObject
{
	//+vtable(true)
	//+version(2)
public: 
	
	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
	HK_DECLARE_REFLECTION();

	hkxSparselyAnimatedString() { }
	hkxSparselyAnimatedString(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_strings(f), m_times(f) {}

		/// The array of string values. We put those string (char*) in a struct since char** would not be interpreted by the
		/// serialization as an array of strings.
	hkArray<hkStringPtr> m_strings;
	
		/// The array of times associated with teach of the values in m_strings.
	hkArray<float> m_times;
};


#endif // HK_SCENEDATA_HKX_ANIMATED_STRING_H

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
