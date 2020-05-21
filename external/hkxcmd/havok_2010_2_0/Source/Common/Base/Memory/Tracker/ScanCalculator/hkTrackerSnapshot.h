/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_TRACKER_SNAPSHOT_H
#define HKBASE_TRACKER_SNAPSHOT_H

#include <Common/Base/Memory/Tracker/Default/hkDefaultMemoryTracker.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>

#include <Common/Base/System/StackTracer/hkStackTracer.h>

struct hkSubString;
class hkMemorySystem;

class hkTrackerSnapshot  //+reflected(false)
{
    public:
        HK_DECLARE_PLACEMENT_ALLOCATOR();
		typedef hkDefaultMemoryTracker::ClassAlloc ClassAlloc;

		struct Allocation //+reflected(false)
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,Allocation);

				/// ==
			hkBool operator==(const Allocation& rhs) const
			{
				return m_start == rhs.m_start &&
					m_size == rhs.m_size;
					//m_traceId == rhs.m_traceId;
			}
				/// !=
			hkBool operator!=(const Allocation& rhs) const { return !(*this == rhs); }

			void* m_start;
			hk_size_t m_size;
			hkStackTracer::CallTree::TraceId m_traceId;		// If < 0 it is not valid
		};

			/// Initialize
		hkResult init(hkMemorySystem* memorySystem = HK_NULL, hkDefaultMemoryTracker* tracker = HK_NULL);
			/// Find what allocation this points to
		const Allocation* findAllocation(const void* ptrIn) const;
			/// Finds the allocation starting at address ptrIn
		const Allocation* findExactAllocation(const void* ptrIn) const;

			/// Returns HK_SUCCESS if the data appears consistent
		hkResult checkConsistent() const;

			/// Work out the total amount of memory allocated (in bytes)
		hk_size_t calcTotalAllocated() const;

			/// For each class find the allocation it is associated with. HK_NULL if not found.
		void findClassAllocation(hkArray<const Allocation*>& allocs) const;

			/// Find the class allocation at the address specified
		const ClassAlloc* findClassAllocation(void* ptr) const;

			/// Get the call tree. Can be used with the m_traceId, to get a call stack for each allocation.
		const hkStackTracer::CallTree& getCallTree() const { return m_callTree; }

			/// Get the class allocs (ordered by start pointer)
		const hkArray<ClassAlloc>& getClassAllocs() const { return m_classAllocations; }

			/// Write access to class allocs. If the data is changed, a call to orderClassAllocs is needed
			/// for functionality such as findClassAllocation to work
		hkArray<ClassAlloc>& getClassAllocs() { return m_classAllocations; }
			/// Order class allocs. The classAllocs must be ordered for findClassAllocation to work. Generally
			/// it is - but getClassAllocs can be used to modify allocs. If that happens, call
			/// orderClassAllocs to fix up the order.
		void orderClassAllocs();

			/// Get all of the allocations (ordered by start pointer)
		const hkArray<Allocation>& getAllocations() const { return m_allocations; }
			/// Swap
		void swap(hkTrackerSnapshot& rhs);

			/// ==
		hkBool operator==(const hkTrackerSnapshot& rhs) const;
			/// !=
		hkBool operator!=(const hkTrackerSnapshot& rhs) const { return !(*this == rhs); }

		/// Dump details (including stack trace if there is one) about an allocation
		void dumpAllocation(const Allocation& alloc, hkOstream& stream) const;

			/// Returns true if the allocation was found and removed
		hkBool removeAllocation(const void* ptr);

			/// Works out the allocations which are different
		static void HK_CALL allocationDiff(const hkTrackerSnapshot& snapA, const hkTrackerSnapshot& snapB, hkArray<Allocation>& onlyInA, hkArray<Allocation>& onlyInB);

			/// Find an allocation in an array. The array is assumed sorted by start address (as it is in the snapshot)
		static const Allocation* findAllocation(const hkArray<Allocation>& allocations, const void* ptrIn);

			/// Ctor
		hkTrackerSnapshot();
			/// Copy ctor
		hkTrackerSnapshot(const hkTrackerSnapshot& rhs);
			/// Dtor
		~hkTrackerSnapshot();

		

    protected:
		void _removeAllocation(void* start);
	

		hkArray<ClassAlloc> m_classAllocations;
		hkArray<Allocation> m_allocations;

		hkStackTracer::CallTree m_callTree;
		hkBool m_isInitialized;
};

#endif // HKBASE_TRACKER_SNAPSHOT_H

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
