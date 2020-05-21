/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_TYPE_SUMMARY_REPORT_UTIL_H
#define HKBASE_TYPE_SUMMARY_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/Report/hkScanReportUtil.h>

class hkRttiTypeCache;

class hkTypeSummaryReportUtil
{
    public:
    	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkTypeSummaryReportUtil);
		typedef hkScanReportUtil::Block Block;
		typedef hkScanReportUtil::Allocation Allocation;
		typedef hkScanReportUtil::RttiNode RttiNode;
		typedef hkScanReportUtil::ParentMap ParentMap;
		typedef hkScanReportUtil::ChildMultiMap ChildMultiMap;
		typedef hkScanReportUtil::DontFollowMap DontFollowMap;
		typedef hkScanReportUtil::TypeIndexMap TypeIndexMap;
		typedef hkScanReportUtil::MemorySize MemorySize;
		typedef hkScanReportUtil::FollowFilter FollowFilter;

			/// The TypeSummary class is used for summarizing information about a type -
			/// such as the amount of memory allocated to that type, and its owned types
			/// and the amount of instances of a type.
		struct TypeSummary
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,TypeSummary);
			const RttiNode* m_type;		///< The type
			int m_numInstances;			///< The number of instances
			int m_numBlocks;			///< Total num blocks
			hk_size_t m_totalSize;		///< Includes memory blocks owned by the type
			hk_size_t m_totalInstanceSize;	///< Memory used on instances
		};

			/// Dump summaries
		static void dumpSummaries(const hkArray<TypeSummary>& summaries, hkOstream& stream);

			/// Calc summaries about the types found
		static void HK_CALL calcTypeSummaries(const hkArray<const Block*>& children, const ParentMap& parentMap, hkArray<TypeSummary>& summaries);
			/// Total up the summaries and put result in summaryOut
		static void HK_CALL calcTotalSummary(const TypeSummary* summaries, int numSummaries, TypeSummary& summaryOut);

			/// Calculates type summaries - but doesn't take into account ownership
		static void HK_CALL calcTypeSummaries(const hkArray<const Block*>& blocks, hkArray<TypeSummary>& summaries);

			/// Report summaries from root objects which are hkReferencedObjects
		static void HK_CALL report(hkTrackerScanSnapshot* scanSnapshot, FollowFilter* filter, hkOstream& stream);


			/// Find the index of a type summary by type
		static int HK_CALL findSummaryByType(const hkArray<TypeSummary>& summaries, const RttiNode* type);
			/// Order summaries by name
		static void HK_CALL orderTypeSummariesByName(TypeSummary* summaries, int numSummaries);
		/// Order summaries by size (largest to smallest size)
		static void HK_CALL orderTypeSummariesBySize(TypeSummary* summaries, int numSummaries);

    protected:
};

#endif // HKBASE_TYPE_SUMMARY_REPORT_UTIL_H

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
