/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_STACK_TRACE_REPORT_UTIL_H
#define HKBASE_STACK_TRACE_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>

	/// The hkStackTraceReportUtil is a utility class used to produce reports that use call stack information.
	/// 
	/// For the utility to operate correctly it requires that the hkTrackerScanSnapshot has stack track information.
	/// This is most easy achieved by having a memory system that can produce stack trace information (such
	/// as the hkCheckingMemorySystem) that implements methods to find out for each allocation what the stack trace
	/// was. For the Havok demos - the hkCheckingMemorySystem can be used just by passing -c on the command line
	/// to the demo.
	///
	/// Generally the reports work by associating all allocations taking place from a function address. Thus you
	/// should consider the memory reported as the cumulative amount of memory from all code executed from the specified
	/// function address. Thus for a single threaded application 'main' will include all of the allocated memory of the system -
	/// as all memory must have been allocated underneath main.
class hkStackTraceReportUtil
{
    public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkStackTraceReportUtil);

		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::MemorySize MemorySize;
		typedef hkScanReportUtil::ChildMultiMap ChildMultiMap;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::DontFollowMap DontFollowMap;
		typedef hkScanReportUtil::FollowFilter FollowFilter;

		struct Summary
		{
			hkUlong m_addr;						///< The function address (as can be used by hkStackTracer - to get the function name, line number etc)
			hk_size_t m_totalSize;				///< The total size in bytes of all allocations from the function address
			hk_size_t m_totalNumAllocs;			///< The total number of allocation from the function address
		};

			/// The report aims to report the relationship between all blocks underneath
			/// a function address.
			/// For each function address it works out all of the allocations performed underneath it. Then
			/// it tries to find the types, and ownership of blocks from those types for those blocks.
			/// The determining of the relationship between blocks is performed by the hkScanReportUtil. The
			/// filter can be used to control determining ownership - just pass HK_NULL if ownership if just
			/// determined by first reaching a block.
		static void HK_CALL report(const hkTrackerScanSnapshot* scanSnapshot, FollowFilter* filter, hkOstream& stream);

			/// Calculate the for each call stack address, a summary of total memory usage, and number of allocations
			/// The summaries are returned in the largest to smallest size.
		static void HK_CALL calcSummaries(const hkTrackerScanSnapshot* scanSnapshot, hkArray<Summary>& summaries);

			/// Report the basic summary information from calcSummaries - that is the amount of memory, and amount
			/// of allocations called from a function address.
		static void HK_CALL reportSummary(const hkTrackerScanSnapshot* scanSnapshot, hkOstream& stream);

};

#endif // HKBASE_STACK_TRACE_REPORT_UTIL_H

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
