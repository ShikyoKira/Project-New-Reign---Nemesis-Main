/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKMONITOR_STREAM_STATISTICS_COLLECTOR_H
#define HKBASE_HKMONITOR_STREAM_STATISTICS_COLLECTOR_H

#include <Common/Base/DebugUtil/StatisticsCollector/hkStatisticsCollector.h>
#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

#define HK_MEM_STREAM_COMMAND_PUSH  'P' // new dir or new node
#define HK_MEM_STREAM_COMMAND_POP   'p' // out of node/dir.
#define HK_MEM_STREAM_COMMAND_CHUNK 'C' // new chunk alloc

/// An implementation of a statistics collector.
/// Similar to hkSimpleStatisticsCollector except that it does not create
/// Nodes as it progresses through the stats collection, rather it writes
/// to a command stream. That stream can then be interpreted later,
/// possibly on a different platform (for use with the Visual Debugger for instance),
/// and then created into Nodes. See the hkStatisticsCollector for more information
class hkStreamStatisticsCollector: public hkStatisticsCollector
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Create a hkStreamStatisticsCollector with the stream
			/// preallocated to the given size in bytes. If the stream
			/// uses more than this size it will reallocate itself automatically.
		hkStreamStatisticsCollector(hkVtableClassRegistry* vtblReg, int memSize);

			/// Delete the collector
		virtual ~hkStreamStatisticsCollector();

			/// The collector uses the same Nodes
			/// as the hkMonitorStreamAnalyzer so that the trees
			/// produced can be inspected by the same code that
			/// you use to inspect the hkMonitorStream.
		typedef hkMonitorStreamAnalyzer::Node Node;

			/// Begin a new section in the stream
			/// You can thus have different types of
			/// snapshot all in the same stream .
			/// For instance you may want the ENGINE
			/// only snapshot but also after that
			/// one that takes in all categories.
		void beginSnapshot( int statisticClasses );

			/// End the current snapshot.
		void endSnapshot( );

			/// Reset the whole stream (wipes all snapshots)
		void reset();

			/// Set the data stream from a different source, usually
			/// a different computer or console.
		void setDataByCopying(const char* buffer, int length, hkBool endianSwap = false);

			/// Make a Node tree from the current stream. If you provide an array to hold the strings
			/// for the Node names then the Nodes can persist after you have deleted this collector,
			/// but be default the Node names will just point into the original stream data (that
			/// has a copy of the names in it). Each node has two values corresponding to the
			/// Used and Allocated memory for the object(s) you collected.
			/// You own the returned Nodes, so delete them after you have inspected their contents.
			/// The string table is optional.
		void makeTree(hkArray<Node*>& m_nodesOut, hkArray<hkStringPtr>* stringTable = HK_NULL);

			/// Write the stats to a text stream. It internally calls makeTree first and
			/// and deletes the nodes once produced when done. It is just a simple ASCII
			/// version of the tree.
		void writeStatistics( hkOstream& outstream, int reportLevel );


        // hkStatisticsCollector interface
        virtual void addReferencedObject(const char* fieldName, const hkReferencedObject* obj, int flags = 0) ;
        // hkStatisticsCollector interface
        virtual void addObject( const hkClass& cls, const char* fieldName, const void* obj, int flags = 0);
        // hkStatisticsCollector interface
        virtual void addChunk(MemoryType type, const char* name, const void* chunkAddress, int usedSize, int allocatedSize = 0) ;
        // hkStatisticsCollector interface
        virtual void pushDir( const char* dirName );
        // hkStatisticsCollector interface
        virtual void popDir();


			/// Get the current data stream. Commonly used to then send this block of data to another process
			/// or computer
		inline const char* getDataStream() { return m_stream.getSize() > 0? m_stream.begin() : HK_NULL; }

			///Get the size of the current data stream.
		inline int getDataStreamSize() { return m_stream.getSize(); }

	public:

		struct Command
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkStreamStatisticsCollector::Command );
			char cmd; // one of the three above
			hkUint8 slen; // chars following for name string. can be 0.
			hkUint16 memSize; // 0 for just a dir
			hkUint16 allocSize; // 0 for just a dir
		};

		hkArray<char> m_stream; // stream of commands (and names)

		inline hkBool hasSpace() { return m_stream.getCapacity() > (m_stream.getSize() + (int)sizeof(Command) + 64); }

	protected:
        hkBool _isKnown(const void* obj) const { return m_knownObjects.getWithDefault(obj, 0) != 0; }
        void _addKnown(const void* obj) { m_knownObjects.insert(obj, 1); }
        void _startObject( const char* name, const void* obj, int size);
        void _endObject( );

		hkPointerMap<const void*, int> m_knownObjects;
		//int	m_enabledStatisticClasses;
};


#endif // HKBASE_HKMONITOR_STREAM_STATISTICS_COLLECTOR_H

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
