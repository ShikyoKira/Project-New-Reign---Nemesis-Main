/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKMONITOR_STATISTICS_COLLECTOR_H
#define HKBASE_HKMONITOR_STATISTICS_COLLECTOR_H

#include <Common/Base/Container/PointerMap/hkPointerMap.h>
class hkStringBuf;

/// This is the interface to a statistics collector for Havok objects. Currently it only
/// collects memory usage statistics but could be extended in future to collect other
/// useful stats. Timing stats are collected separately through the hkMonitorStream.
///
/// All hkReferencedObject in Havok (most user level objects) have the following virtual calls:
///
/// \verbatim
///    virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const
///    virtual const hkClass* getClassType() const
/// \endverbatim
///
/// The getClassType method provides a way to determine the exact type of a class. The default implementation
/// of getClassType will return HK_NULL. In this situation if the type is serialized an implementation of hkStatisticsCollector
/// may still be able to determine the type by looking up the type based on the vtbl. Thus in most circumstances
/// if a serialized type is being used, using the default behavior of returning HK_NULL, will produce the correct
/// result. To make a non serialized track memory - this method should be implemented. Care must be taken with any derived type
/// to make sure it also implements the method, else the wrong type could be returned. The setting up of the hkClass
/// information for a non serialized type can be achieved by placing
///
/// \verbatim extern const hkClass YourClassNameClass; \endverbatim
///
/// in the header - before your class. Implementing the getClassTypeMethod to return the type
///
/// \verbatim virtual const hkClass* getClassType() const { return &YourClassNameClass; } \endverbatim
///
/// Finally the type needs to be implemented in the .cpp file. Place the following after your header includes
///
/// \verbatim HK_REFLECTION_DEFINE_STUB_VIRTUAL(YourClassName, hkReferencedObject); \endverbatim
///
/// The second parameter to the macro - should be the base class that YouClassName derived from.
///
/// The 'calcContentStatistics' methods purpose is to inform a hkStatisticsCollector interface about all of the
/// additional memory consumed internally by a class. The method should exclude
/// the memory taken up by the object itself (the object could be embedded in another object or on the stack).
///
/// If the type is serialized, and all of the additional memory consuming
/// members (such as arrays, and pointers) are also serialized, then the default implementation will automatically
/// track all of the memory usage using hkStatisticsCollectorUtil which uses the serialization reflection in
/// order to track memory. If the class contains some members which aren't serialized but need to be tracked, then
/// the appropriate calls with the memory to track can be made on the hkStatisticsCollector. The remainder of the serialized
/// types members can be tracked by
///
/// \verbatim hkStatisticsCollectorUtil::addClassContents(const void* obj, const hkClass& cls, hkStatisticsCollector* collector); \endverbatim
///
/// Finally a call to the parent classes implementation of calcContentStatistics, should be made, with the parents
/// class type. Passing the parents class type will ensure if the parents implementation is the hkStatisticsCollectorUtil
/// implementation, then the correct memory will be followed.
///
/// On a class which doesn't implement serialization, the same two steps should be made - first tell the
/// hkStatisticsCollector of any additional memory consumed by just this classes implementation, and then call the parents
/// implementation. For example
///
/// \verbatim
/// class MyDerived: public: MyBase
/// {
///      ...
///     virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const;
///     virtual const hkClass* getClassType() const { return &MyDerivedClass; }
///     ...
///     protected:
///     hkArray<int> m_array;
///     hkpRigidBody* m_body;
/// };
///
/// void MyClass::calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const
/// {
///     // Add data for this class
///     collector->addReferencedObject("body", m_body);
///     collector->addArray("array", m_array);
///     // Let the base class inform the collector of its memory consumption
///     MyBase::calcContentStatistics(collector, &MyBaseClass);
/// }
/// \endverbatim
///
/// Normally the statistics collectors record the memory used the object
/// (m_memSizeAndFLags + all used (<= allocated, e.g.: hkArray:getSize())) and also the total
/// allocated (m_memSizeAndFLags + all allocated (e.g.: hkArray::getCapacity()) ) so that
/// you can work out not only the total memory required but also if there is much wasted memory.
///
/// If an object is serialized in, its memory is marked as not allocated. As it stands this memory
/// is not tracked at all by the hkStatistics collector.
///
/// Passing HK_NULL for a pointer to a piece of memory is acceptable - and just indicates that memory hasn't been
/// allocated. Additionally an implementation of hkStatisticsCollector, should correctly handle if the same block is
/// passed to the collector appropriately.
class hkStatisticsCollector: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Memory Type
        enum MemoryType
        {
            MEM_OBJECT,         ///< Memory allocated to an object
            MEM_NORMAL,         ///< Memory allocated from hkMemory
            MEM_ALLOCATED,      ///< Allocated using hkAllocate or hkAlignedAllocate
            MEM_ARRAY,          ///< Allocated with the 'array allocator'
            MEM_OTHER,          ///< Allocated with some other memory allocator (external memory manager)
            MEM_LAST,           ///< Marks end of list
        };

        enum Flag
        {
            EXCLUDE_OBJECT_MEMORY = 0x1,        ///< If set the memory of the object itself will not be added
        };

            /// Add an object derived from hkReferencedObject. The field name and obj can be HK_NULL as necessary
            /// Flags is a combination of Flag flags or-ed together.
        virtual void addReferencedObject(const char* fieldName, const hkReferencedObject* obj, int flags = 0) = 0;

            /// Add an object not derived from hkReferencedObject. The pointer must point to the start of the object. NOTE that cls, and the pointer
            /// should point to the most derived type of the object to collect memory correctly.
            /// fieldName and the object can be HK_NULL as necessary.
            /// Flags is a combination of Flag flags or-ed together.
        virtual void addObject( const hkClass& cls, const char* fieldName, const void* obj, int flags = 0) = 0;

			/// Add a named chunk of memory for the current object. Both used and total allocated are usually given.
        virtual void addChunk(MemoryType type, const char* name, const void* chunkAddress, int usedSize, int allocatedSize = 0) = 0;

			/// Add a directory for logically grouping objects
		virtual void pushDir( const char* dirName ) = 0;

			/// End the current directory
		virtual void popDir() = 0;

            /// Add a normal chunk
        void addNormalChunk(const char* name, const void* p, int size, int allocated = 0);
            /// Add an object chunk
        void addObjectChunk(const char* name, const void* p, int size, int allocated = 0);

			/// Get the vtbl registry
		hkVtableClassRegistry* getVtblClassRegistry() const { return m_vtblRegistry; }

            /// Returns the size of an allocated object
        int getReferencedObjectSize(const hkReferencedObject* obj);

			/// Returns the amount of memory that is allocated for a size of an actual allocation
			/// (The allocator generally uses more space than the allocation request asks for)
		static int HK_CALL getAllocatedSize(const void* p, int size);

		void addStringPtr(const char* name, const hkStringPtr& s);

			/// A convenience templated function to allow an hkArray to be easily added.
		template<typename T>
        HK_FORCE_INLINE void addArray( const char* name, const hkArray<T>& array )
		{
			if ( !(array.m_capacityAndFlags & array.DONT_DEALLOCATE_FLAG) || (m_includeUnmanagedArrayMemory))
			{
				int allocated = getAllocatedSize((void*)array.begin(), array.getCapacity() * hkSizeOf(T));
                addChunk( MEM_ARRAY, name, array.begin(), array.getSize() * hkSizeOf(T), allocated );
			}
		}

		template<typename T>
        HK_FORCE_INLINE void addSmallArray( const char* name, const hkSmallArray<T>& array )
		{
			if ( !(array.m_capacityAndFlags & array.DONT_DEALLOCATE_FLAG) || (m_includeUnmanagedArrayMemory))
			{
				// Just assume the allocated is the same as used - as can't use the 'getAllocatedSize' as memory allocation
				// is from a specialized allocator
                addChunk( MEM_ARRAY, name, array.begin(), array.getSize() * hkSizeOf(T), array.getCapacity() * hkSizeOf(T) );
			}
		}
		
		template<typename T>
		HK_FORCE_INLINE void addRefObjectArrayAndRecurseOnItems (const char* name, const hkArray<T>& array)
		{
			pushDir(name);

			addArray("array", array);

			for (int i=0; i<array.getSize(); ++i)
			{
				addReferencedObject(name, array[i]);
			}
			popDir();

		}

		template<typename T>
		HK_FORCE_INLINE void addRefObjectArrayAndRecurseOnNamedItems (const char* arrayName, const hkArray<T>& array)
		{
			pushDir(arrayName);
			addArray("array", array);

			for (int i=0; i<array.getSize(); ++i)
			{
				addReferencedObject(array[i]->m_name ? array[i]->m_name.cString() : "unnamed", array[i]);
			}
			popDir();

		}

		template<typename KEY, typename VALUE>
        HK_FORCE_INLINE void addPointerMap( const char* name, const hkPointerMap<KEY,VALUE>& map )
		{
			int used = map.getMemSize();
			int allocated = getAllocatedSize( map.getMemStart(), used);
            addChunk( MEM_NORMAL, name, map.getMemStart(), used, allocated );
		}

            /// Find the type - first thru calling the getClassType on the obj, and then using the registry if there is one set up
        const hkClass* findClassType(const hkReferencedObject* obj);

			/// Return the memory type as a string
		static const char* HK_CALL memoryTypeToCstring(MemoryType type);

			/// Turn memory size to text
		static void HK_CALL memorySizeToText(int size, hkStringBuf& stringOut);

			/// Ctor. Pass in the vtblClassRegistry that will be used for type lookup. Can pass in HK_NULL.
			/// NOTE: vtblReg is not reference counted, but must be maintained for the life of the collector
		hkStatisticsCollector(hkVtableClassRegistry* vtblReg);

			/// Dtor
		~hkStatisticsCollector();

    protected:
        void addReferencedObjectMemory(const hkReferencedObject* obj, int flags);
        void addObjectMemory(const hkClass& cls, const void* obj, int flags);

	public:
			/// A client data which can be used to pass information from the parent to the child.
			/// For children of hkpWorld this points to the hkpCollisionInput.
		const void* m_clientData;
		hkVtableClassRegistry* m_vtblRegistry;

			/// If this is true, non-Havok owned array memory will also be counted.
		bool m_includeUnmanagedArrayMemory;
};


#endif // HKBASE_HKMONITOR_STATISTICS_COLLECTOR_H

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
