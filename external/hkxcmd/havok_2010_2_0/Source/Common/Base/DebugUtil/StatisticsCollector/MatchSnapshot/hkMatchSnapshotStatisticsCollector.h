/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MATCH_SNAPSHOT_STATISTICS_COLLECTOR_H
#define HK_MATCH_SNAPSHOT_STATISTICS_COLLECTOR_H

#include <Common/Base/DebugUtil/StatisticsCollector/hkStatisticsCollector.h>
#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Base/Memory/System/Util/hkDebugMemorySnapshot.h>

class hkMatchSnapshotStatisticsCollector;

class hkMatchSnapshotStatisticsCollector : public hkStatisticsCollector
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

	private:

		class AllocationSet
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_DEBUG_ALLOCATOR(HK_MEMORY_CLASS_BASE, AllocationSet);

				enum { MAX_ALLOCS = 8 };

					/// Ctor
				AllocationSet(const void* ptr,const hkDebugMemorySnapshot::PointerInfo* info, hk_size_t allocatedSize);

					/// Add an allocation to this
				void addAlloc(const void* ptr,const hkDebugMemorySnapshot::PointerInfo* info, hk_size_t allocatedSize);

					/// Returns true if the allocations 'match' - in particular the stack trace matches
				hkBool matches(int traceDepth,const void* ptr,const hkDebugMemorySnapshot::PointerInfo* info);

					/// Dump out the contained information
				void dump( const hkMatchSnapshotStatisticsCollector& collector) const;

					/// Returns true if any part of the call stack contains the string
				hkBool callStackContainsString(hkMatchSnapshotStatisticsCollector& collector, const char* in);

					///
				void getCallStackString( const hkMatchSnapshotStatisticsCollector& collector, hkStringBuf& s) const;

					/// more low level version of above function
				void getCallStackString( const hkMatchSnapshotStatisticsCollector& collector, char* buffer, int bufferLen ) const;

				static void HK_CALL _printFunc(const char*, void* context);

				static void HK_CALL _concatFunc(const char* text, void* context);

				//
				// Members
				//

				int m_totalAllocs;

				hkUlong m_sumAllocatedSize;

				// Number of pointers we have stored
				hkUint8 m_numStoredAllocs;

				/// Allocations of this type
				const void* m_allocs[MAX_ALLOCS];

				/// Info
				const hkDebugMemorySnapshot::PointerInfo* m_info;

				/// Linked to the next allocation
				AllocationSet* m_next;
		};

	public:
        enum { MAX_TRACE = 16 };

            /// Ctor
        hkMatchSnapshotStatisticsCollector(hkVtableClassRegistry* vtblReg, const hkDebugMemorySnapshot& snapshot,hkOstream& stream);

        // hkStatisticsCollector interface
        virtual void addReferencedObject(const char* fieldName, const hkReferencedObject* obj, int flags = 0);
        virtual void addObject( const hkClass& cls, const char* fieldName, const void* obj, int flags = 0);
        virtual void addChunk(MemoryType type, const char* name, const void* chunkAddress, int usedSize, int allocatedSize = 0);
		virtual void pushDir( const char* dirName ) {}
		virtual void popDir() {}

            /// Dump how many are remaining
        void dumpRemaining() const;

			/// Return the total size of all remaining
		void getTotalSizeInfo( hk_size_t& sumRequestedSizeOut, hk_size_t& sumAllocatedSizeOut ) const;


            /// Marks all these allocations as already reached
		void addReached( const hkDebugMemorySnapshot& snapshot);

            /// Get the strings that are ignored
        hkArrayBase<hkStringPtr>& getCallStackIgnoreStrings() { return m_callStackIgnoreStrings; }

            /// Add an ignore string
        void addCallStackIgnoreString(const char* in);

	protected:

        static void HK_CALL _printFunc(const char*, void* context);

        static HK_FORCE_INLINE hkBool _compareAllocs( const AllocationSet* a, const AllocationSet* b )
		{
            return (a->m_info->m_numBytes > b->m_info->m_numBytes);
        }


        hkBool _isKnown(const void* obj);
        void _addKnown(const void* obj);
		hkBool _notReachable(const void* ptr);

			/// Dump the stack trace for an object which can be reached by calcContentStatistics, but
			/// is not known by the memory allocator. Potentially the wrong allocator is used.
		void _dumpStackTraceForUnknown(const void* ptr);

        hkOstream& m_stream;

        const hkDebugMemorySnapshot& m_snapshot;

            /// Map of pointers
        hkPointerMap<const void*, const hkDebugMemorySnapshot::PointerInfo*, hkContainerDebugAllocator> m_chunks;

            /// As we traverse we can remove pointers that are reached
        hkPointerMap<const void*, int, hkContainerDebugAllocator> m_notReached;

            /// Known objects
        hkPointerMap<const void*, int, hkContainerDebugAllocator> m_knownObjects;

            /// List of strings, which if found in the call stack will cause the
            /// list to be ignored
		hkArray<hkStringPtr>::Debug m_callStackIgnoreStrings;
};


#endif // HK_MATCH_SNAPSHOT_STATISTICS_COLLECTOR_H

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
