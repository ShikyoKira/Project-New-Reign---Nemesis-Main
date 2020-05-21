/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_WIN32STREAMWRITER_H
#define HK_BASE_WIN32STREAMWRITER_H

#include <Common/Base/System/Io/Writer/hkStreamWriter.h>
#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>

class hkWin32StreamWriter : public hkStreamWriter
{
	public:

		hkWin32StreamWriter(const char* fname);
		hkWin32StreamWriter(hkFileSystem::StdStream s);
		void close();
		virtual ~hkWin32StreamWriter();
		virtual int write( const void* buf, int nbytes);
		virtual void flush();
		virtual hkBool isOk() const;
		virtual hkResult seek( int offset, SeekWhence whence);
		virtual int tell() const;

	protected:

		typedef void* HANDLE;
		HANDLE m_handle;
};

typedef hkWin32StreamWriter DefaultFileWriter;

#endif //HK_BASE_WIN32STREAMWRITER_H

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
