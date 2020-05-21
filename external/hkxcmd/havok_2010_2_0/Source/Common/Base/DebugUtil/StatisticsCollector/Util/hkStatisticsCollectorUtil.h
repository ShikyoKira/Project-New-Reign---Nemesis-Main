/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_STATISTICS_COLLECTOR_UTIL_H
#define HK_STATISTICS_COLLECTOR_UTIL_H

#include <Common/Base/DebugUtil/StatisticsCollector/hkStatisticsCollector.h>
#include <Common/Base/Reflection/hkClass.h>

/// A utility class used for working out automatically 'calcContentStatistics' for classes. This is achieved by using the
/// reflection information held with classes. The methods will automatically recurse down contained members of an object
/// passed in finding out how much memory child classes use up.
class hkStatisticsCollectorUtil
{
public:
		/// Uses the reflection information to write to the statistics collector the amount of memory that is being used
		/// getClassType on obj must return a type for the function to work
		/// Passing obj as HK_NULL is valid and will return immediately
	static void HK_CALL defaultCalcStatistics(const hkReferencedObject* obj, hkStatisticsCollector* collector);

		/// Uses reflection to work out memory used. obj must be a pointer to a cls object.
		/// Passing obj as HK_NULL is valid and will return immediately
	static void HK_CALL defaultCalcStatistics(const void* obj, const hkClass& cls, hkStatisticsCollector* collector);

		/// Adds only the fields at this level.
	static void HK_CALL addClassContents(const void* obj, const hkClass& cls, hkStatisticsCollector* collector);

		/// Adds only the fields at this level and up the tree to the parent.
	static void HK_CALL addClassContentsAll(const void* obj, const hkClass& cls,hkStatisticsCollector* collector);
		/// Adds only the fields at this level and up the tree to the parent.
	static void HK_CALL addClassContentsAll(const hkReferencedObject* obj, hkStatisticsCollector* collector);

		/// Returns true if the class or any of its parent classes contains any members
	static hkBool HK_CALL hasContents(const hkClass& cls);

		/// Returns true if the member type does not allocate any extra memory
	static hkBool HK_CALL isMemberSimple(const hkClassMember& mem);
		/// Returns true if a class cannot allocate any extra memory
	static hkBool HK_CALL isClassSimple(const hkClass& clsIn);

protected:

	///// The in pointer must be an hkReferencedObject pointer
	//static const hkClass* _getClass(void* in);

	static hkBool _isClassSimple(const hkClass& clsIn);
	static void _addObjectContents(const char* fieldName, const void* obj, const hkClass& cls, hkStatisticsCollector* collector);
	static void _addArrayContents(const hkClassMember& mem, void* data, int size, hkStatisticsCollector* collector);
	static void _addHomogeneousArrayContents(const char* fieldName, const hkClass& cls, void* data, int size, hkStatisticsCollector* collector);
	static void _addArrayData( const char* fieldName, void* data, int size, int capacity, int objSize, hkStatisticsCollector* collector);
	static int _getArrayElementSize(const hkClassMember& mem);
	static void _addObject(const char* fieldName, const void* obj, const hkClass& cls, hkStatisticsCollector* collector);
	static void _addCstring(const hkClassMember& mem, const char* string, hkStatisticsCollector* collector);


	//static void _addChildObjectContents(const char* fieldName, void* obj, const hkClass& cls, hkStatisticsCollector* collector);
};


#endif // HK_STATISTICS_COLLECTOR_UTIL_H

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
