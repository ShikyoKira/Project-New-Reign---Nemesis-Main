/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_REPORT_STATISTICS_COLLECTOR_H
#define HK_REPORT_STATISTICS_COLLECTOR_H

#include <Common/Base/DebugUtil/StatisticsCollector/hkStatisticsCollector.h>
#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

class hkStatisticClassCount
{
    public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkStatisticClassCount);
		typedef hkStatisticsCollector::MemoryType MemoryType;

			/// Ctor
        hkStatisticClassCount(const hkClass* cls = HK_NULL);
			/// Add a usage
		void addAllocation(MemoryType type, int used, int allocated);

            /// Add cumulative allocation
        void addCumulativeAllocation(int used, int allocated) { m_cumulativeAllocatedBytes += allocated; m_cumulativeUsedBytes += used; m_numCumulativeAllocations++; }
			/// Dump the details
		void dump(hkOstream& out);
			/// Resets the count
		void reset();

		int getTotalAllocated() const { return _calcTotal(m_totalAllocatedBytes); }
		int getTotalUsed() const { return _calcTotal(m_totalUsedBytes); }
		int getTotalAllocations() const { return _calcTotal(m_numAllocations); }

	protected:
		static int HK_CALL _calcTotal(const int* in);

	public:

        // Class -> can be HK_NULL
        const hkClass* m_class;
		// Number of allocations of this type of object
		int m_numObjects;

		// The number of allocations of that type
		int m_numAllocations[hkStatisticsCollector::MEM_LAST];
		// The total allocated for the memory type
        int m_totalAllocatedBytes[hkStatisticsCollector::MEM_LAST];
		// The total number of bytes used for a type
        int m_totalUsedBytes[hkStatisticsCollector::MEM_LAST];

        int m_cumulativeAllocatedBytes;             ///< Total bytes for this and all of its children
        int m_cumulativeUsedBytes;                  ///< Total bytes for this and all of its children
		int m_numCumulativeAllocations;				///< All allocations including children
};

class hkReportStatisticsCollector: public hkStatisticsCollector
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
            /// Ctor
        hkReportStatisticsCollector(hkVtableClassRegistry* vtblReg, hkOstream& stream);
            /// Dtor
        ~hkReportStatisticsCollector() { _clear(); }

            /// Called before any reporting
        void start();
            /// Called at the end of reporting - will give a summary
        void end();

        // hkStatisticsCollector interface
        virtual void addReferencedObject(const char* fieldName, const hkReferencedObject* obj, int flags = 0) ;
        // hkStatisticsCollector interface
        virtual void addObject( const hkClass& cls, const char* fieldName, const void* obj, int flags = 0);
        // hkStatisticsCollector interface
        virtual void addChunk(MemoryType type, const char* name, const void* chunkAddress, int usedSize, int allocatedSize = 0) ;
        // hkStatisticsCollector interface
        virtual void pushDir( const char* dirName );
        // hkStatisticsCollector interface
        virtual void popDir();

		const hkPointerMap<const hkClass*,hkStatisticClassCount*>& getTypeCount() { return m_typeCount; }

	protected:

		static HK_FORCE_INLINE hkBool _orderCounts(hkStatisticClassCount* a, hkStatisticClassCount* b)
		{
            return a->m_cumulativeUsedBytes > b->m_cumulativeUsedBytes;
		}


        hkBool _isKnown(const void* obj);
        void _addKnown(const void* obj);

		void _startObject( const hkClass* cls, const char* fieldName, const void* obj);
		void _endObject();

        hkStatisticClassCount* _ensureCount(const hkClass* cls);

		void _dumpClassCount();
        void _tab();

        void _addCumulative(int usedSize, int allocatedSize);

            /// Stream to write out to
        hkOstream& m_stream;
            /// The current recursion depth
        int m_depth;

        hkBool m_started;

        void _clear();

            /// Known objects
        hkPointerMap<const void*, int> m_knownObjects;
            /// Amount of times a type is hit
        hkPointerMap<const hkClass*,hkStatisticClassCount*> m_typeCount;

        hkStatisticClassCount m_totalMemory;

        // Stack of of the statistics
        hkArray<hkStatisticClassCount*> m_countStack;
};


#endif // HK_REPORT_STATISTICS_COLLECTOR_H

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
