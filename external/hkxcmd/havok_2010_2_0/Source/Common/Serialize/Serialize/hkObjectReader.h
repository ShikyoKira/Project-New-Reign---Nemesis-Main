/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_OBJECT_READER_H
#define HK_SERIALIZE_OBJECT_READER_H

class hkRelocationInfo;
class hkStreamReader;

/// Interface to read a single object from a stream.
/// Objects read consist of two parts - a relocatable buffer and
/// a relocation info which describes the pointers within the buffer.
/// After the relocations have been applied, the buffer is no longer
/// relocatable.
class hkObjectReader : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE);

			/// Read an object into the supplied buffer.
			/// The buffer must be large enough to hold the object
			/// This method may fail if the buffer size is too small,
			/// there is a read error or parse error.
			/// Returns the size of the buffer used or -1 on error.
		virtual int readObject( hkStreamReader* reader, void* buf, int bufLen, const hkClass& klass, hkRelocationInfo& reloc ) = 0;

			/// Read a raw binary chunk.
		virtual hkResult readRaw( hkStreamReader* reader, void* buf, int bufLen ) = 0;
};

#endif //HK_SERIALIZE_OBJECT_READER_H

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
