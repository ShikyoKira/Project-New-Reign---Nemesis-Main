/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_PACKFILE_WRITER_H
#define HK_PACKFILE_WRITER_H

#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>
#include <Common/Serialize/Util/hkSerializeMultiMap.h>
#include <Common/Serialize/Util/hkStructureLayout.h>

class hkClass;
class hkClassEnum;
class hkVtableClassRegistry;

/// Write a reflected object and its references recursively.
/// A packfile contains a single "contents" object and all objects
/// pointed to from the contents object recursively. i.e., every object
/// in the file is reachable from the contents object.
/// The file is built in two passes. First all objects are traversed
/// and a list of pending writes is created. You can intercept the
/// object traversal with the AddObjectListener.
/// When all object dependencies are resolved, the list is traversed
/// again and the objects written.
class hkPackfileWriter : public hkReferencedObject
{
	public:

			/// Called when an object is about to be added to the pending writes.
		class AddObjectListener : public hkReferencedObject
		{
			public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				typedef const void* ObjectPointer;
				typedef const hkClass* ClassPointer;

					/// Called when an object is about to be added.
					/// The implementation may modify objP, klassP
					/// to point to a different object/type if required.
					/// If opjP is set to NULL, the object is skipped.
					/// NOTE: if you change the object or class pointer, you
					/// must ensure that the object is valid until save() is
					/// called. Typically the listener holds a list these objects
					/// to be destroyed in its destructor.
				virtual void addObjectCallback( ObjectPointer& objP, ClassPointer& klassP ) = 0;
		};

		/// Options to fine tune writing.
		struct Options
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkPackfileWriter::Options);
			Options()
				:	m_userTag(0),
				m_writeMetaInfo(false),
				m_writeSerializedFalse(false),
				m_contentsVersion(HK_NULL)
			{
			}
			/// An optional tag for this file.
			/// You can use this tag to identify different types of file,
			/// examining only the file header.
			hkUint32 m_userTag;
			/// For binary writes. Defaults to the current host layout.
			hkStructureLayout m_layout;
			/// Whether or not to save the hkClass for each object.
			hkBool m_writeMetaInfo;
			/// Should serialized=false members be written
			hkBool m_writeSerializedFalse;
			/// Override the saved version string. Use with caution.
			const char* m_contentsVersion;
		};


		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE);

		hkPackfileWriter(const Options& options = Options());

		~hkPackfileWriter();

			/// Set the object "obj" as the contents of this file.
			/// When a pointer to a polymorphic type is encountered, the
			/// class registry is used to find the exact type. The registry
			/// may be null if there are no pointers to polymorphic types.
			/// If a type is encountered which is not in the registry, the
			/// pointer is saved as null and the object ignored.
			/// Packfile writers cannot be reused. Use a new packfile writer
			/// for each setContents call.
		virtual void setContentsWithRegistry(
			const void* object, const hkClass& klass,
			const hkVtableClassRegistry* registry,
			AddObjectListener* addListen=HK_NULL);

			/// Set the object "obj" as the contents of this file.
			/// Calls setContentsWithRegistry with the registry in hkBuiltinTypeRegistry.
		virtual void setContents(
			const void* object, const hkClass& klass,
			AddObjectListener* addListen=HK_NULL);

			/// Don't save "object", but generate an import instead.
			/// When "object" is referenced by this packfile but is created
			/// somewhere else, you can use this method to prevent the
			/// object being saved. At load time you can reconnect using
			/// the supplied id. See also hkPackfileData::getImportsExports()
		virtual void addImport( const void* object, const char* id );

			/// Make "object" visible outside this packfile.
			/// You can give objects identifiers which are accessible
			/// through hkPackfileData::getImportsExports().
		virtual void addExport( const void* object, const char* id );

			/// Save the contents to the given stream with the given options.
			/// You may call save several times with different options after
			/// a single call to setContents.
		virtual hkResult save( hkStreamWriter* stream, const Options& options ) = 0;

		//
		// Section control
		//

			/// Section tags are short strings.
		typedef const char* SectionTag;

			/// The default section for normal object data.
		static SectionTag SECTION_TAG_DATA;

			/// The default section for hkClass instances.
		static SectionTag SECTION_TAG_TYPES;

			/// Add a section to the file.
			/// Sections are written in the order that they are
			/// added, so you can control the file layout with multiple
			/// calls to addSection.
		virtual void addSection(SectionTag sectionTag);

			/// If pointer is encountered, save it to section "sectionTag".
			/// See also setSectionForClass.
		virtual void setSectionForPointer( const void* ptr, SectionTag sectionTag );

			/// Save objects of type "k" to section "sectionTag".
			/// When choosing the section for an object, we first check for
			/// an explicit override (setSectionForPointer) then check for a
			/// match by type including parent types.
		virtual void setSectionForClass( const hkClass& k, SectionTag sectionTag );

			/// Write the current version into the given buffer.
		static void HK_CALL getCurrentVersion(char* buf, int bufLen);

		struct PendingWrite
		{
			const void* m_pointer; // data pointer.
			const hkClass* m_klass; // hkClass pointer or NULL for raw.
			const void* m_origPointer; // pointer before addObjectListener
			const hkClass* m_origClass; // class before addObjectListener
			int m_sectionIndex;
			int m_dataSize; // set for raw data only.

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkPackfileWriter::PendingWrite );
		};

	protected:

		enum { INDEX_IGNORE = -1, INDEX_IMPORT = -2, INDEX_ERROR = -3 };

		hkUint32 findSectionFor( const void* obj, const hkClass& k, SectionTag tag );
		void addObject(          const void* obj, const hkClass& k, const hkVtableClassRegistry* classRegistry, AddObjectListener* addListener, SectionTag sectionTag);
		void addPendingWrite( const void* obj, const hkClass& k, const void* objo, const hkClass& ko, SectionTag sectionTag);
		int notDuplicateMetaData(const void* pointer, const hkClass* klass);
		hkUint32 sectionTagToIndex( SectionTag sectionTag );

	protected:

		hkArray<PendingWrite> m_pendingWrites;
		hkPointerMap<const void*, int> m_knownObjects; // address -> pending write index
		hkPointerMap<const void*, const char*> m_imports; // address -> symbol name
		hkPointerMap<const void*, const char*> m_exports; // address -> symbol name
		hkStringMap<const hkClass*> m_knownClasses; // name -> class pointer
		hkStringMap<const hkClassEnum*> m_knownEnums; // name -> enumpointer
		hkPointerMap<const void*, const void*> m_replacements; // original -> replacement
		hkArray<char*> m_knownSections;
		hkStringMap<int> m_sectionTagToIndex;
		hkArray<hkVariant> m_objectsWithUnregisteredClass;

		hkInt32 m_contentsPtrPWIndex; // index into the pending writes of the contents ptr
		hkInt32 m_contentsClassPWIndex;// index into the pending writes of the contents class
		int m_numDataInstances;
		int m_numClassInstances;

		hkPointerMap<const void*, hkUint32> m_sectionOverrideByPointer;
		hkStringMap<hkUint32> m_sectionOverrideByType;

		hkSerializeMultiMap<const void*, int> m_pwIndexesFromReferencedPointer; // list of pending write indexes of pointers referenced by key, like a typological map
		Options m_startOptions;
};

#endif // HK_PACKFILE_WRITER_H

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
