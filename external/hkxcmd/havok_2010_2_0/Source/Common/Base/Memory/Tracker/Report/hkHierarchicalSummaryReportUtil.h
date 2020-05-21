/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HIERARCHICAL_SUMMARY_REPORT_UTIL_H
#define HKBASE_HIERARCHICAL_SUMMARY_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerScanSnapshot.h>

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>
#include <Common/Base/Memory/Tracker/Report/hkTypeSummaryReportUtil.h>

#include <Common/Base/Container/PointerMultiMap/hkPointerMultiMap.h>

class hkRttiTypeCache;

	/// The hierarchical summary report is designed around a hierarchy of ownership of blocks.
	/// This hierarchy is stored in a parentMap (and associated childMultiMap).
	/// The relationship is basically a tree - unfortunately the trees of ownership can be very deep (for example
	/// a linked list can be seen as a type of 'tree' with an arbitrary depth).
	/// To get around the depth problem the report is provided with a maximum depth - any blocks beneath
	/// this depth will be reported in summary.
	///
	/// The hierarchy/ownership relationship is displayed in the report as an indentation.
class hkHierarchicalSummaryReportUtil
{
    public:
		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::ChildMultiMap ChildMultiMap;
		typedef hkScanReportUtil::DontFollowMap DontFollowMap;
		typedef hkScanReportUtil::TypeIndexMap TypeIndexMap;
		typedef hkScanReportUtil::MemorySize MemorySize;
		typedef hkScanReportUtil::FollowFilter FollowFilter;

		typedef hkTypeSummaryReportUtil::TypeSummary TypeSummary;

			/// 
		static void HK_CALL report(hkTrackerScanSnapshot* scanSnapshot, int maxDepth, const Block* rootBlock, FollowFilter* filter, hkOstream& stream);
			/// Report summary when ownership has been determined
		static void HK_CALL report(hkTrackerScanSnapshot* scanSnapshot, int maxDepth, const hkArray<const Block*>& rootBlocks, const ParentMap& parentMap, const ChildMultiMap& childMultiMap, hkOstream& stream);

    protected:
};

#endif // HKBASE_HIERARCHICAL_SUMMARY_REPORT_UTIL_H

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
