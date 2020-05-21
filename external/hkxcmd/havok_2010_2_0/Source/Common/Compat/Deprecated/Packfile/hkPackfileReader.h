/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_PACKFILE_READER_H
#define HK_PACKFILE_READER_H

#include <Common/Serialize/Packfile/hkPackfileData.h>

class hkStreamReader;
class hkObjectUpdateTracker;
class hkClassNameRegistry;
class hkTypeInfoRegistry;
template <typename T, typename A> class hkStringMap;

/// Base interface to read in a packed file.
/// Reading is done two steps - first the file is read
/// and reconstructed in memory (loadEntireFile). Secondly
/// versioning, pointer fixups and finishing functions
/// are run (getContents).
class hkPackfileReader : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE);

		hkPackfileReader();

		~hkPackfileReader();

			/// Simple interface. Load the entire file, fixes up all pointers.
			/// This method internally allocates memory for storing the packfile.
			/// hkBinaryPackfileReader has finer grained loading operations, and
			/// also a method which allows loading with no memory allocations.
			/// hkXmlPackfile reader needs class descriptions to load the objects.
			/// We look for class descriptions in two places : first the file itself
			/// and secondly in the hkBuiltinTypeRegistry. See also
			/// hkXmlPackfileReader::loadEntireFileWithRegistry.
		virtual hkResult loadEntireFile( hkStreamReader* reader ) = 0;

			/// Get the top level object in the file.
			/// This function assumes that the packfile version matches the in-memory
			/// version. You may use hkVersionUtil if this is not the case. See the
			/// serialization section of the manual for more details.
			/// Usually you will use getContents instead of this function.
			/// If the packfile contents type do not match the expected class type
			/// this method will return HK_NULL. You may pass HK_NULL as the expected
			/// class to disable this check.
			/// Some objects will require a finishing step. i.e., to initialize
			/// members which have not been serialized or to initialize vtables.
			/// If "finish" is not HK_NULL, apply these finishing steps.
		virtual void* getContentsWithRegistry( const char* expectedClassName, const hkTypeInfoRegistry* finish ) = 0;

			/// Get the top level object in the file.
			/// Calls getContentsWithRegistry with the finish registry from hkBuiltinTypeRegistry.
		virtual void* getContents( const char* expectedClassName );

			/// Get the class name of contents, if available.
		virtual const char* getContentsClassName() const = 0;

			//
			// Versioning access functions.
			//

			/// Get all objects in the packfile.
			/// Used by the versioning code. Note that if a packfile has been
			/// stripped of metadata, then this array may be empty.
		virtual hkArray<hkVariant>& getLoadedObjects() const = 0;

			/// Get top level object in the packfile.
			/// Used by the versioning code.
		virtual hkVariant getTopLevelObject() const = 0;

			/// Get a tracker which knows about the internal structure of the packfile.
		virtual hkObjectUpdateTracker& getUpdateTracker() const = 0;

			/// Get a string representing the current version of objects within the file.
			/// E.g. "Havok-4.1.0".
		virtual const char* getContentsVersion() const;

			/// Label the packfile as containing the given version.
		virtual void setContentsVersion(const char* version );

		virtual const hkClassNameRegistry* getClassNameRegistry() const = 0;

	public:

			/// Get a handle to memory allocations made during loading.
			/// You can add a reference to this, and delete the pack file reader after loading.
			/// Removing your reference will deallocate any loaded data.
			/// IMPORTANT: Make sure that the pack file content is versioned before you call this function.
		virtual hkPackfileData* getPackfileData() const = 0;

			// compatibility
		typedef hkPackfileData AllocatedData;
		AllocatedData* getAllocatedData() const { return getPackfileData(); }

			/// Return true if the SDK version matches the packfile version.
		hkBool32 isVersionUpToDate() const;
		void warnIfNotUpToDate() const;

	public:

			///
		typedef const char* SectionTag;
		typedef hkPointerMap<const hkClass*, hkInt32> UpdateFlagFromClassMap;

		mutable UpdateFlagFromClassMap m_updateFlagFromClass;

			// Current contents version.
		char* m_contentsVersion;

			///
		static void HK_CALL updateMetaDataInplace( hkClass* classInOut, int fileVersion, const char* contentsVersion, UpdateFlagFromClassMap& updateFlagFromClass );
};

#endif // HK_PACKFILE_READER_H

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
