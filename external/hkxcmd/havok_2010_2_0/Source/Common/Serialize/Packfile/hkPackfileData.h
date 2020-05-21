/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_PACKFILE_DATA_H
#define HK_PACKFILE_DATA_H

#include <Common/Serialize/Resource/hkResource.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>

class hkTypeInfo;

	/// An interface to the data loaded by a packfile reader.
class hkPackfileData : public hkResource
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Create an empty object.
		hkPackfileData();

			// Inherited
		virtual ~hkPackfileData();

			// Inherited
		virtual void callDestructors();

			// Inherited
		virtual void getImportsExports( hkArray<Import>& impOut, hkArray<Export>& expOut ) const;

			// Inherited
		virtual const char* getName() const { return m_name; }

			/// Set the name of this data.
		void setName(const char* name);

			/// Prevent destructors from being called.
			/// Normally destructors are automatically called to free any runtime
			/// allocations (typically array resizes). You can prevent this behavior
			/// if you are cleaning up the data in some other way such as explicit
			/// destructor calls.
		void disableDestructors() { m_destructorsEnabled = false; }

			// Inherited
		virtual void* getContentsPointer(const char* typeName, const hkTypeInfoRegistry* finishRegistry) const;

			// Inherited
		virtual const char* getContentsTypeName() const;

	protected:

		struct Chunk
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkPackfileData::Chunk );
			Chunk(void* p, int n, HK_MEMORY_CLASS c) : pointer(p), numBytes(n), memClass(c) { }
			Chunk() {}
			void* pointer;
			int numBytes;
			HK_MEMORY_CLASS memClass;
		};
	public:
		
		// Internal functions for use by packfile readers.

			// Add a normal allocation.
		void addAllocation(void* p) { m_memory.pushBack(p); }

			// Check if the packfile data is not empty.
		bool isDirty() const { return m_topLevelObject || m_memory.getSize() > 0 || m_chunks.getSize() > 0 || m_exports.getSize() > 0 || m_imports.getSize() > 0; }

			// Add a chunk allocation.
		void addChunk(void*p, int n, HK_MEMORY_CLASS c) { m_chunks.pushBack( Chunk(p,n,c) ); }

			// Track object 'o' for cleanup in callDestructors().
		void trackObject(void* o, const char* n) { m_trackedObjects.insert(o, n); }

			// Make an object visible with a symbolic name.
		void addExport( const char* symbolName, void* object );

			// Remove an object from the export list.
		void removeExport( void* object );

			// Link a location to the symbolic name.
		void addImport( const char* symbolName, void** location );

			// Remove a location from the import list.
		void removeImport( void** location );

			// Set top level object.
		void setContentsWithName(void* topLevelObject, const char* typeName);

			// Set packfile data class name registry.
		void setPackfileClassNameRegistry(const hkClassNameRegistry* classReg);

			// Return TRUE if objects are finished.
		hkBool32 finishedObjects() const;

		virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const;

	protected:

		typedef hkPointerMap<void*, const char*> TrackedObjectMap;
		typedef hkStringMap<const hkTypeInfo*> TrackedTypeMap;

	protected:

		void* m_topLevelObject;
		char* m_name;
		TrackedObjectMap m_trackedObjects;
		mutable TrackedTypeMap m_trackedTypes;
		hkRefPtr<const hkClassNameRegistry> m_packfileClassRegistry;
		hkBool32 m_destructorsEnabled;
		hkArray<void*> m_memory;
		hkArray<Chunk> m_chunks;
		hkArray<Export> m_exports;
		hkArray<Import> m_imports;

		friend class hkPackfileObjectUpdateTracker;
};

#endif // HK_PACKFILE_DATA_H

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
