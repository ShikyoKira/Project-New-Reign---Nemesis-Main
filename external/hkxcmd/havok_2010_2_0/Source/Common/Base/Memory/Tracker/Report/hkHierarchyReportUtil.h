/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HIERARCHY_REPORT_UTIL_H
#define HKBASE_HIERARCHY_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerScanSnapshot.h>

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>

#include <Common/Base/Container/PointerMultiMap/hkPointerMultiMap.h>

class hkRttiTypeCache;

	/// hkHierarchyReportUtil is utility class for displaying reports from a hkTrackerScanSnapshot
	///
	/// The reports produced are from a given root block - a depth first search will search out the blocks that
	/// can be reached from the root. An optional filter (pass HK_NULL if filtering is not required) can be used to control
	/// reachability from one block to another.
	///
	/// The report summarizes the memory usage along a particular path from the root.
	/// The size is the size of all blocks at that path. The total is the total memory for blocks at that depth, and any at a lower depth
	/// along the path.
class hkHierarchyReportUtil
{
    public:
		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::MemorySize MemorySize;
		typedef hkScanReportUtil::FollowFilter FollowFilter;

		static void HK_CALL report(hkTrackerScanSnapshot* scanSnapshot, hkBool asCsv, const Block* rootBlock, FollowFilter* filter, hkOstream& stream);
};

#endif // HKBASE_HIERARCHY_REPORT_UTIL_H

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
