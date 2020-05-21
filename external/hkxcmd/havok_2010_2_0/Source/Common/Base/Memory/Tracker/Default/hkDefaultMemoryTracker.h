/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_DEFAULT_MEMORY_TRACKER_H
#define HKBASE_DEFAULT_MEMORY_TRACKER_H

#include <Common/Base/Container/PointerMap/hkMap.h>

#include <Common/Base/Container/StringMap/hkStringMap.h>

#include <Common/Base/Memory/Allocator/FreeList/hkFreeList.h>

#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

#include <Common/Base/Memory/Tracker/hkMemoryTracker.h>

#include <Common/Base/Reflection/Registry/hkClassNameRegistry.h>

class hkOstream;

struct hkDefaultMemoryTrackerAllocator
{
	hkMemoryRouter::Allocator& get(void*) { return *s_allocator; }
	static hkMemoryRouter::Allocator* s_allocator;
};

extern const hkClass hkDefaultMemoryTrackerClass;

class hkDefaultMemoryTracker : public hkMemoryTracker //+reflected(false)
{
	public:
		HK_DECLARE_PLACEMENT_ALLOCATOR();

		// A class allocation
		struct ClassAlloc //+reflected(false)
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,ClassAlloc);
			enum 
			{
				FLAG_REFERENCED_OBJECT = 0x1,
				FLAG_NOT_ALLOCATED = 0x2,		///< If set - the object resides in this memory, but it wasn't allocated from the heap
			};
				/// ==
			hkBool operator==(const ClassAlloc& rhs) const;
				/// !=
			hkBool operator!=(const ClassAlloc& rhs) const { return !(*this == rhs); }

			const char* m_typeName;					///< The type name
			hk_size_t m_size;						///< The size of the allocation
			void* m_ptr;							///< The pointer
			int m_flags;							///< The flags
		};
		typedef hkPointerMap<void*, ClassAlloc*, hkDefaultMemoryTrackerAllocator> ClassAllocMapType;
		typedef hkPointerMap<void*, int, hkDefaultMemoryTrackerAllocator> DeletedMap;
		typedef hkPointerMap<void*, ClassAlloc*, hkDefaultMemoryTrackerAllocator> CreatedMap;

		// hkMemoryTracker implementation
        virtual void onNewReferencedObject(const char* typeName, hk_size_t size, void* ptr);
        virtual void onDeleteReferencedObject(void* ptr);
        virtual void onNewObject(const char* typeName, hk_size_t size, void* ptr);
        virtual void onDeleteObject(void* ptr);

        virtual void addTypeDefinition(const TypeDefinition& def);
        virtual const TypeDefinition* findTypeDefinition(const char* name);

		virtual hk_size_t getTypeDefinitions(const TypeDefinition** typeDefinitions);
	
			/// Find the class allocation
		const ClassAlloc* findClassAlloc(void* ptr) const;

			/// Will assert when this alloc is removed.
		void setAssertRemoveAlloc(const ClassAlloc* alloc);

			/// Returns map of all of the class allocations
		const ClassAllocMapType& getClassAllocations() const { return m_classAllocMap; }

            // hkReferencedObject Implementation
        virtual const hkClass* getClassType() const { return &hkDefaultMemoryTrackerClass; }

			/// Returns true if delete tracking is enabled.
			/// Delete tracking will cause all memory deletions addresses to be stored in map
			/// This can be used to determine if an address has become invalid - even if a new object has been constructed at
			/// the same address
		hkBool isTrackingEnabled() const { return m_trackingEnabled; }
			/// Enable delete tracking
		void setTrackingEnabled(hkBool enable) { m_trackingEnabled = enable; }
			/// Clears any state in the delete tracking table
		void clearTrackingHistory();
			/// Get the delete map
		const DeletedMap& getDeletedMap() const { return m_deletedMap; }
			/// Get the created map
		const CreatedMap& getCreatedMap() const { return m_createdMap; }

			/// Enter the critical section - stopping other threads updating the trackers info
		hkCriticalSection& getCriticalSection() { return m_criticalSection; }

		/// Ctor
		hkDefaultMemoryTracker(hkMemoryAllocator* allocator);

	protected:

		ClassAlloc* _addClassAlloc(const char* type, hk_size_t size, void* ptr, int flags);
		void _removeClassAlloc(void* ptr);

		hkBool m_trackingEnabled;	///< 
		DeletedMap m_deletedMap;	///< Stores the list of deleted objects
		CreatedMap m_createdMap;	///< Stores objects created and active

		typedef hkArray<void*, hkDefaultMemoryTrackerAllocator> AllocArrayType;
		typedef hkStringMap<const TypeDefinition*, hkDefaultMemoryTrackerAllocator> NameTypeMap;
        
		hkFreeList m_classAllocFreeList;
		ClassAllocMapType m_classAllocMap;
		NameTypeMap m_nameTypeMap;

		const ClassAlloc* m_assertOnRemove;				

		mutable hkCriticalSection m_criticalSection;
};

#endif // HKBASE_MEMORY_TRACKER_H

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
