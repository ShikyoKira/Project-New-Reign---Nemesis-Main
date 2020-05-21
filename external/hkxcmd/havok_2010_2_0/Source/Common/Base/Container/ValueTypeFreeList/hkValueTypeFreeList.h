/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_VALUE_TYPE_FREE_LIST_H
#define HKBASE_VALUE_TYPE_FREE_LIST_H

#ifndef __HAVOK_PARSER__
#include <new>
#endif



/// Array based free list of VALUE_TYPE items, allocations and deallocations calls ctor and dtor.
///
/// NOTE: This is a low-level allocation/storage facility, not meant to be used as a general purpose collection.
/// NOTE 2: INDEX_TYPE must implement hkHandle.
/// WARNING: The first 32bits of a VALUE_TYPE instance is overwritten if not allocated.
///
template <typename VALUE_TYPE, typename INDEX_TYPE, int GROWTH>
struct hkValueTypeFreeList
{
	//+hk.ReflectedFile("CompoundShape")
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkValueTypeFreeList);
	HK_DECLARE_REFLECTION();

	hkValueTypeFreeList(hkFinishLoadedObjectFlag f) : m_items(f) {}


	/// Constructor.
	hkValueTypeFreeList()
	{
		clear();
	}

	/// Clear the free list
	inline void							clear()
	{
		m_items.clear();
		m_firstFree=-1;
	}

	/// Allocate one item, call VALUE_TYPE ctor.
	inline INDEX_TYPE					allocate()
	{
		if(m_firstFree<0)
		{
			HK_COMPILE_TIME_ASSERT(sizeof(VALUE_TYPE)>=sizeof(hkInt32));
			const int	oldCapacity = m_items.getSize();
			m_items.reserveExactly(m_items.getSize()+GROWTH); m_items.expandBy(GROWTH);
			for(int i=oldCapacity,last=m_items.getSize()-1; i<=last; ++i)
			{
				((hkInt32&)m_items[i]) = (hkInt32) (i<last? i+1 : -1);
			}
			m_firstFree	=	oldCapacity;
		}
		hkInt32	item = m_firstFree;
		m_firstFree = (hkInt32&)m_items[item];
		new(&m_items[item]) VALUE_TYPE();
		return INDEX_TYPE(item);
	}

	/// Release one item, call VALUE_TYPE dtor.
	inline void							release(INDEX_TYPE index)
	{
		(&m_items[index.value()])->~VALUE_TYPE();
		((hkInt32&)m_items[index.value()]) = m_firstFree;
		m_firstFree = index.value();
	}	

	/// Read-write indexed access.
	HK_FORCE_INLINE	VALUE_TYPE&			operator[](INDEX_TYPE index)			
	{ 		
		return m_items[index.value()]; 
	}

	/// Read-only indexed access.
	HK_FORCE_INLINE	const VALUE_TYPE&	operator[](INDEX_TYPE index) const
	{
		return m_items[index.value()];
	};

	/// Get the item index from its reference.
	HK_FORCE_INLINE	INDEX_TYPE			indexOf(const VALUE_TYPE& item) const	{ return INDEX_TYPE(((&item)-m_items.begin())); }

	/// Get the capacity of the free list.
	HK_FORCE_INLINE int					getCapacity() const					{ return m_items.getSize(); }

	/// Get the end index of the free list.
	HK_FORCE_INLINE INDEX_TYPE			getEnd() const						{ return INDEX_TYPE(getCapacity()); }
		
	/// Get an read-only element from its index using software cache on if called from an SPU.
	HK_FORCE_INLINE	const VALUE_TYPE&	getAtWithCache(INDEX_TYPE index) const
	{ 		
		return m_items[index.value()];
	}

private:
	
	hkArray<VALUE_TYPE>	m_items;		///< Item storage (free and allocated).
	hkInt32				m_firstFree;	///< First free item index.
};

#endif // HKBASE_VALUE_TYPE_FREE_LIST_H

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
