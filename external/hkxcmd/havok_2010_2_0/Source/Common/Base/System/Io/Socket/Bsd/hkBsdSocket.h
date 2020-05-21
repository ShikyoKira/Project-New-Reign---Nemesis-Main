/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BASE_BSD_SOCKET_H
#define HK_BASE_BSD_SOCKET_H

#include <Common/Base/hkBase.h>
#include <Common/Base/System/Io/Socket/hkSocket.h>

class hkBsdSocket : public hkSocket
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS );

#		if defined(HK_PLATFORM_X64)
			typedef unsigned __int64 socket_t;
#		elif defined(HK_PLATFORM_WIN32) || defined(HK_PLATFORM_XBOX) || defined(HK_PLATFORM_XBOX360)
			typedef unsigned socket_t;
#		else
			typedef int socket_t;
#		endif

		hkBsdSocket(socket_t s=socket_t(-1));

		virtual ~hkBsdSocket();

		virtual hkBool isOk() const;

		virtual void close();

		virtual int read( void* buf, int nbytes);

		virtual int write( const void* buf, int nbytes);

		// client

		virtual hkResult connect(const char* servername, int portNumber);

		virtual hkResult asyncSelect(void* notificationHandle, hkUint32 message, SOCKET_EVENTS events);

		virtual bool canRead();
		

		// server

		hkResult listen(int port);
		hkSocket* pollForNewClient();


		// Switch between blocking/non-blocking modes if desired
		hkResult setBlocking(hkBool blocking);
		

	protected:

		hkResult createSocket();

		socket_t m_socket;
};

/// Initializes the network for BSD style sockets
void HK_CALL hkBsdNetworkInit();

/// Shuts down the network
void HK_CALL hkBsdNetworkQuit();

#endif // HK_BASE_BSD_SOCKET_H

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
