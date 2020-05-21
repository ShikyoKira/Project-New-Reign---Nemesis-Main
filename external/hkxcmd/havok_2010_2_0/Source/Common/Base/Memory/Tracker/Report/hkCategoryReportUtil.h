/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_CATEGORY_REPORT_UTIL_H
#define HKBASE_CATEGORY_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerScanSnapshot.h>

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>
#include <Common/Base/Memory/Tracker/Report/hkTypeSummaryReportUtil.h>

#include <Common/Base/Container/PointerMultiMap/hkPointerMultiMap.h>

class hkRttiTypeCache;

	/// The category report aims to report memory by predefined 'categories'.
	/// The categories - defined by the 'Category' enum in the class hkCategoryReportUtil
	/// are designed to try and give a good division in the usage of memory across the Havok
	/// physics library.
	///
	/// In the actual report - all blocks are determined to belong to a category
	/// and then type summary system of hkScanReportUtil is used to report all of the types/blocks
	/// that belong to a specific category.
class hkCategoryReportUtil
{
    public:
        HK_DECLARE_PLACEMENT_ALLOCATOR();
		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::ChildMultiMap ChildMultiMap;
		typedef hkScanReportUtil::DontFollowMap DontFollowMap;
		typedef hkScanReportUtil::TypeIndexMap TypeIndexMap;
		typedef hkScanReportUtil::MemorySize MemorySize;
		typedef hkScanReportUtil::FollowFilter FollowFilter;
		typedef hkScanReportUtil::NameTypePair NameTypePair;
		
		typedef hkTypeSummaryReportUtil::TypeSummary TypeSummary;

		enum Category
		{
			CATEGORY_WORLD = 0,				///< Objects/blocks reached from the world (excluding hkpRigidBody)
			CATEGORY_CONTACT_MGR,			///< Objects/blocks reached form contact manager/s
			CATEGORY_RIGID_BODY,			///< Rigid bodies
			CATEGORY_COLLISION,				///< hkpAgentXX + hkpCollisionDispatcher
			CATEGORY_PHANTOM,				///< Phantom
			CATEGORY_CONSTRAINT,			///< Constraint instances
			CATEGORY_PACK_FILE_DATA,		///< Packfile and objects reached from it
			CATEGORY_ACTION,				///< Actions

			//CATEGORY_DESTRUCTION_WORLD,		///< Objects/blocks reached from destruction world
			// CATEGORY_PHYSICS,

			CATEGORY_GRAPHICS,				///< Graphics objects/blocks (roots identified by the HKG prefix)
			CATEGORY_DESTRUCTION,
			CATEGORY_AI,
			CATEGORY_ANIMATION,
			CATEGORY_BEHAVIOR,
			CATEGORY_CLOTH,
			CATEGORY_GEOMETRY_PROCESSING,

			CATEGORY_REMAINING,				///< Any remaining blocks - that haven't been put into one of the previous categories
			CATEGORY_UNASSIGNED_REMAINING, 

			CATEGORY_LAST,					///< The last category in the list
		};

			/// Calculate category information
			/// The typeIndexMap associates different types with category enums. This can be used by functions in
			/// the hkScanReportUtil to traverse the scan snapshot.
			/// The parent map is set up with
		static void HK_CALL calcCategories(hkTrackerScanSnapshot* scanSnapshot, FollowFilter* filter, ParentMap& parentMap, TypeIndexMap& typeIndexMap, hkArray<const Block*>& rootBlocks);
			/// Get the category name
		static const char* HK_CALL getCategoryName(Category type);

			/// Returns the type name associated with the category, or HK_NULL if it isn't a 'type' style category
		static const char* HK_CALL getCategoryTypeName(Category type);

			/// Do a report which reports
		static void HK_CALL report(hkTrackerScanSnapshot* scanSnapshot, FollowFilter* filter, hkOstream& stream);

    protected:
};

#endif // HKBASE_CATEGORY_REPORT_UTIL_H

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
