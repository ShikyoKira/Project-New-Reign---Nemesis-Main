/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_SERIALIZABLE_SCAN_SNAPSHOT_H
#define HKBASE_SERIALIZABLE_SCAN_SNAPSHOT_H

#include <Common/Base/Memory/Tracker/Default/hkDefaultMemoryTracker.h>

#include <Common/Base/Algorithm/Sort/hkSort.h>

#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerScanSnapshot.h>

extern const class hkClass hkTrackerSerializableScanSnapshotBlockClass;

extern const class hkClass hkTrackerSerializableScanSnapshotAllocationClass;

struct hkSubString;
class hkMemorySystem;
class hkTrackerScanSnapshot;

extern const hkClass hkTrackerSerializableScanSnapshotClass;

class hkTrackerSerializableScanSnapshot: public hkReferencedObject
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		typedef hkTrackerScanSnapshot::Block ScanBlock;
		typedef hkTrackerTypeTreeNode RttiNode;
		typedef hkPointerMap<const RttiNode*, int> TypeIndexMap;

		struct Allocation
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, Allocation);
			HK_DECLARE_REFLECTION();
				/// Default Ctor
			Allocation() {}
				/// Serialize Ctor
			Allocation(hkFinishLoadedObjectFlag f) {}

				/// ==
			hkBool operator==(const Allocation& rhs) const { return m_start == rhs.m_start && m_size == rhs.m_size && m_traceId == rhs.m_traceId; }
				/// !=
			hkBool operator!=(const Allocation& rhs) const { return !(*this == rhs); }

			hkUlong m_start;					///< Start 'address'
			hkUlong m_size;					///< Size in bytes
			int m_traceId;						///< Stack trace id
		};
 
		struct Block
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, Block);
			HK_DECLARE_REFLECTION();

				/// Copy Ctor
			Block(const Block& rhs):
				m_typeIndex(rhs.m_typeIndex),
				m_start(rhs.m_start),
				m_size(rhs.m_size),
				m_arraySize(rhs.m_arraySize),
				m_startReferenceIndex(rhs.m_startReferenceIndex),
				m_numReferences(rhs.m_numReferences)
			{
			}
				
				/// ==
			hkBool operator==(const Block& rhs) const;
				/// !=
			hkBool operator!=(const Block& rhs) const { return !(*this == rhs); }

				/// Serialize Ctor
			Block(hkFinishLoadedObjectFlag f) {}
				/// Default Ctor
			Block() {}
 
			int m_typeIndex;
			hkUlong m_start;					///< The start address
			hkUlong m_size;					///< The size in bytes
			int m_arraySize;					///< -1 if not an array, else number of members

			int m_startReferenceIndex;
			int m_numReferences;
		};

			/// ==
		hkBool operator==(const hkTrackerSerializableScanSnapshot& rhs) const;
			/// !=
		hkBool operator!=(const hkTrackerSerializableScanSnapshot& rhs) const { return !(*this == rhs); }

		void load(const hkTrackerScanSnapshot& snapshot);
		
		void store(hkTrackerLayoutCalculator* layoutCalc, hkTrackerScanSnapshot& snapshot);

			// Clear the contents
		void clear();

			/// Get a trace
		void getTrace(int id, hkArray<hkUlong>& trace) const;

			/// Default ctor
		hkTrackerSerializableScanSnapshot();
			/// Serialize ctor
		hkTrackerSerializableScanSnapshot(hkFinishLoadedObjectFlag f);

			/// Dtor
		~hkTrackerSerializableScanSnapshot();
	protected:
		void _initTypeIndexMap(const hkArray<ScanBlock*>& blocks, TypeIndexMap& map);
		void _addTraceText(const char* text);

		hkArray<Allocation> m_allocations;
		hkArray<Block> m_blocks;
		hkArray<int> m_refs;

		hkArray<hkUint8> m_typeNames;

		// Information for traces
		hkArray<hkUint8> m_traceText;			///< Holds all of the trace text information. In the order in m_traceAddrs
		hkArray<hkUint64> m_traceAddrs;			///< The addrs
		hkArray<int> m_traceParents;				///< The parent to each address
};

#endif // HKBASE_SERIALIZABLE_SCAN_SNAPSHOT_H

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
