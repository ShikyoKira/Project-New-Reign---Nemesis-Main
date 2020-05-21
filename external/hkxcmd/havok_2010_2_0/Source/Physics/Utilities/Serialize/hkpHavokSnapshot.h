/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_HAVOKSNAPSHOTPACKFILE_H
#define HK_HAVOKSNAPSHOTPACKFILE_H

#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Packfile/hkPackfileWriter.h>

class hkResource;

/// This is a simple way to snapshot (serialize) the whole scene into a file.
/// This can be useful for debugging purposes.
class hkpHavokSnapshot
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpHavokSnapshot);
			/// Converts some objects to different types for snapshots.
			/// Some objects are inherently not serializable because they point to
			/// external memory. i.e., the vertex and index arrays of an hkpMeshShape. When these objects
			/// are encountered, we convert them before saving. i.e hkpMeshShape -> hkpStorageMeshShape.
		class ConvertListener : public hkPackfileWriter::AddObjectListener
		{
			public:
			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				~ConvertListener();
				virtual void addObjectCallback( ObjectPointer& p, ClassPointer& k );
				hkArray<hkReferencedObject*> m_objects;
		};

			/// By default, a binary tagfile snapshot is generated
			/// Settings any of these options overrides this default
		enum SnapshotOptionsBits
		{
			SNAPSHOT_BINARY_TAGFILE = 0, ///< Default, binary tagfile
			SNAPSHOT_TEXT = 1, ///< Output an XML snapshot
			SNAPSHOT_PACKFILE = 2, ///< Output a packfile snapshot
		};

		struct Options : public hkFlags<SnapshotOptionsBits, int>
		{
			Options(SnapshotOptionsBits b = SNAPSHOT_BINARY_TAGFILE) : hkFlags<SnapshotOptionsBits, int>(b) {}
			// Convert from the old option: hkBool binaryFormat
			Options(bool b) : hkFlags<SnapshotOptionsBits, int>(b ? SNAPSHOT_BINARY_TAGFILE : SNAPSHOT_TEXT) {}
		};

			/// Save a snapshot of the world to filename in tagfile form (binary).
			/// Returns true on success
			/// If you don't provide a target layout then the current host layout is assumed.
			/// NOTE: some objects are inherently not serializable because they point to
			/// external memory. i.e., the vertex and index arrays of an hkpMeshShape. When these objects
			/// are encountered, we convert them before saving. i.e hkpMeshShape -> hkpStorageMeshShape.
		static hkBool HK_CALL save(const class hkpWorld* world, hkStreamWriter* writer, Options outputOptions = SNAPSHOT_BINARY_TAGFILE, const hkStructureLayout::LayoutRules* targetLayout = HK_NULL, bool saveContactPoints = false );

			/// Save a snapshot of a hkpPhysicsData to filename.
		static hkBool HK_CALL save( const class hkpPhysicsData* data, hkStreamWriter* writer, Options outputOptions = SNAPSHOT_BINARY_TAGFILE, const hkStructureLayout::LayoutRules* targetLayout = HK_NULL );

			/// Save a snapshot of a given object under a RootLevelContainer to the given stream
		static hkBool HK_CALL saveUnderRootLevel( const void* data, const hkClass& dataClass, hkStreamWriter* writer, Options outputOptions = SNAPSHOT_BINARY_TAGFILE, const hkStructureLayout::LayoutRules* targetLayout = HK_NULL );

			/// Save a snapshot of a given object to filename.
		static hkBool HK_CALL save( const void* data, const hkClass& dataClass, hkStreamWriter* writer, Options outputOptions = SNAPSHOT_BINARY_TAGFILE, const hkStructureLayout::LayoutRules* targetLayout = HK_NULL, hkPackfileWriter::AddObjectListener* userListener = HK_NULL );

			/// Load a snapshot from a filename. It will search
			/// the root level container in the file for a hkpPhysicsData class. It will autodetect
			/// the file type (packfile or tagfile).
			/// NOTE: remember to remove the reference from allocatedData once you are finished using the loaded data.
		static class hkpPhysicsData* HK_CALL load(class hkStreamReader* reader, hkResource** allocatedData);
};

#endif // HK_HAVOKSNAPSHOTSESSION_H

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
