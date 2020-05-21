/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CRC_64_STREAM_WRITER_H
#define HK_CRC_64_STREAM_WRITER_H

#include <Common/Base/System/Io/Writer/Crc/hkCrcStreamWriter.h>

/// CRC64
class hkCrc64StreamWriter : public hkCrcStreamWriter<hkUint64, 0xc96c5795d7870f42ull> 
{
public:
HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
	hkCrc64StreamWriter( hkUint64 startCrc = 0 ) : hkCrcStreamWriter<hkUint64, 0xc96c5795d7870f42ull>( startCrc ) {}

	static const hkUint64 g_crc64lookupTable[256];
};

// Specialize the template for crc64 to use the look-up table.
template<>
inline hkUint64 hkCrcStreamWriter<hkUint64, 0xc96c5795d7870f42ull>::calcPermute(hkUint64 p)
{
	return hkCrc64StreamWriter::g_crc64lookupTable[p];
}


#endif // HK_CRC_64_STREAM_WRITER_H

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
