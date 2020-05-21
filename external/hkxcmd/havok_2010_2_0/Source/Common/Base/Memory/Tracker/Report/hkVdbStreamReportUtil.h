/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_VDB_STREAM_REPORT_UTIL_H
#define HKBASE_VDB_STREAM_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>

	/// This utility generates a report containing type, callstack and size information.
	/// It also generates a graph describing the relationship between blocks
	///
	/// This utility is intended to be invoked by Visual Debugger client
class hkVdbStreamReportUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkVdbStreamReportUtil);

		typedef hkTrackerScanSnapshot::Block Block;
		typedef hkTrackerScanSnapshot::Allocation Allocation;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::ChildMultiMap ChildMultiMap;
		
			/// Line-oriented succinct report suitable for streaming over a socket
		static void HK_CALL generateReport( const hkTrackerScanSnapshot* scanSnapshot, hkOstream& stream );
};

#endif // HKBASE_VDB_STREAM_REPORT_UTIL_H

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
