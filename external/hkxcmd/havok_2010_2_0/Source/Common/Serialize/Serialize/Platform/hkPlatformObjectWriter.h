/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_PLATFORM_OBJECT_WRITER_H
#define HK_SERIALIZE_PLATFORM_OBJECT_WRITER_H

#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Serialize/hkObjectWriter.h>
#include <Common/Serialize/Copier/hkObjectCopier.h>

class hkOArchive;
class hkObjectCopier;

/// Write an object in the native format of a given platform.
class hkPlatformObjectWriter : public hkObjectWriter
{
public:

	class Cache : public hkReferencedObject
	{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(hkPlatformObjectWriter::Cache);

		Cache();
		virtual ~Cache();

		hkClass* get( const hkClass* klass, const hkStructureLayout& layout );

		hkPointerMap<const void*, void*> m_platformClassFromHostClass;
		hkPointerMap<const hkClass*, int> m_platformClassComputed;
		hkArray<void*> m_allocations;

		Cache(hkFinishLoadedObjectFlag f) : m_allocations(f) {}
	};

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Create an hkPlatformObjectWriter.
	hkPlatformObjectWriter(const hkStructureLayout& layout, class Cache* cache = HK_NULL, hkObjectCopier::ObjectCopierFlags flags = hkObjectCopier::FLAG_RESPECT_SERIALIZE_IGNORED);

			/// Destroy an hkPlatformObjectWriter.
		virtual ~hkPlatformObjectWriter();

			/// Save object data, using class information klass.
		virtual hkResult writeObject(hkStreamWriter* writer, const void* data, const hkClass& klass, hkRelocationInfo& reloc );

			/// Write a raw binary chunk.
		virtual hkResult writeRaw( hkStreamWriter* writer, const void* data, int dataLen );

			/// Get the layout this writer was created with.
		const hkStructureLayout& getLayout() const;

	
	protected:

		hkObjectCopier* m_copier;
		Cache* m_cache;
};

#endif //HK_SERIALIZE_PLATFORM_OBJECT_WRITER_H

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
