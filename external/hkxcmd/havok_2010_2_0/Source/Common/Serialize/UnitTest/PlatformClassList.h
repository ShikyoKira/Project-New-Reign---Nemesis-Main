/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef PLATFORM_CLASS_LIST_H
#define PLATFORM_CLASS_LIST_H

#include <Common/Serialize/Copier/hkDeepCopier.h>

struct PlatformClassList
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, PlatformClassList );
	PlatformClassList( const hkClass*const* list )
	{
		hkDeepCopier::CopyFromOriginal copyFromOriginal;
		for( const hkClass*const* orig = list; *orig != HK_NULL; ++orig )
		{
			void* copy = copyFromOriginal.getWithDefault( *orig, HK_NULL );
			if( copy == HK_NULL )
			{
				copy = hkDeepCopier::deepCopy( *orig, hkClassClass, &copyFromOriginal );
				m_allocs.pushBack(copy);
			}
			m_copies.pushBack( static_cast<hkClass*>(copy) );
		}
		m_copies.pushBack( HK_NULL );
	}
	~PlatformClassList()
	{
		for( int i = 0; i < m_allocs.getSize(); ++i )
		{
			hkDeepCopier::freeDeepCopy( m_allocs[i] );
		}
	}
	void computeOffsets( hkStructureLayout::LayoutRules rules )
	{
		hkStructureLayout layout(rules);

		hkPointerMap<const hkClass*, int> classesDone;
		for( hkClass* const* kp = m_copies.begin(); *kp != HK_NULL; ++kp )
		{
			layout.computeMemberOffsetsInplace( **kp, classesDone );
		}
	}

	hkArray<hkClass*> m_copies;
	hkArray<void*> m_allocs;
};

#endif // PLATFORM_CLASS_LIST_H

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
