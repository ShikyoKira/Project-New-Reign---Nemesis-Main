/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_SCAN_REPORT_UTIL_H
#define HKBASE_SCAN_REPORT_UTIL_H

#include <Common/Base/Memory/Tracker/ScanCalculator/hkTrackerScanSnapshot.h>

#include <Common/Base/Container/PointerMultiMap/hkPointerMultiMap.h>

class hkTrackerTypeTreeCache;

	/// The hkScanReportUtil class provides a set of functionality useful to producing
	/// reports from hkTrackerScanSnapshot objects.
	/// 
	/// One of the overriding issues to do with memory reporting has to do with ownership.
	/// The hkTrackerScanSnapshot contains 'Blocks' of memory - and the snapshot also contains
	/// information about the connection of one block to another through 'references'. These
	/// references are determined by the hkTrackerScanCalculator - and a Block will have a connection
	/// to another block if it contains a pointer to it for example.
	/// What this means is that the hkTrackerScanSnapshot is made up of Blocks and these are connected
	/// together in a graph. How the graph is connected is basically arbitrary - other than a
	/// pointer from one block from the other is found.
	///
	/// Generally though we think of one object 'owning' another object. It may be that an object
	/// is shared - meaning one object is owned by more than one other object.
	/// In order to simplify reporting its often useful to deal with Blocks as if they can only
	/// be owned by another Block.
	///
	/// In order to determine ownership many of the functions of hkScanReportUtil do breadth first
	/// traversal from roots. If a block is reached by a traversal, and does not belong to another
	/// Block, it is owned by the Block it was reached from.
	/// When doing the traversal its necessary to have blocks to start the traversal from - described
	/// in this functionality as root blocks. In some methods you can directly specify a root block/s
	/// to perform traversal from.
	/// 
	/// It's often useful to use type allocations as the root blocks for traversal. This makes ownership
	/// fairly fine grained, but the fine granularity generally avoids problems with sharing, as sharing
	/// often takes place at the level of objects.
class hkScanReportUtil
{
    public:
    	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkScanReportUtil);
		typedef hkTrackerScanSnapshot::Block Block;
		typedef hkTrackerSnapshot::Allocation Allocation;
		typedef hkTrackerTypeTreeNode RttiNode;
		typedef hkTrackerTypeTreeNode Node;
		typedef hkPointerMap<const Block*, const Block*> ParentMap;
		typedef hkPointerMultiMap<const Block*, const Block*> ChildMultiMap;
		typedef hkPointerMap<const Block*, int> DontFollowMap;
		typedef hkPointerMap<const RttiNode*, int> TypeIndexMap;
	
		enum Traversal
		{
			TRAVERSAL_DEPTH_FIRST,
			TRAVERSAL_BREADTH_FIRST,
		};

			/// MemorySize struct class can be streamed into an hkOstream, and will perform
			/// formatting of a size to be displayed nicely as a memory size.
			/// For example the size 1024 will be displayed as 1 Kb
			/// The struct also has flags that can perform more fine grained control on the formatting
			/// of the display of a size.
		struct MemorySize
		{
			// Tail formats
			// Bytes
			// .0 Kb
			//  Kb
			// .00 Mb
			enum 
			{
				MAX_FULL_DIGITS = 10,						///< Maximum full digits
				TAIL_SIZE = 3 + 1 + 2,						///< The amount of characters after full digits
				TOTAL_SIZE = MAX_FULL_DIGITS + TAIL_SIZE,
			};
			enum Flag
			{
				FLAG_PAD_RIGHT = 0x1,	///< If set the right hand side will be padded so the digits line up
				FLAG_PAD_LEFT = 0x2,	///< If set the left had side will be padded, such the num chars is constant to the last digit
				FLAG_RAW = 0x4,			///< Not pretty printed, just the raw size
			};
			enum
			{
				FORMAT_PAD_RAW = FLAG_RAW | FLAG_PAD_LEFT | FLAG_PAD_RIGHT,
				FORMAT_PAD_PRETTY = FLAG_PAD_LEFT | FLAG_PAD_RIGHT,
			};
			MemorySize(hk_size_t size, int flags = 0):m_size(size), m_flags(flags) {}
			hk_size_t m_size;
			int m_flags;
		};

		
		struct NameTypePair
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkScanReportUtil::NameTypePair);
			const char* m_name;
			int m_typeIndex;
		};

			/// The FollowFilter interface can be used to control ownership relationships between types.
			/// To use derive your class from FollowFilter, and implement the shouldFollow method
			/// then pass a pointer to your implementation to one of the methods used to determine ownership.
			/// A call to the method should determine with the childBlock should be owned by the block.
			/// if shouldFollow returns false, then the childBlock won't be owned by Block along the current
			/// connection.
		class FollowFilter
		{
			public:
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkScanReportUtil::FollowFilter);
				virtual ~FollowFilter() {}
				typedef hkTrackerScanSnapshot::Block Block;
				typedef hkTrackerTypeTreeNode RttiNode;

				virtual hkBool shouldFollow(const Block* block, const Block* childBlock, hkTrackerLayoutCalculator* layoutCalc, const hkTrackerTypeLayout* layout, int memberIndex) = 0;
		};

			/// Find all of the children of block (as defined by a child map)
		static void HK_CALL findChildren(const Block* block, const ChildMultiMap& childMap, hkArray<const Block*>& children);
			/// Append the children of block recursively
		static void HK_CALL appendChildren(const Block* block, const ChildMultiMap& childMap, hkArray<const Block*>& children);

			/// Calculate a multimap which stores all of the children for each block (i.e., the opposite of a parent map)
		static void HK_CALL calcChildMap(const ParentMap& parentMap, ChildMultiMap& childMap);

			/// Find the ownership - by traversing children. The filter can be used to stop traversals.
		static void HK_CALL calcParentMap(const hkTrackerScanSnapshot* scanSnapshot, const Block* rootBlock, FollowFilter* filter, ParentMap& parentMap, Traversal traversal = TRAVERSAL_BREADTH_FIRST);

			/// Append depth first traversal from the root block to other blocks onto the parentMap
		static void HK_CALL appendDepthFirstParentMap(const hkTrackerScanSnapshot* scanSnapshot, const Block* rootBlock, DontFollowMap* dontFollowMap, FollowFilter* filter, ParentMap& parentMap);
			/// Append breadth first traversal from the root blocks to other blocks onto the parentMap
		static void HK_CALL appendBreadthFirstParentMap(const hkTrackerScanSnapshot* scanSnapshot, const Block* rootBlock, DontFollowMap* dontFollowMap, FollowFilter* filter, ParentMap& parentMap);

			/// Appends parent mappings as a traversal is performed. Traversal to a child can be stopped via a dontFollowMap or a filter.
			/// The filter or dontFollowMap can be HK_NULL is filtering isn't required.
		static void HK_CALL appendParentMap(const hkTrackerScanSnapshot* scanSnapshot, const Block* rootBlock, DontFollowMap* dontFollowMap, FollowFilter* filter, ParentMap& parentMap, Traversal traversal = TRAVERSAL_BREADTH_FIRST);
			/// Append parent mappings, from the specified root blocks. Additionally once a traversal is made, the blocks found will be added to the don't follow map.
		static void HK_CALL appendParentAndDontFollowMap(const hkTrackerScanSnapshot* scanSnapshot, const hkArray<const Block*>& rootBlocks, DontFollowMap& dontFollowMap, FollowFilter* filter, ParentMap& parentMap);

			/// For printing memory sizes
		friend hkOstream& HK_CALL operator<<(hkOstream& stream, const hkScanReportUtil::MemorySize& size);
			/// Convert size into text
		static void HK_CALL memorySizeToText(hk_size_t size, int flags, hkStringBuf& string);

			/// Dump call stack for an allocation pointer was allocated
		static void HK_CALL dumpAllocationCallStack(hkStackTracer* tracer, hkTrackerScanSnapshot* scanSnapshot, void* ptr, hkOstream& stream);
			/// Write out trace information
		static void HK_CALL writeTraceText(hkStackTracer* tracer, hkTrackerScanSnapshot* snapshot, hkUlong addr, hkOstream& stream);
			/// Write the call stack specified from the traceId. Tracer can be HK_NULL if the scan snapshot contains trace text.
		static void HK_CALL dumpCallStack(hkStackTracer* tracer, hkTrackerScanSnapshot* scanSnapshot, hkStackTracer::CallTree::TraceId traceId, hkOstream& stream);

			/// Append spaces to stream
		static void HK_CALL appendSpaces(hkOstream& stream, int size);
			/// Align to the right
		static void HK_CALL alignRight(hkOstream& stream, const hkSubString& text, int totalSize);
			/// Align to the left
		static void HK_CALL alignLeft(hkOstream& stream, const hkSubString& text, int totalSize);

			/// Get the member index. -1 if there isn't one
		static int HK_CALL calcMemberIndex(const hkTrackerScanSnapshot* scanSnapshot, const hkTrackerTypeLayout* layout, const Block* block, const Block* childBlock);
			/// Get the member name (HK_NULL if not found)
		static const char* HK_CALL calcMemberName(const hkTrackerScanSnapshot* scanSnapshot, const Block* block, const Block* childBlock);

			/// Find all of the hkReferencedObject derived classes in the snapshot - add them to dontFollowMap, and to the rootBlocks array
		static void HK_CALL calcReferencedObjectRootBlocks(const hkTrackerScanSnapshot* scanSnapshot, DontFollowMap& dontFollowMap, hkArray<const Block*>& rootBlocks);
			/// Calculates 'root' blocks of the blocks specified. A root block is one which is not contained inside another.
		static void HK_CALL calcRootBlocks(const hkArray<const Block*>& blocks, hkArray<const Block*>& rootBlocks);
			/// Uses hkReferencedObject derived classes as roots. Any remaining blocks which have a named type will be subsequent roots.
		static void HK_CALL calcTypeRootBlocks(const hkTrackerScanSnapshot* scanSnapshot, DontFollowMap& dontFollowMap, FollowFilter* filter, ParentMap& parentMap, hkArray<const Block*>& rootBlocks);

			/// Gets (or creates) a type as defined in a TypeDefinition
		static const RttiNode* getTypeTreeNode(const hkMemoryTracker::TypeDefinition* typeDef, hkTrackerTypeTreeCache* typeCache);

			/// Total up the sizes in all of the blocks
		static hk_size_t HK_CALL calcTotalSize(const hkArray<const Block*>& blocks);
			/// Total up the sizes of all of the allocations
		static hk_size_t HK_CALL calcTotalAllocatedSize(const hkArray<const Allocation*>& allocs);

			/// Calculate the total amount of memory allocated to hold the blocks
		static hk_size_t HK_CALL calcTotalAllocatedSize(const hkTrackerScanSnapshot& snapshot, const hkArray<const Block*>& blocks);
			/// Calculate all of the blocks referenced from the parent map
		static void HK_CALL calcParentMapBlocks(const ParentMap& parentMap, hkArray<const Block*>& blocks); 

			/// Find all of the allocations referenced from the blocks
		static void HK_CALL findUsedAllocations(const hkTrackerScanSnapshot& snapshot, const hkArray<const Block*>& blocks, hkArray<const Allocation*>& allocs);

			/// Find the derived types from clsDef and set them to the first type that is reached which as a typeIndex defined in the typeMap
		static void HK_CALL findDerivedTypes(hkMemoryTracker& memoryTracker, const hkMemoryTracker::ClassDefinition* clsDef, hkTrackerTypeTreeCache* typeCache, TypeIndexMap& typeIndexMap);
			/// Find the typeIndices for all types registered with the memoryTracker by finding the first type that has a typeIndex set in the typeMap
		static void HK_CALL findAllDerivedTypes(hkMemoryTracker& memoryTracker, hkTrackerTypeTreeCache* typeCache, TypeIndexMap& typeIndexMap);
			/// Add the types specified by the pairs into the type map
		static void HK_CALL addTypes(const hkTrackerScanSnapshot* scanSnapshot, const NameTypePair* pairs, int numPairs, TypeIndexMap& typeIndexMap);

			/// Appends all of the blocks that have the specified type indices
		static void HK_CALL appendBlocksWithTypeIndices(const hkTrackerScanSnapshot* scanSnapshot, const TypeIndexMap& typeIndexMap, const int* typeIndices, int numTypeIndices, hkArray<const Block*>& blocksOut);
			/// Append all of the blocks that have the specified type index
		static void HK_CALL appendBlocksWithTypeIndex(const hkTrackerScanSnapshot* scanSnapshot, const TypeIndexMap& typeIndexMap, int typeIndex, hkArray<const Block*>& blocksOut);
			/// Append all blocks that have a type index
		static void HK_CALL appendBlocksWithTypeIndex(const hkTrackerScanSnapshot* scanSnapshot, const TypeIndexMap& typeIndexMap, hkArray<const Block*>& blocksOut);

			/// Get a type index for the type
		static int HK_CALL getTypeIndex(const Block* block, const TypeIndexMap& typeIndexMap);


			/// Set the type of any type found in the snapshot, not in the typeIndexMap that has the specified prefix to the type index
		static void HK_CALL setTypeIndexByNamePrefix(hkTrackerScanSnapshot* scanSnapshot, const char* prefix, hkBool prefixFollowedByCapital, int typeIndex, TypeIndexMap& typeIndexMap);

			/// Given a type by, try and find the associated node. Will look for the type in the memory trackers type definitions, and if that fails
			/// will do a linear search of the allocations of that type from the scanSnapshot. If the type is not found returns HK_NULL.
		static const RttiNode* HK_CALL findTypeFromName(const hkTrackerScanSnapshot* scanSnapshot, const char* name);

			/// Work out the path of blocks to the a block
		static void HK_CALL calcPath(const ParentMap& parentMap, const Block* block, hkArray<const Block*>& path);
			/// Calculates the path to a particular block from a parent map
		static void HK_CALL calcTextPath(const hkTrackerScanSnapshot* scanSnapshot, const ParentMap& parentMap, const Block* block, hkStringBuf& pathOut);

			/// Append the block type
		static void HK_CALL appendBlockType(const Block* block, hkOstream& stream);
			/// Get the block type as a string
		static void HK_CALL getBlockTypeAsText(const Block* block, hkStringBuf& string);


    protected:

		static void HK_CALL _calcParentMap(hkTrackerScanSnapshot* scanSnapshot, const hkArray<const Block*>& rootBlocks, DontFollowMap& dontFollowMap, FollowFilter* filter, ParentMap& parentMap);

};

#endif // HKBASE_SCAN_REPORT_UTIL_H

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
