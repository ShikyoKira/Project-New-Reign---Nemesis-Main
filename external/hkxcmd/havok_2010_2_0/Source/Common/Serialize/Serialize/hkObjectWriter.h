/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_OBJECT_WRITER_H
#define HK_SERIALIZE_OBJECT_WRITER_H

class hkRelocationInfo;

/// Interface to write a single object to a stream.
class hkObjectWriter : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE);

			/// Save object data, using class information klass.
			/// Relocation info is added to relocOut.
		virtual hkResult writeObject( hkStreamWriter* writer, const void* data, const hkClass& klass, hkRelocationInfo& relocOut ) = 0;

			/// Write a raw binary chunk.
		virtual hkResult writeRaw( hkStreamWriter* writer, const void* data, int dataLen ) = 0;
};

#endif //HK_SERIALIZE_OBJECT_WRITER_H

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
