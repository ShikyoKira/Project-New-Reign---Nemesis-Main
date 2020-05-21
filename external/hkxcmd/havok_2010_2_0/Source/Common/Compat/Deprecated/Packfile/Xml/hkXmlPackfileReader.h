/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_XML_PACKFILE_READER_H
#define HK_XML_PACKFILE_READER_H

#include <Common/Compat/Deprecated/Packfile/hkPackfileReader.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>

class hkRelocationInfo;
class hkChainedClassNameRegistry;
class hkXmlPackfileUpdateTracker;
template <typename K, typename V, typename Allocator> class hkPointerMap;

/// Reads an XML packfile.
class hkXmlPackfileReader : public hkPackfileReader
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			///
		hkXmlPackfileReader();

			///
		~hkXmlPackfileReader();

			/// Load the entire file, fixes up all pointers.
			/// If the root element packfile has a "classversion" attribute,
			/// we use the hkVersionRegistry to find the metadata for this version.
			/// Use a new packfile reader for each loadEntireFile call.
		virtual hkResult loadEntireFile( hkStreamReader* reader );

			/// Load the entire file, fixes up all pointers.
			/// If classes are present in the file they are used, falling
			/// back to the supplied registry if not found. See also loadEntireFile.
			/// Use a new packfile reader for each loadEntireFileWithRegistry call.
		virtual hkResult loadEntireFileWithRegistry( hkStreamReader* reader, const hkClassNameRegistry* reg );

			// Inherited from hkPackfileReader
		virtual void* getContentsWithRegistry( const char* className, const hkTypeInfoRegistry* finish );

			// Inherited from hkPackfileReader
		virtual const char* getContentsClassName() const;

			// Inherited from hkPackfileReader
		virtual hkPackfileData* getPackfileData() const;

			// Inherited from hkPackfileReader
		virtual hkArray<hkVariant>& getLoadedObjects() const;

			// Inherited from hkPackfileReader
		virtual hkVariant getTopLevelObject() const;

			// Inherited from hkPackfileReader
		virtual hkObjectUpdateTracker& getUpdateTracker() const;

		const hkClassNameRegistry* getClassNameRegistry() const;

	protected:

		const hkClass* getClassByName( const char* className, hkStringMap<hkClass*>& partiallyLoadedClasses, hkPointerMap<const hkClass*, int>& offsetsRecomputed, int classVersion, const char* contentsVersion ) const;
		void handleInterObjectReferences( const char* objName, void* object, const hkRelocationInfo& reloc, const hkStringMap<void*>& nameToObject, hkStringMap<int>& unresolvedReferences );

			/// Replace all hkClass packfile pointers with hkClass
			/// pointers from the provided registry.
		void useClassesFromRegistry(const hkClassNameRegistry& registry) const;

	protected:

			// Contains all data which may persist after this reader is destroyed.
		AllocatedData* m_data;

			// Names (tags) of sections in the order they appear.
		hkArray<const char*> m_knownSections;
			// Maps section tag to physical index. (see m_knownSections).
		hkStringMap<int> m_sectionTagToIndex;

			// Loaded non-class objects
		mutable hkArray<hkVariant> m_loadedObjects;

			// Tracker
		hkXmlPackfileUpdateTracker* m_tracker;

			// Classes
		mutable hkRefPtr<hkChainedClassNameRegistry> m_packfileClassRegistry;
};

#endif // HK_XML_PACKFILE_READER_H

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
