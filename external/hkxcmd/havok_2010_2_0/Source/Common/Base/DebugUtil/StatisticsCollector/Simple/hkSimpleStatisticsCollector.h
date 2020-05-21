/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKMONITOR_SIMPLE_STATISTICS_COLLECTOR_H
#define HKBASE_HKMONITOR_SIMPLE_STATISTICS_COLLECTOR_H

#include <Common/Base/DebugUtil/StatisticsCollector/hkStatisticsCollector.h>
#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

/// An implementation of a statistics collector.
/// This collector creates Nodes (the same Nodes as the hkMonitorStreamAnalyzer)
/// on the fly as it sees objects. The Nodes will contain two values, the current used
/// memory and the current allocated memory. The Nodes will be in a tree structure matching
/// the structure requested by the begin/end and push/pop methods.
/// See the hkStatisticsCollector for more information
class hkSimpleStatisticsCollector: public hkStatisticsCollector
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		/// Create a hkSimpleStatisticsCollector
		hkSimpleStatisticsCollector(hkVtableClassRegistry* vtblReg);

		/// Delete the collector. Will delete all held data (all current Node trees (snapshots) and the stack)
		virtual ~hkSimpleStatisticsCollector();

		/// Begin a new section in the stream
		/// You can thus have different types of
		/// snapshot all in the same stream .
		/// For instance you may want the ENGINE
		/// only snapshot but also after that
		/// one that takes in all categories.
		void beginSnapshot( int statisticClasses );

		/// End the current snapshot.
		void commitSnapshot( );

		/// Write the stats from the current Node tree to a text stream.
		void writeStatistics( hkOstream& outstream, int reportLevel = hkMonitorStreamAnalyzer::REPORT_SUMMARIES );

		/// Reset the whole collector (wipes all snapshots and the current Node tree)
		void reset();

		typedef hkMonitorStreamAnalyzer::Node Node;

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

	protected:

		Node* newNode( Node* parent, const char* name, int size, const void* obj, int allocatedSize);

        Node* _startObject(const char* name, const void* obj, int size);
        void _endObject(Node* node, const void* obj, int size);

	public:
		hkArray<Node*>::Debug m_snapshots;
		
	protected:
        const char* _isKnown(const void* obj) const { return m_knownObjects.getWithDefault(obj, 0); }
        void _addKnown(const void* obj, const char* what) { m_knownObjects.insert(obj, what); }

		/// Known objects
		hkPointerMap<const void*, const char*, hkContainerDebugAllocator> m_knownObjects;
		hkArray<Node*>::Debug m_currentStack;
};

#endif // HKBASE_HKMONITOR_SIMPLE_STATISTICS_COLLECTOR_H

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
