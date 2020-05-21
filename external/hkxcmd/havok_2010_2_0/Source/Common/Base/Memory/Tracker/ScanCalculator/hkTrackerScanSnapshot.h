/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_SCAN_SNAPSHOT_H
#define HKBASE_SCAN_SNAPSHOT_H

#include <Common/Base/Reflection/TypeTree/hkTrackerTypeTreeCache.h>

#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerSnapshot.h>
#include <Common/Base/Memory/Tracker/LayoutCalculator/hkTrackerLayoutCalculator.h>

#include <Common/Base/Container/StringMap/hkStorageStringMap.h>

class hkTrackerSerializableScanSnapshot;

	/// The hkTrackerScanSnapshot is a structure representing the usage of memory at a particular instance.
	///
	/// A hkTrackerScanSnapshot can be produced by providing a hkTrackerSnapshot to the hkTrackerScanCalculator. Once the hkTrackerScanSnapshot
	/// has been produced, the actual underlying objects that the representation has been created from can change, and be destroyed
	/// without affecting the hkTrackerScanSnapshot.
	///
	/// The hkTrackerScanSnapshot can be thought of as a 'database' describing the inter-relationships and types of all the allocations at the
	/// time the hkTrackerSnapshot was taken - and the database is independent of the existence of the objects.
	/// 
	/// The snapshot consists of two basic types 'Allocation's and 'Block's. An Allocation is
	/// an allocation that has been made - usually these are determined via performing a memory
	/// walk on the hkMemorySystem. Allocations can be thought of the low level description of underlying
	/// memory allocations. Memory allocations cannot overlap, and must be unique.
	///
	/// A 'Block' is a more high level concept. It describes how a particular piece of memory is being used in the form
	/// of its type. Additionally a Block also stores how it 'references' other Blocks.
	/// A reference in this context generally means that it contains a pointer to another Block. A block also stores
	/// the Allocation it belongs to. A Block must in its entirety fit into a single Allocation.
	/// 
	/// For example if an object is 'new'd there will be an Allocation describing the underlying memory
	/// that is holding the object, and there will be Block which will describe that a piece of the Allocation (typically all of it) that
	/// holds an object of the type.
	/// An Allocation that holds an object may not be the same size as the Block - because the allocator may assign more space than the object needs
	/// for example.
	/// 
	/// Extending the idea - take an hkArray<int> of capacity 16, and holding 2 elements. The allocation for the storage for the array
	/// will be at least 16 * sizeof(int) bytes. The block will start at the start of the allocation and be 2 * sizeof(int) bytes in size. It will
	/// also have the type 'int[2]' associated with it.
	///
	/// The data held inside of hkScanSnaphsot is arranged so that finding out information about blocks and allocations is reasonably fast.
	/// For example all Allocations are held in address order, so finding if an address belongs to a particular allocation can
	/// be found fairly quickly with a binary chop.
	///
	/// Sometimes its useful to know where an Allocation originated from. This can be determined from an Allocation from the m_traceId.
	/// The traceId can be applied to the callTree to determine the call stack that produced the Allocation. This can often be
	/// used to determine when a Block was created - by finding the associated Allocation. If the hkCheckingMemorySystem is used
	/// (this can be switched on in the Havok demos with the -c option) allocations will have stack trace ids associated with them. The
	/// hkTrackerScanCalculator requires a hkMemorySystem which implements the stack tracing functions for the hkTrackerScanCalculator to store the
	/// trace int he hkTrackerScanCalculator.
	/// 
	/// The stack trace can be turned back into readable text using the hkStackTracer.
class hkTrackerScanSnapshot: public hkReferencedObject
{
	friend class hkTrackerSerializableScanSnapshot;
	public:
		HK_DECLARE_CLASS_ALLOCATOR(hkTrackerScanSnapshot);


		typedef hkTrackerTypeTreeNode Node;
		typedef hkTrackerSnapshot::Allocation Allocation;

		struct Block
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ENGINE, Block);
			enum
			{
				INVALID_USED_SIZE = hkTrackerLayoutHandler::ReferenceInfo::INVALID_USED_SIZE
			};

				/// Returns true if is named type
			hkBool isNamedType(const char* name) const
			{
				return m_type && m_type->isNamedType() && m_type->m_name == name;
			}

				/// Start
			Block(const Node* type, void* start, hk_size_t size, const Allocation* alloc = HK_NULL):
				m_type(type),
				m_start((const hkUint8*)start),
				m_size(size),				///< Total size in bytes. If INVALID_USED_SIZE, means the size is currently undefined.
				m_arraySize(-1),
				m_startReferenceIndex(0),
				m_numReferences(0),
				m_allocation(alloc)
			{}

			const Node* m_type;				///< The type
			const hkUint8* m_start;			///< The start address
			hk_size_t m_size;				///< The size in bytes

			int m_arraySize;				///< -1 if not an array, else number of members

			int m_startReferenceIndex;		///< The start index in 'references' for references for this block
			int m_numReferences;			///< The number of references

			const Allocation* m_allocation;		///< The allocation this belongs to (HK_NULL if none)
		};

			/// Add a block
		Block* addBlock(const Node* type, void* start, hk_size_t size, const Allocation* alloc = HK_NULL);
			/// Get a block by a start address
		Block* getBlock(void* start) const { return m_blockMap.getWithDefault(start, HK_NULL); }
			/// Find a block - does some pointer managing
		Block* findBlock(void* ptr) const;

			/// Clear
		void clear();

			/// Order the blocks in pointer start
		void orderBlocks();

			/// Calculates the total memory used - i.e., memory which has blocks.
			/// This is tricky - because a block could be embedded inside another block.
			/// Method assumes the blocks are ordered
		hk_size_t calcTotalUsed() const;

			/// Determines if a pointer is likely a pointer (either to a block, or to an allocation)
		hkBool isPointer(void* ptr) const;

			/// Adds all of the blocks with the type name to blocksOut list
		void findBlocksByType(const char* name, hkArray<Block*>& blocksOut) const;

			/// Get the layout calculator
		hkTrackerLayoutCalculator* getLayoutCalculator() const { return m_layoutCalc; }
			/// Get the blocks
		const hkArray<Block*>& getBlocks() const { return m_blocks; }
			/// Get the tracker snapshot
		//const hkTrackerSnapshot* getTrackerSnapshot() const { return m_snapshot; }
			/// Get the allocations
		const hkArray<Allocation>& getAllocations() const { return m_allocations; }

			/// Get all of the references
		hkArray<Block*>& getReferences() { return m_references; }
			/// Get all of the references
		const hkArray<Block*>& getReferences() const { return m_references; }

			/// Find an allocation
		const Allocation* findAllocation(const void* ptr) const;

			/// Calculate the total allocated (by summing up all Allocation sizes)
		hk_size_t calcTotalAllocated() const;

			/// Get the start of the list of references from a block
		inline Block*const* getBlockReferences(const Block* block) const; 

			/// Finds the index of refdBlock in the list of blocks from block. Returns -1 if not found
		int findReferenceIndex(const Block* block, const Block* refdBlock) const;
	
			/// Returns true if has trace text
		hkBool hasTraceText() const { return m_traceText.getSize() > 0; }
			/// Get some trace text
		const char* getTraceText(hkUlong addr) const; 
			/// Set trace text associated with an address. Cannot set text on an already set address.
		void setTraceText(hkUlong addr, const char* text);

			/// Get the call tree. Can be used with the m_traceId, to get a call stack for each allocation.
		const hkStackTracer::CallTree& getCallTree() const { return m_callTree; }
			/// Get the call tree. Can be used with the m_traceId, to get a call stack for each allocation.
		hkStackTracer::CallTree& getCallTree() { return m_callTree; }

			/// Performs checks to find if the snapshot appears valid
		hkResult checkConsistency() const;

			/// Find all the blocks that belong to an allocation
		void findAllocationBlocks(const Allocation* alloc, hkArray<Block*>& blocksOut);

			/// Ctor
		hkTrackerScanSnapshot(const hkTrackerSnapshot* snapshot, hkTrackerLayoutCalculator* layoutCalc);
			/// Default ctor
		hkTrackerScanSnapshot();

			/// Dtor
		~hkTrackerScanSnapshot();

			/// Calculate the total memory used from an unordered set of blocks
		static hk_size_t HK_CALL calcTotalUsed(const Block*const* blocksIn, int numBlocks);
			/// Calculate the used memory of a set of ordered blocks
		static hk_size_t HK_CALL calcOrderedTotalUsed(const Block*const* blocks, int numBlocks);

			/// Order blocks, the order required that calcOrderedTotalUsed
		static void HK_CALL orderBlocks(const Block** blocksIn, int numBlocks);

	protected:
		hkArray<Block*> m_blocks;
		hkPointerMap<void*, Block*> m_blockMap;
		hkArray<Allocation> m_allocations;
		hkArray<Block*> m_references;						///< The references from blocks (section is listed in the block)
		hkStackTracer::CallTree m_callTree;					///< Call tree

		hkFreeList m_blockFreeList;

		hkRefPtr<hkTrackerLayoutCalculator> m_layoutCalc;

		hkPointerMap<hkUlong, int> m_traceMap;				///< Maps an address from the trace to an index to a piece of text in the m_traceText
		hkArray<char> m_traceText;							///< Holds all of the trace text
		hkArray<char> m_typeText;							///< Holds type text
};

// ---------------------------------------------------------------------------

hkTrackerScanSnapshot::Block*const* hkTrackerScanSnapshot::getBlockReferences(const Block* block) const
{ 
	HK_ASSERT(0x32432432, block->m_startReferenceIndex >= 0 && block->m_startReferenceIndex <= m_references.getSize()); 
	return m_references.begin() + block->m_startReferenceIndex; 
}

#endif // HKBASE_SCAN_SNAPSHOT_H

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
