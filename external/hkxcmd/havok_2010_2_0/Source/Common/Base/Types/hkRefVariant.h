/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKREFVARIANT_H
#define HKBASE_HKREFVARIANT_H

class hkVtableClassRegistry;

/// A generic reference counted object with metadata.

class hkRefVariant: public hkRefPtr<hkReferencedObject>
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkRefVariant);

		HK_FORCE_INLINE hkRefVariant();
		HK_FORCE_INLINE hkRefVariant(const hkReferencedObject* o);
		HK_FORCE_INLINE hkRefVariant(const hkRefVariant& other);
		hkRefVariant(const hkVariant& v);
		hkRefVariant(void* o, const hkClass* k);
		HK_FORCE_INLINE hkRefVariant(hkFinishLoadedObjectFlag f);

		HK_FORCE_INLINE void operator=(const hkRefVariant& other);
		HK_FORCE_INLINE void operator=(const hkReferencedObject* o);

		void set(void* o, const hkClass* k);

		HK_FORCE_INLINE operator hkVariant () const;
		const hkClass* getClass() const;
};


#include <Common/Base/Types/hkRefVariant.inl>

#endif // HKBASE_HKREFVARIANT_H

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
