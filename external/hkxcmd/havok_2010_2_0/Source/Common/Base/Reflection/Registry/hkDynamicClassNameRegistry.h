/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DYNAMIC_CLASS_NAME_REGISTRY_H
#define HK_DYNAMIC_CLASS_NAME_REGISTRY_H

#include <Common/Base/Reflection/Registry/hkClassNameRegistry.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>
#include <Common/Base/Reflection/hkClass.h>

	/// The class implements the hkClassNameRegistry interfaces
	/// and allows dynamically change the registry.
class hkDynamicClassNameRegistry : public hkClassNameRegistry
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Create dynamic class name registry with specific name,
			/// the name is HK_NULL by default.
		hkDynamicClassNameRegistry(const char* name = HK_NULL) : m_name(name) {}

			/// Implements hkClassNameRegistry::getName()
		virtual const char* getName() const
		{
			return m_name;
		}

			/// Set dynamic registry name
		virtual void setName(const char* name)
		{
			m_name = name;
		}

			/// Implements hkClassNameRegistry::getClassByName()
		virtual const hkClass* getClassByName( const char* className ) const
		{
			return m_map.getWithDefault(className, HK_NULL);
		}

			/// Implements hkClassNameRegistry::getClasses()
		virtual void getClasses( hkArray<const hkClass*>& classes ) const
		{
			hkStringMap<const hkClass*>::Iterator iter = m_map.getIterator();
			while (m_map.isValid(iter))
			{
				classes.expandOne() = m_map.getValue(iter);
				iter = m_map.getNext(iter);
			}
		}

			/// Register a class possibly under a different name.
			/// If name is null, the class name is used.
			/// The name is not copied and must be valid for the lifetime
			/// of this object.
		virtual void registerClass( const hkClass* klass, const char* name = HK_NULL )
		{
			m_map.insert( name ? name : klass->getName(), klass );
		}

			/// Register a null terminated list of classes.
		virtual void registerList( const hkClass* const * classes)
		{
			const hkClass* const * ci = classes;
			while(*ci != HK_NULL)
			{
				registerClass( *ci );
				++ci;
			}
		}

			/// Merges all entries from "mergeFrom" string map (potentially overwriting current entries).
		virtual void merge(const hkStringMap<const hkClass*>& mergeFromMap)
		{
			hkStringMap<const hkClass*>::Iterator iter = mergeFromMap.getIterator();
			while (mergeFromMap.isValid(iter))
			{
				registerClass( mergeFromMap.getValue(iter) );
				iter = mergeFromMap.getNext(iter);
			}
		}

			/// Merges all entries from "mergeFrom" class name registry (potentially overwriting current entries).
		virtual void merge(const hkClassNameRegistry& mergeFrom)
		{
			hkArray<const hkClass*> classes;
			mergeFrom.getClasses(classes);
			for( int i = 0; i < classes.getSize(); ++i )
			{
				registerClass( classes[i] );
			}
		}

	protected:

		const char* m_name;
		hkStringMap<const hkClass*> m_map;
};

#endif // HK_DYNAMIC_CLASS_NAME_REGISTRY_H

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
