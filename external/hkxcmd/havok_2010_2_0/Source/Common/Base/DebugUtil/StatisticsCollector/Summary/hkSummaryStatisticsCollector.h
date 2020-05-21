/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SUMMARY_STATISTICS_COLLECTOR_H
#define HK_SUMMARY_STATISTICS_COLLECTOR_H

#include <Common/Base/DebugUtil/StatisticsCollector/hkStatisticsCollector.h>

#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Base/DebugUtil/StatisticsCollector/Report/hkReportStatisticsCollector.h>

class hkSummaryStatisticsCollector: public hkStatisticsCollector
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Ctor
		hkSummaryStatisticsCollector(hkVtableClassRegistry* vtblReg);

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

            /// Cleat the current totals
        void clear();

            /// Get the totals
        const hkStatisticClassCount& getTotalCount() const { return m_totalMemory; }

	protected:

        hkBool _isKnown(const void* obj);
        void _addKnown(const void* obj);

		void _startObject( const hkClass* cls, const char* fieldName, const void* obj);
		void _endObject();

        void _addCumulative(int usedSize, int allocatedSize);


            /// Known objects
        hkPointerMap<const void*, int> m_knownObjects;

        hkStatisticClassCount m_totalMemory;

};


#endif // HK_REPORT_STATISTICS_COLLECTOR_H

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
