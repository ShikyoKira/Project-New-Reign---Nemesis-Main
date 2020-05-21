/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCENEDATA_HKX_ANIMATED_BOOL_H
#define HK_SCENEDATA_HKX_ANIMATED_BOOL_H

extern const hkClass hkxSparselyAnimatedBoolClass;

	/// An hkxSparselyAnimatedBool stores the values associated with an hkxAttribute of type bool. The animation of values is stored as
	/// pairs (time, value).
class hkxSparselyAnimatedBool : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
public: 
	
	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
	HK_DECLARE_REFLECTION();

	hkxSparselyAnimatedBool() { }
	hkxSparselyAnimatedBool(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_bools(f), m_times(f) {}

		/// The array of sampled bools. The array can be of any length but
		/// the time stamps in the m_times array must be of the same length
		/// as there is a one to one mapping between the two arrays.
	hkArray<hkBool> m_bools;
	
		/// The array of times associated with teach of the values in m_bools.
	hkArray<float> m_times;
};

#endif //HK_SCENEDATA_HKX_ANIMATED_BOOL_H

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
