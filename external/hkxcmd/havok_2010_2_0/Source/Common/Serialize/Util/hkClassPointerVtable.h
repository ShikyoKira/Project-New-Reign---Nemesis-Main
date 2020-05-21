/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_hkClassPointerVtable_H
#define HK_hkClassPointerVtable_H

#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>

class hkClassNameRegistry;

	/// Utility classes for faking vtables.
	/// These utilities are used when methods require looking up an objects dynamic
	/// type from a base pointer type. Normally we would use the vtable but sometimes that
	/// is not available e.g., when the data is from an older version or if the library containing
	/// the vtables is not linked. We can replicate the functionality of vtables for the purpose
	/// of reflection by storing a hkClass pointer where the vtable would usually be.
namespace hkClassPointerVtable
{
	class TypeInfoRegistry : public hkTypeInfoRegistry
	{
		public:

			TypeInfoRegistry(const hkStringMap<const hkClass*>& classes);
			TypeInfoRegistry(const hkClassNameRegistry* classes);
			~TypeInfoRegistry();
			virtual const hkTypeInfo* finishLoadedObject( void* obj, const char* className ) const;

		protected:

			//const hkStringMap<const hkClass*>& m_classes;
			hkRefPtr<const hkClassNameRegistry> m_classes;
			mutable hkStringMap<hkTypeInfo*> m_typeInfos;
	};

	class VtableRegistry : public hkVtableClassRegistry
	{
		public:

			virtual void registerVtable( const void* vtable, const hkClass* klass );
			virtual const hkClass* getClassFromVirtualInstance( const void* obj ) const;
	};
}

#endif //HK_hkClassPointerVtable_H

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
