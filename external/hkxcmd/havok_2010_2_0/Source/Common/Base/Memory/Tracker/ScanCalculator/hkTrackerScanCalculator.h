/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_SCAN_CALCULATOR_H
#define HKBASE_SCAN_CALCULATOR_H


#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerSnapshot.h>
#include <Common/Base/Memory/Tracker/LayoutCalculator/hkTrackerLayoutCalculator.h>

#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerScanSnapshot.h>

#include <Common/Base/Container/StringMap/hkStorageStringMap.h>

class hkTrackerSnapshot;

class hkTrackerScanCalculator
{
    public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ENGINE, hkTrackerScanCalculator);

		typedef hkTrackerTypeTreeNode RttiNode;
		typedef hkTrackerTypeTreeNode Node;

		typedef hkTrackerSnapshot::Allocation Allocation;
		typedef hkTrackerScanSnapshot::Block Block;
		
			/// 
		hkTrackerScanSnapshot* createSnapshot(const hkTrackerSnapshot* snapshot, hkTrackerLayoutCalculator* layoutCalc);		

			/// Returns true if the type will be scanned uniquely (i.e., its layout effectively changes)
		hkBool isFullScanType(const RttiNode* type) const;

	protected:

		hkBool _isVirtual(const RttiNode* type) const { return m_virtualTypeMap.getWithDefault(type, 0) != 0; }

		struct Unknown
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,Unknown);
			const RttiNode* m_type;			///< The type
			const hkUint8* m_start;			///< The start address
			hk_size_t m_size;				///< The size in bytes
			int m_numEntries;				///< Num entries of this type
			int m_stride;					///< The bytes between member entries if this is an 'array'
		};
		//HK_ACCESS_CHILD_TYPE(Unknown);

		HK_FORCE_INLINE static hkBool HK_CALL _orderUnknownByType(const Unknown& a, const Unknown& b)
		{
			return a.m_type < b.m_type;
		}
		//void _checkBlocks(const hkTrackerSnapshot* snapshot);
		
		hkTrackerTypeLayout* _calcScanLayout(const RttiNode* type, hk_size_t size, const Unknown* unknowns, int numUnknowns);
		
		
		hkTrackerScanSnapshot* _createSnapshot(const hkTrackerSnapshot* snapshot, hkTrackerLayoutCalculator* layoutCalc);

		void _addBlockReferences(const hkUint8* start, const hkArray<hkTrackerTypeLayout::Member>& members, hkArray<Block*>& references);
		void _findBlockLayoutReferences(Block* block, hkArray<Block*>& references);
		void _scanReferences(const hkUint8* start, hk_size_t size, hkArray<Block*>& references);
		void _scanBlockReferences(Block* block, hkArray<Block*>& references);
		void _findBlockReferences(Block* block, hkArray<Block*>& references);
		void _findBlockReferences(Block* block);
		void _addRefInfos(const hkArray<hkTrackerLayoutHandler::ReferenceInfo>& refInfos);

		hkTrackerScanSnapshot* m_scanSnapshot;

		hkPointerMap<const RttiNode*, int> m_virtualTypeMap;
		
};

#endif // HKBASE_SCAN_CALCULATOR_H

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
