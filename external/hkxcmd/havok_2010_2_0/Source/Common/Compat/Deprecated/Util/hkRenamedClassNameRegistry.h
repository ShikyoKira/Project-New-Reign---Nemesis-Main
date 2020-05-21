/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_RENAMED_CLASSNAME_REGISTRY_H
#define HK_SERIALIZE_RENAMED_CLASSNAME_REGISTRY_H

#include <Common/Base/Reflection/Registry/hkClassNameRegistry.h>
#include <Common/Compat/Deprecated/Version/hkVersionRegistry.h>

class hkRenamedClassNameRegistry : public hkClassNameRegistry
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		/// This takes an existing reg and just queries into it
		hkRenamedClassNameRegistry( const hkVersionRegistry::ClassRename* renames = HK_NULL, const hkClassNameRegistry* originalRegistry = HK_NULL )
			: m_originalRegistry(HK_NULL)
		{
			setOriginalRegistry( originalRegistry );
			registerRenames( renames );
		}
		~hkRenamedClassNameRegistry()
		{
			if( m_originalRegistry )
			{
				m_originalRegistry->removeReference();
			}
		}

		virtual const char* getName() const
		{
			return m_originalRegistry ? m_originalRegistry->getName() : HK_NULL;
		}

		virtual void getClasses( hkArray<const hkClass*>& classes ) const
		{
			if( m_originalRegistry )
			{
				m_originalRegistry->getClasses(classes);
			}
		}

		virtual const hkClass* getClassByName( const char* oldname ) const
		{
			const char* name = m_renames.getWithDefault( oldname, oldname );
			return m_originalRegistry ? m_originalRegistry->getClassByName(name) : HK_NULL;
		}

		void setOriginalRegistry( const hkClassNameRegistry* originalRegistry )
		{
			if( originalRegistry )
			{
				originalRegistry->addReference();
			}
			if( m_originalRegistry )
			{
				m_originalRegistry->removeReference();
			}
			m_originalRegistry = originalRegistry;
		}

		const char* getRename( const char* oldName ) const
		{
			return m_renames.getWithDefault(oldName, HK_NULL);
		}

		void registerRenames( const hkVersionRegistry::ClassRename* renames )
		{
			if( renames )
			{
				for( const hkVersionRegistry::ClassRename* r = renames; r->oldName != HK_NULL; ++r )
				{
					m_renames.insert( r->oldName, r->newName );
				}
			}		
		}

		hkStringMap<const char*> m_renames;
		const hkClassNameRegistry* m_originalRegistry;
};

#endif // HK_SERIALIZE_RENAMED_CLASSNAME_REGISTRY_H

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
