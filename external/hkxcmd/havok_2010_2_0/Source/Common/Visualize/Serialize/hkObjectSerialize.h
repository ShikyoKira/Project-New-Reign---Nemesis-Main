/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VISUALIZE_OBJECT_SERIALIZE_H
#define HK_VISUALIZE_OBJECT_SERIALIZE_H

#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Common/Serialize/Serialize/Platform/hkPlatformObjectWriter.h>
#include <Common/Serialize/Copier/hkObjectCopier.h>

class hkStreamReader;
class hkStreamWriter;
class hkDisplaySerializeIStream;
class hkDisplaySerializeOStream;

/// A utility class that can serialize in and out classes
/// from a stream, and store the created
class hkObjectSerialize 
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkObjectSerialize );
		struct LocalFixup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkObjectSerialize::LocalFixup );

			int fromOffset;
			int toOffset;
		};

		struct GlobalFixup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkObjectSerialize::GlobalFixup );

			int fromOffset;
			hkUint64 toID; 
			hkUint64 toClassID;
		};

	public:
		
			/// Will stream the object to the writer.
			/// Returns the total number of bytes written (-1 for error).
			/// Write object does not add to the held mappings in any way, 
			/// and is a static utility class. DataID is the ptr value on
			/// the server side usually.
			/// If you want to inspect or store the global fix-ups that where sent 
			/// (i.e.: items (both classes and objects) that are queriable 
			/// by the client you have written to). When a VDB requests an object by
			/// ID the only place it can meaningfully come from is a data ptr that 
			/// has been sent in this global fix-up list.
			/// cache allows a hkPlatformObjectWriter::Cache to persist across multiple
			/// writes which may be necessary if the data is shared between multiple objects
		static int HK_CALL writeObject( hkStreamWriter* s,
						const hkStructureLayout& destLayout,
						hkUint64 dataID,
						const void* data,
						const hkClass& klass,
						hkArray<GlobalFixup>& globalFixups,
						hkPlatformObjectWriter::Cache* cache = HK_NULL,
						hkObjectCopier::ObjectCopierFlags flags = hkObjectCopier::FLAG_RESPECT_SERIALIZE_IGNORED);

			/// It assumes the form is the same as writeObject produces.
			/// It will allocate and return a 64bit id for the data
			/// which is the same as the ptr on the server platform
			/// Returns the id of the object read in.
			/// 0 is not a valid id, and is returned on error.
			/// klassID is set to the exact klassID of the data read in.
		static hkUint64 HK_CALL readObject( hkStreamReader* s, void*& data, hkUint32& dataSize, hkArray<GlobalFixup>& globalFixups, hkUint64& klassID );

			/// Write an object (optionally recursively) to an hkDisplaySerializeOStream.
		static int HK_CALL writeObject(	hkDisplaySerializeOStream* s,
						hkUchar preObjectToken,
						const void* data,
						const hkClass& klass,
						bool writePacketSize,
						bool recur,
						bool writeClass,
						const hkVtableClassRegistry* classRegistry = HK_NULL, hkPlatformObjectWriter::Cache* cache = HK_NULL,
						hkObjectCopier::ObjectCopierFlags flags = hkObjectCopier::FLAG_RESPECT_SERIALIZE_IGNORED);

			/// Write an object (optionally recursively) to an hkDisplaySerializeOStream. Any objects contained in writtenObjects
			/// are not written, and written objects are added to it.
		static int HK_CALL writeObject(	hkDisplaySerializeOStream* s,
						hkUchar preObjectToken,
						const void* data,
						const hkClass& klass,
						hkPointerMap<const void*, const hkClass*>& writtenObjects,
						bool writePacketSize,
						bool recur,
						bool writeClass,
						const hkVtableClassRegistry* classRegistry = HK_NULL, hkPlatformObjectWriter::Cache* cache = HK_NULL,
						hkObjectCopier::ObjectCopierFlags flags = hkObjectCopier::FLAG_RESPECT_SERIALIZE_IGNORED);
												
			/// Writes object to stream. Set writePacketSize to true while writing data from server to client. Set
			/// writePacketSize to false while writing client to server.
		static void HK_CALL writeObject( hkDisplaySerializeOStream* stream,
						hkReferencedObject* object,
						bool writePacketSize,
						bool writePackfile,
						hkStructureLayout layout = hkStructureLayout::MsvcWin32LayoutRules,
						hkPlatformObjectWriter::Cache* cache = HK_NULL,
						hkObjectCopier::ObjectCopierFlags flags = hkObjectCopier::FLAG_RESPECT_SERIALIZE_IGNORED);
		
			/// Reads an object from stream.  Returns HK_NULL if the object could not be read.
		static hkReferencedObject* HK_CALL readObject( hkDisplaySerializeIStream* stream, hkSerializeUtil::ErrorDetails& errorDetails );

	protected: // a static class / namespace

		hkObjectSerialize() {} 
};

#endif //  HK_VISUALIZE_OBJECT_SERIALIZE_H

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
