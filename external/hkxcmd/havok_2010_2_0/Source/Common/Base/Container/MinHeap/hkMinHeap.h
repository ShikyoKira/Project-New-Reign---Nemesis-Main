/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MIN_HEAP_H
#define HK_MIN_HEAP_H

// Include for swap
#include <Common/Base/Algorithm/Sort/hkSort.h>

template <typename T>
struct hkMinHeapDefaultOperations
{
	public:
		HK_FORCE_INLINE static hkBool32 lessThan( const T& a, const T& b ) { return ( a < b ); }
			/// Set index
		HK_FORCE_INLINE static void setIndex(const T& a, int index ) {}
			/// Swap
		HK_FORCE_INLINE static void swap(T& a, T& b) { hkAlgorithm::swap(a, b);}
			/// Returns true if the index is correct.
		HK_FORCE_INLINE static hkBool32 hasIndex(const T& a, int i) { return true; }
};

template <typename T, typename OPS = hkMinHeapDefaultOperations<T> >
class hkMinHeap
{
	public:
		typedef hkMinHeap<T, OPS> ThisType;
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, ThisType);
		
			/// Get the index of the parent of an index. Returns -1 if the parent of the root is requested.
		HK_FORCE_INLINE static int getParent(int i) { HK_ASSERT(0x3423a432, i >= 0); return (i - 1) >> 1;}
			/// Get the index of the left child on the heap (the index will be out of range if there isn't one)
		HK_FORCE_INLINE static int getLeftChild(int i) {return (i + i + 1);}
			/// Get the index of the right child on the heap (the index will be out of range if there isn't one)
		HK_FORCE_INLINE static int getRightChild(int i) {return (i + i + 2);}

			/// Removes the entry at index i
		void removeEntry(int i);
			/// Add an entry
		void addEntry(const T& entry);

			/// Get an entry
		T& getEntry(int i) { return m_contents[i]; }
			/// Get an entry
		const T& getEntry(int i) const { return m_contents[i]; }

			/// Pop the top entry
		void popTop() { removeEntry(0); }
			/// The top holds the minimum entry
		const T& getTop() const { return m_contents[0]; }

			/// Returns true if its empty
		hkBool32 isEmpty() const { return m_contents.getSize() == 0; }
			/// Call if an entry is altered, such that its cost value is changed. Returns the final index of the entry.
		int setEntry(int i, const T& entry);

			/// Empty the contents
		void clear() { m_contents.clear(); }

			/// Reorders element i, upwards (if necessary). Call if an alteration to element i, makes its cost less than previously.
			/// Assuming the heap was previously 'OK' calling after such a change will keep it OK.
		int upHeap(int i);
			/// Reorders element i, downwards (if necessary). Call if an alteration to element i, makes it cost more than previously.
			/// Assuming the heap was previously 'OK' calling after such a change will keep it OK.
		int downHeap(int i);

			/// Returns true if the heap is correctly formed
		hkBool32 isOk() const;

			/// Get the amount of members
		int getSize() const { return m_contents.getSize(); }

			/// Optimize allocations
		void reserve(int n) { m_contents.reserve(n); }

			/// Get the contents
		hkArray<T>& getContents() { return m_contents; }
			/// Get the const contents
		const hkArray<T>& getContents() const { return m_contents; }

	protected:
		// Get the index of a smaller child on the heap. If doesn't have a smaller child will return an index >= num entries
		int _getSmallerChild(int i) const;

		hkArray<T> m_contents;
};

#include <Common/Base/Container/MinHeap/hkMinHeap.inl>

#endif // HK_MIN_HEAP_H

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
