/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_PRODUCT_REPORT_UTIL_H
#define HKBASE_PRODUCT_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>

#include <Common/Base/Memory/Tracker/Report/hkTypeSummaryReportUtil.h>

//class hkRttiTypeCache;

	/// The hkProductReportUtil is a utility class that is designed to report
	/// memory usage patterns found in an hkTrackerScanSnapshot structure.
	///
	/// In order to determine block ownership the facilities of hkScanReportUtil are used. The filter
	/// can be used to control block ownership.
	/// The root blocks are blocks which can have the ownership to a product determined.
class hkProductReportUtil
{
    public:
		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::FollowFilter FollowFilter;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::ChildMultiMap ChildMultiMap;
		typedef hkScanReportUtil::DontFollowMap DontFollowMap;
		typedef hkScanReportUtil::TypeIndexMap TypeIndexMap;

		enum Product
		{
			PRODUCT_AI = 0,
			PRODUCT_ANIMATION,
			PRODUCT_BEHAVIOR,
			PRODUCT_CLOTH,
			PRODUCT_COMMON,
			PRODUCT_DESTRUCTION,
			PRODUCT_PHYSICS,
			PRODUCT_GEOMETRY_PROCESSING,
			PRODUCT_GRAPHICS,
			PRODUCT_OTHER,
			PRODUCT_LAST,					///< Must keep at end - used to iterate over the products
		};

			/// Returns the product type as text
		static const char* HK_CALL getProductName(Product type);

			/// Determine ownership into product categories of blocks
			/// The filter can be used to fine tune block ownership relationships. Passing HK_NULL will have ownership determined by
			/// first to reach it.
			/// The typeIndexMap will be set up such that it maps each type to one of the Product enum values
			/// rootBlocks will be filled in with the roots, and the parent map is set up to store the ownership relationship between blocks
		static void HK_CALL calcCategories(hkTrackerScanSnapshot* scanSnapshot, FollowFilter* filter, ParentMap& parentMap, TypeIndexMap& typeIndexMap, hkArray<const Block*>& rootBlocks);

			/// Report block product ownership information in summary.
			/// The summarizing is performed by finding all blocks belonging to a product category. Then the TypeSummary system of hkScanReportUtil
			/// is used
		static void HK_CALL reportSummary(hkTrackerScanSnapshot* scanSnapshot, FollowFilter* filter, hkOstream& stream);

};

#endif // HKBASE_PRODUCT_REPORT_UTIL_H

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
