/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BASE_STDIOSTREAMREADER_H
#define HK_BASE_STDIOSTREAMREADER_H

#include <Common/Base/Fwd/hkcstdio.h>
#include <Common/Base/System/Io/Reader/Seekable/hkSeekableStreamReader.h>
#include <Common/Base/Fwd/hkcstdio.h>

using namespace std;

class hkStdioStreamReader : public hkSeekableStreamReader
{
public:
	hkStdioStreamReader( const char* nameIn );
	virtual ~hkStdioStreamReader();
	virtual int read( void* buf, int nbytes);	
	virtual hkBool isOk() const;
	virtual hkBool markSupported() const;
	virtual hkBool seekTellSupported() const;
	virtual hkResult seek( int offset, SeekWhence whence);
	virtual int tell() const;

	FILE* m_handle;
	hkBool m_isOk;
};

typedef hkStdioStreamReader DefaultFileReader;

#endif //HK_BASE_STDIOSTREAMREADER_H

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
