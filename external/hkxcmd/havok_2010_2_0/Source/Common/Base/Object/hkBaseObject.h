/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKBASEOBJECT_H
#define HKBASE_HKBASEOBJECT_H

extern const hkClass hkBaseObjectClass;

/// Base class for all Havok classes that have virtual functions.
/// In gcc2 for instance, if the virtual base class has data in it the vtable
/// is placed after the data, whereas most other compilers always have the vtable
/// at the start. Thus we have an empty virtual base class to force the vtable
/// to always be at the start of the derived objects.
/// All Havok managed objects inherit from a sub class of this, hkReferencedObject
/// that stores the memory size and the reference count info (if used).
class hkBaseObject
{
	//+hk.MemoryTracker(ignore=True)
	public:
		HK_DECLARE_REFLECTION();
		
			/// Virtual destructor for derived objects
		HK_FORCE_INLINE virtual ~hkBaseObject() {}

	public:

		HK_FORCE_INLINE hkBaseObject() { }
		HK_FORCE_INLINE hkBaseObject( class hkFinishLoadedObjectFlag flag ) {}
};

hkBool::CompileTimeFalseType hkIsVirtual(void*);
hkBool::CompileTimeTrueType hkIsVirtual(hkBaseObject*);

#endif // HKBASE_HKBASEOBJECT_H

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
