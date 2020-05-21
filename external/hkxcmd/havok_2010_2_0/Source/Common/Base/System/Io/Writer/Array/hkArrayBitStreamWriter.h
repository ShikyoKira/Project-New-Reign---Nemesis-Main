/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_ARRAY_BIT_STREAM_WRITER_H
#define HKBASE_ARRAY_BIT_STREAM_WRITER_H

#include <Common/Base/System/Io/Writer/hkBitStreamWriter.h>

class hkArrayBitStreamWriter : public hkBitStreamWriter
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_STREAM);

	public:

		hkArrayBitStreamWriter(hkArray<hkUint8>& writeDestination);

		/// Writes a number of bits from the given buffer. Returns the number of
		/// bits successfully written
		virtual int write(const void* buf, int numBits);

		HK_FORCE_INLINE int writeInt(int inValue, int numBits)
		{
			return write(&inValue, numBits);
		}

	protected:

		hkArray<hkUint8>* m_destination;	///< Array holding the written data
		int m_byteOffset;	///< Current byte offset inside the array
		int m_bitOffset;	///< Current bit offset inside the array
};

#endif	//	HKBASE_ARRAY_BIT_STREAM_WRITER_H

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
