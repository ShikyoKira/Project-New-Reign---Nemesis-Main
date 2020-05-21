/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_STACKTRACER_H
#define HKBASE_STACKTRACER_H

class hkOstream;

	/// An object which can generate stack traces.
	/// Some platforms may also be able to associate addresses to
	/// function and source file information.
class hkStackTracer : public hkReferencedObject, public hkSingleton<hkStackTracer>
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE_CLASS);

			/// Stores multiple call stacks in a tree to save space.
			/// Call stacks are inserted and return a TraceId which can be
			/// later used to retrieve the original call stack.
			/// For this to work optimally, the full call stack should be used
			/// so that identical prefixes can be merged and the tree does not
			/// become multi-rooted.
		class CallTree
		{
			public:
				HK_DECLARE_PLACEMENT_ALLOCATOR();

				struct Node
				{
					HK_DECLARE_POD_TYPE();
					Node() : m_value(0), m_parent(-1), m_firstChild(-1), m_next(-1), m_usageCount(0) {}
					hkUlong m_value;
					int m_parent;
					int m_firstChild;
					int m_next;
						/// The *external* usage count.
					int m_usageCount;
				};

				typedef int TraceId;

				CallTree(hkMemoryAllocator* allocator = HK_NULL) : m_allocator(allocator), m_rootNode(-1), m_firstFreeNode(-1) {}
					/// Copy Ctor
				CallTree(const CallTree& rhs);

					/// Assignment
				void operator=(const CallTree& rhs);

				~CallTree();

				void init( hkMemoryAllocator* a ) { m_allocator = a; }
				void quit();

					/// Get a stack trace and insert it into the given tree.
				TraceId insertCallStack( hkStackTracer& tracer );

					/// Add a call stack to the tree and get its id.
				TraceId insertCallStack( const hkUlong* addrs, int numAddrs );

					/// The identified callstack need no longer be stored.
				void releaseCallStack( TraceId id );

					/// Retrieve a call stack from an id.
				int getCallStack( TraceId id, hkUlong* addrs, int maxAddrs ) const;
					/// Return the amount of addresses in the full call stack
				int getCallStackSize(TraceId id) const;

					/// Get traces - the ids map into the arrays. The parents hold whats the parent for the current index. The parent is -1 if its the root
				void getTraces(hkArray<hkUlong>& addrs, hkArray<int>& parents) const;
		
					/// Dump a trace from an id
				void dumpTrace(TraceId id, hkOstream& stream) const;

					/// Read only access for traversal
				const Node& node(int i) const { return m_nodes[i]; }

					/// Swap
				void swap(CallTree& rhs);

					/// Get the nodes
				const hkArrayBase<Node>& getNodes() const { return m_nodes; }

				hkBool isEmpty() const;

			protected:
					/// Get a free node or allocate one if none are free.
				int getFreeNode();

					/// Check that the node is still required in the call tree.
				void removeNodeIfUnused( int id );

			protected:

				hkArrayBase<Node> m_nodes;
				hkMemoryAllocator* m_allocator;
					/// The index of the root node or -1 if there is no root node yet.
				int m_rootNode;
					/// Start of a stack of free nodes, linked by m_next links.
				int m_firstFreeNode;
		};

		~hkStackTracer();

			/// The type of the callback given to dumpStackTrace.
		typedef void (HK_CALL *printFunc)(const char*, void* context);

			/// Create a stacktracer.
		static hkStackTracer* HK_CALL create();

			/// Print the stack trace with pfunc.
			/// pfunc may be called multiple times.
			/// The output format is platform specific.
		void dumpStackTrace( const hkUlong* trace, int numtrace, printFunc pfunc, void* context=HK_NULL ) const;

			/// Write at most maxtrace stack entries into 'trace'.
			/// Return the number of entries written.
		int getStackTrace( hkUlong* trace, int maxtrace );

			/// If you dynamically load DLLs then you will need to refresh the symbols
			/// so that the stack tracer can see them
		void refreshSymbols();

			/// Print the module information with pfunc.
			/// pfunc may be called multiple times.
			/// The output format is platform specific.
		void getModuleInfo( printFunc pfunc, void* context=HK_NULL ) const;
		
	protected:

		hkStackTracer();

		void* m_impl;
};

#endif // HKBASE_STACKTRACER_H

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
