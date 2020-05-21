/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MEMORY_CLASS_TABLE_H
#define HK_MEMORY_CLASS_TABLE_H

struct hkMemoryClassStatistics
{
	hk_size_t m_size_in_use;
	int m_n_allocates;
	int m_n_news;
	hk_size_t m_new_size_in_use;
	int m_blocks_in_use;
	hk_size_t m_max_size_in_use;

	void init()
	{
		m_blocks_in_use = 0;
		m_max_size_in_use = 0;
		m_n_allocates = 0;
		m_n_news = 0;
		m_new_size_in_use = 0;
		m_size_in_use = 0;
	}

	void operator += (const hkMemoryClassStatistics& b)
	{
		this->m_blocks_in_use += b.m_blocks_in_use;
		this->m_max_size_in_use += b.m_max_size_in_use;
		this->m_n_allocates += b.m_n_allocates;
		this->m_n_news += b.m_n_news;
		this->m_new_size_in_use += b.m_new_size_in_use;
		this->m_size_in_use += b.m_size_in_use;
	}

	void operator -= (const hkMemoryClassStatistics& b)
	{
		this->m_blocks_in_use -= b.m_blocks_in_use;
		this->m_max_size_in_use -= b.m_max_size_in_use;
		this->m_n_allocates -= b.m_n_allocates;
		this->m_n_news -= b.m_n_news;
		this->m_new_size_in_use -= b.m_new_size_in_use;
		this->m_size_in_use -= b.m_size_in_use;
	}

};

struct hkMemoryClassInfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkMemoryClassInfo);
	hkMemoryClassStatistics		m_classStats;
	const char*					m_name;
	int							m_parent;			//the parent class for this class
	int							m_numSubClasses;	//the number of subclasses

	hkMemoryClassInfo(const char* n,int parent)
		: m_name(n), m_parent(parent)
	{
	}
};

extern hkMemoryClassInfo hkMemoryClassesTable[];

#endif //HK_MEMORY_CLASS_TABLE_H

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
