/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_SOCKET_H
#define HK_BASE_SOCKET_H

#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>

class hkSocketImpl;

/// A simple platform independent socket.
class hkSocket : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		enum SOCKET_EVENTS {
			SOCKET_NOEVENTS = 0, // register for this only to get back to a blocking socket (default)
			SOCKET_CAN_READ = 1, // Data has arrived to read
			SOCKET_CAN_WRITE = 2, // Space is now available to write
			SOCKET_CONNECT = 4, // A connect has succeeded
			SOCKET_CLOSED = 8, // The socket has closed.
			SOCKET_ALLEVENTS = 0xff // register for all of the above
		};

		static hkSocket* (HK_CALL *create)();
		
			/// Return true if the connection is still live.
		virtual hkBool isOk() const = 0;

			/// Close the connection.
		virtual void close() = 0;

			/// Read at most nbytes into buf. Return number of bytes read.
		virtual int read( void* buf, int nbytes) = 0;

			/// Write at most nbytes from buf. Return number of bytes written.
		virtual int write( const void* buf, int nbytes) = 0;

			/// Connect to host 'remote' at specified port.
		virtual hkResult connect( const char* remote, int port) = 0;

			/// Start listening for connections on the specified port.
		virtual hkResult listen(int port) = 0;

			/// Get notification of network events instead of having to poll for them
		virtual hkResult asyncSelect(void* notificationHandle, hkUint32 message, SOCKET_EVENTS events) { return HK_FAILURE; }

			/// Is there data to read? Normally implemented as a select() with 0 time out for read flags
		virtual bool canRead() { return false; /*safer to assume can't than spin forever on blocking reads*/ }

			/// Check for new connections on a socket which we are listen()ing on.
			/// Returns NULL if there are no new connections.
		virtual hkSocket* pollForNewClient() = 0;

			/// Get a stream reader for this connection.
		hkStreamReader& getReader() { return m_reader; }

			/// Get a stream writer for this connection.
		hkStreamWriter& getWriter() { return m_writer; }

			/// This flag is used to ensure the network is only set up when a socket is created.
			/// Initially set to false this is check in the base hkSocket constructor and when false calls hkSocket::s_platformNetInit
		static hkBool s_platformNetInitialized;

			/// Pointers to default network initialization function.
			/// This is called during hkBaseSystem::init().
			/// Setting this to HK_NULL before hkBaseSystem::init() is called will bypass initialization.
		static void (HK_CALL *s_platformNetInit)(void);

			/// Pointers to default network shutdown function.
			/// This is called during hkBaseSystem::quit().
			/// Setting this to HK_NULL before hkBaseSystem::quit() is called will bypass shutdown.
		static void (HK_CALL *s_platformNetQuit)(void);

	protected:

		hkSocket();

	private:

		class ReaderAdapter : public hkStreamReader
		{
			public:
				HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				ReaderAdapter() : m_socket(HK_NULL) { }
				virtual int read( void* buf, int nbytes );
				virtual hkBool isOk() const;

				hkSocket* m_socket;
		};

		class WriterAdapter : public hkStreamWriter
		{
			public:
				HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
				WriterAdapter() : m_socket(HK_NULL) { }
				virtual int write( const void* buf, int nbytes );
				virtual hkBool isOk() const;

				hkSocket* m_socket;
		};

		ReaderAdapter m_reader;
		WriterAdapter m_writer;
};

#endif // HK_BASE_SOCKET_H

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
