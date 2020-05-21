/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_SIZE_REPORT_UTIL_H
#define HKBASE_SIZE_REPORT_UTIL_H


#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>
	
	/// The hkSizeReportUtil is a utility class designed to produce reports about block or allocation sizes.
class hkSizeReportUtil
{
    public:
		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::MemorySize MemorySize;
		typedef hkScanReportUtil::ChildMultiMap ChildMultiMap;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::DontFollowMap DontFollowMap;


			/// Display blocks from largest to smallest, along with the path from a 'root block' (a hkReferencedObject type) that
			/// reaches the allocation - if one can be found.
		static void HK_CALL report(const hkTrackerScanSnapshot* scanSnapshot, hkScanReportUtil::FollowFilter* filter, hkOstream& stream);

			/// Display allocation sizes, and number of allocations of size
		static void HK_CALL allocationReport(const hkTrackerScanSnapshot* scanSnapshot, hkOstream& stream);

			/// A report which displays allocation by type and size
		static void HK_CALL typeAugmentedReport(const hkTrackerScanSnapshot* scanSnapshot, hkOstream& stream);
};

#endif // HKBASE_SIZE_REPORT_UTIL_H

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
