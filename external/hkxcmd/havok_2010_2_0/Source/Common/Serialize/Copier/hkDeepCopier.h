/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_COPIER_DEEP_COPIER_H
#define HK_SERIALIZE_COPIER_DEEP_COPIER_H

#include <Common/Serialize/Copier/hkObjectCopier.h>

template <typename K, typename V, typename Allocator> class hkPointerMap;

class hkVtableClassRegistry;
class hkTypeInfoRegistry;

/// Utility to copy an object and all pointed objects.
namespace hkDeepCopier
{
		/// Source To
	typedef hkPointerMap<const void*, void*> CopyFromOriginal;

		/// Copy an object and all pointed objects.
		/// Copied objects are flattened into a single allocation
		/// like a headerless packfile.
		/// If copyFromOriginal is not null, it is checked for previously copied objects
		/// and deep copying uses these objects instead of recursing. Newly
		/// copied objects are also written into it on completion.
		/// If vtableClassRegistry is not HK_NULL, it will be used to find out
		/// the derived class of the objects being copied. It is required
		/// if you are copying polymorphic objects. If you want the objects
		/// to be "finished" (setting their vtables, etc), you must also supply
		/// a non-null finishLoadedObjectRegistry.
	void* HK_CALL deepCopy(	const void* dataIn, 
							const hkClass& klassIn, 
							CopyFromOriginal* copyFromOriginal, 
							hkVtableClassRegistry* vtableClassRegistry = HK_NULL,
							hkTypeInfoRegistry* loadedObjectRegistry = HK_NULL,
							hkObjectCopier::ObjectCopierFlags flags = hkObjectCopier::FLAG_NONE );

		/// Free an object that has been deep copied
		/// This will call destructors for any object that
		/// requires it and release the allocated memory
	hkResult freeDeepCopy(void* data);
}

#endif //HK_SERIALIZE_COPIER_DEEP_COPIER_H

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
