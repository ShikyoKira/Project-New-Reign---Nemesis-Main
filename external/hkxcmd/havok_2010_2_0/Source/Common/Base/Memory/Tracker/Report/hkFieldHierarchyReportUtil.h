/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_FIELD_HIERARCHY_REPORT_UTIL_H
#define HKBASE_FIELD_HIERARCHY_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>

	/// The hkFieldHierarchyReportUtil that produces a text report from a hkTrackerScanSnapshot.
	///
	/// This report shows the fields taken in each class that reach the different blocks owned.
	/// Ownership is determined by following the references between blocks from the root block passed
	/// to the report method. Ownership can be controlled using the FollowFilter.
class hkFieldHierarchyReportUtil
{
	public:
		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::TypeIndexMap TypeIndexMap;
		typedef hkScanReportUtil::MemorySize MemorySize;
		typedef hkScanReportUtil::FollowFilter FollowFilter;

			/// Produces a text report written to stream of blocks found from the rootBlock.
			/// The filter parameter can be used to control ownership - passing as HK_NULL means all blocks that can be reached
			/// will be owned.
		static void HK_CALL report(hkTrackerScanSnapshot* scanSnapshot, const Block* rootBlock, FollowFilter* filter, hkOstream& stream);

    protected:
};

#endif // HKBASE_FIELD_HIERARCHY_REPORT_UTIL_H

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
