/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_TULIP_REPORT_UTIL_H
#define HKBASE_TULIP_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>

	/// Tulip is an open source graph data visualization tool.
	/// This utility's purpose is to take memory scan snapshots and put them in
	/// a form where they can be visualized inside of the tulip application.
	///
	/// Tulip is available for download here:
	/// http://sourceforge.net/projects/auber/
	///
	/// The output of the reports should be sent to '.tlp' files, for loading into tulip.
	/// Note that when initially loading a file none of the blocks are positioned, so you will generally get a
	/// single block in the middle of the screen. To position the blocks you can use 'Algorithm>Layout' menu
	/// option from within tulip and then set a suitable algorithm for the kind of graph the report is using.
	/// For example 'Algorithm/Tree/Bubble Tree' is suitable for tree like graphs - as produced by reportCategorySummary
	/// You'll be more restricted in the type of layouts form 'report' as they are generally not trees.
	///
	/// In the report the size of the allocation is shown by the size of the block or more specifically the volume of the block.
	/// I.e., an allocation of 64 bytes will be shown by a cube 4 by 4 by 4.
class hkTulipReportUtil
{
    public:
		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::MemorySize MemorySize;
		typedef hkScanReportUtil::ChildMultiMap ChildMultiMap;

			/// Reports the raw graph of connectivity held within the snapshot.
			/// In general this means that the graph will not be a tree, but a general graph.
		static void HK_CALL report(hkTrackerScanSnapshot* scanSnapshot, hkOstream& stream);
			/// Uses the hkCategoryReportUtil to categorize blocks.
			/// Normally this produces a forest of trees with all the blocks in each tree belonging to a single category.
			/// Tulip has more facilities for handling a graph which is a single tree.
			/// In order to achieve this - all root blocks, are placed into a node for each category, and each category node
			/// is attached to an overall root node. This makes the graph a single tree of all blocks.
		static void HK_CALL reportCategorySummary(hkTrackerScanSnapshot* scanSnapshot, hkScanReportUtil::FollowFilter* filter, hkOstream& stream);
};

#endif // HKBASE_TULIP_REPORT_UTIL_H

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
