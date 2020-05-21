/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CLASS_NAME_REGISTRY_H
#define HK_CLASS_NAME_REGISTRY_H

class hkClass;

	/// Associates string type names with hkClass objects.
class hkClassNameRegistry : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Get name of the registry.
		virtual const char* getName() const = 0;
			/// Get a class by name or HK_NULL if it was not registered.
		virtual const hkClass* getClassByName( const char* className ) const = 0;
			/// Get array of registered classes, e.g., to iterate through them.
		virtual void getClasses( hkArray<const hkClass*>& classes ) const = 0;
};

#endif // HK_CLASS_NAME_REGISTRY_H

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
