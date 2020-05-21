/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BINARY_PACKFILE_READER_H
#define HK_BINARY_PACKFILE_READER_H

#include <Common/Compat/Deprecated/Packfile/hkPackfileReader.h>

class hkStreamReader;
class hkPackfileHeader;
class hkPackfileSectionHeader;
class hkTypeInfoRegistry;
class hkChainedClassNameRegistry;
class hkPackfileObjectUpdateTracker;
typedef hkPackfileObjectUpdateTracker hkBinaryPackfileUpdateTracker;

/// Reads a memory image packfile.
/// Note that unlike the hkBinaryPackfileWriter which can export
/// from any platform to any platform, this class does minimal
/// processing and can only read files which have been written
/// for the host platform.
class hkBinaryPackfileReader : public hkPackfileReader
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Create an uninitialized reader.
		hkBinaryPackfileReader();
		
			/// Cleans up any allocated memory.
		~hkBinaryPackfileReader();

			/// Load the entire file, fixes up all pointers.
			/// Use a new packfile reader for each loadEntireFile call.
		virtual hkResult loadEntireFile( hkStreamReader* reader );

			/// Load from a preallocated memory chunk.
			/// Given a binary packfile that is already in
			/// memory, set all the header and section pointers
			/// inplace and fixup all the pointers.
			/// This method does not do any memory allocation and the
			/// user must ensure that "data" remains valid for the lifetime
			/// of the packfile.
			/// Use a new packfile reader for each loadEntireFileInplace call.
		virtual hkResult loadEntireFileInplace( void* data, int dataSize );

			/// Implements hkPackfileReader::getContentsWithRegistry().
			/// For packfiles with metadata, all hkClass pointers are replaced
			/// with corresponding hkClass from hkBuiltinTypeRegistry.
		virtual void* getContentsWithRegistry( const char* className, const hkTypeInfoRegistry* finish );

			// Inherited from hkPackfileReader
		virtual const char* getContentsClassName() const;

			/// Implements hkPackfileReader::getPackfileData().
			/// Return packfile data with all the hkClass signatures checked
			/// and updated hkVariant class pointers.
			/// For packfiles with metadata, all hkClass pointers are replaced
			/// with corresponding hkClass from hkBuiltinTypeRegistry.
		virtual hkPackfileData* getPackfileData() const;

			// Inherited from hkPackfileReader
		virtual hkArray<hkVariant>& getLoadedObjects() const;

			// Inherited from hkPackfileReader
		virtual hkVariant getTopLevelObject() const;

			// Inherited from hkPackfileReader
		virtual hkObjectUpdateTracker& getUpdateTracker() const;

	public:

			//
			// Special load/save methods for more control
			//

			/// Load the file header only.
			/// If dst is null, memory is allocated internally.
		hkResult loadFileHeader(hkStreamReader* reader, hkPackfileHeader* dst = HK_NULL);

			/// Return a reference to the header.
			/// Must have called loadHeader() or loadEntireFile() first.
		const hkPackfileHeader& getFileHeader() const;

			/// Get the number of sections in this file.
			/// Must have called loadHeader() or loadEntireFile() first.
		int getNumSections() const;

			/// Read the section headers into dst.
			/// Assumes the stream is in the correct position.
			/// If dst is null, memory is allocated internally.
			/// The dst should be big enough to hold header.m_numSections sections.
		hkResult loadSectionHeadersNoSeek(hkStreamReader* reader, hkPackfileSectionHeader* dst = HK_NULL);

			/// Return a reference to the i'th section.
			/// Must have called loadSectionHeader() or loadEntireFile() first.
		hkPackfileSectionHeader& getSectionHeader(int idx) const;

			/// Read a single section.
			/// A user buffer for the data may be supplied. The required size is given
			/// by getSectionHeader(sectionIndex).m_endOffset. If dst is null, memory
			/// is allocated internally.
			/// You should call fixupGlobalReferences() after you are finished
			/// loading and unloading. Also if you use imports or exports in this section,
			/// you will need to update the resource manager.
			/// Note: loading and unloading sections will have undefined results
			/// after the packfile contents have been versioned.
		hkResult loadSection(hkStreamReader* reader, int sectionIndex, void* buf = HK_NULL);

			/// Throw away inter data associated with this section
			/// You should call fixupGlobalReferences() after you are finished
			/// loading and unloading. Also if you use imports or exports in this section,
			/// you will need to update the resource manager.
			/// Note: loading and unloading sections will have undefined results
			/// after the packfile contents have been versioned.
		hkResult unloadSection(int sectionIndex);

			/// Fixup pointers between objects in loaded sections.
			/// Nullifies any pointers to data which has been unloaded. Sets up
			/// pointers to sections which have been loaded.
		hkResult fixupGlobalReferences();

			/// Fixup object virtual tables.
		hkResult finishLoadedObjects( const hkTypeInfoRegistry& finish );

			/// See loadSection.
			/// This is a special case of loadSection() when the stream is already at
			/// the correct offset, as happens when called from loadEntireFile().
			/// No error checking is done because the reader may not implement tell().
		hkResult loadSectionNoSeek(hkStreamReader* reader, int sectionIndex, void* buf = HK_NULL);

			/// Return class name registry that may contain hkClass objects from packfile.
			/// The class name registry may be invalidated if versioning was applied
			/// to packfile contents, so this function must be called again to return
			/// the up to date registry.
		const hkClassNameRegistry* getClassNameRegistry() const;

	protected:

			/// Convert a section tag to a section index.
		virtual int getSectionIndex( SectionTag sectionTag ) const;

			/// Get data by physical index.
			/// Returns null if section is not loaded.
		virtual void* getSectionDataByIndex( int sectionIndex, int offset=0 ) const;

		void* getOriginalContents() const;
		const char* getOriginalContentsClassName() const;
			
		struct PackfileObject
		{
			void* object;
			int section;
			int offset;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkBinaryPackfileReader::PackfileObject );
		};
			/// Get original metadata hkClass locations in the packfile sections.
			/// Any versioning steps previously applied to the packfile contents are ignored.
		void findClassLocations( hkArray<PackfileObject>& objectsOut ) const;

			/// Replace all hkClass packfile pointers with hkClass
			/// pointers from the provided registry.
		void useClassesFromRegistry(const hkClassNameRegistry& registry) const;

	private:

		class BinaryPackfileData : public hkPackfileData
		{
			public:
				HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				BinaryPackfileData() {}
				void freeSection(void* p);
				void untrackAndDestructObject( void* o );
		};

			// Our allocations
		BinaryPackfileData* m_packfileData;
			// The overall header or null if not read yet.
		hkPackfileHeader* m_header;
			// Array of section headers. See m_header->m_numSections.
		hkPackfileSectionHeader* m_sections;
			// Pointers to sectiondata.
		hkInplaceArray<void*,16> m_sectionData;
			// Offset of packfile from start of stream.
		int m_streamOffset;
			// Lazily create this array from index.
		mutable hkArray<hkVariant>* m_loadedObjects;
			// Lazily create the tracker if needed.
		mutable hkBinaryPackfileUpdateTracker* m_tracker;
			//
		mutable hkRefPtr<hkChainedClassNameRegistry> m_packfileClassRegistry;
};

#endif // HK_BINARY_PACKFILE_READER_H

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
