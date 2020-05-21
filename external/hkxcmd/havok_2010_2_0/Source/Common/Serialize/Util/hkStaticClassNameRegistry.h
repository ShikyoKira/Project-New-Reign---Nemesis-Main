/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_STATIC_CLASS_NAME_REGISTRY_H
#define HK_STATIC_CLASS_NAME_REGISTRY_H

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/Registry/hkClassNameRegistry.h>

	/// The class can be used to create static class name registries.
	/// The hkClass size and member offsets are recomputed lazily if the
	/// the hkStaticClassNameRegistry object was created with the
	/// c-array of non-const hkClass pointers.
class hkStaticClassNameRegistry : public hkClassNameRegistry
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkStaticClassNameRegistry(hkClass*const* classes, int classVersion, const char* name);
		hkStaticClassNameRegistry(const hkClass*const* classes, int classVersion, const char* name);

		virtual const char* getName() const;
		virtual const hkClass* getClassByName(const char* className) const;
		virtual void getClasses(hkArray<const hkClass*>& classes) const;

	private:

		void checkIfReady() const;

		const char* m_name;
		const hkClass*const* m_classes;
		int m_classVersion;
		mutable hkBool32 m_ready;
};

#endif // HK_STATIC_CLASS_NAME_REGISTRY_H

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
