/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_VERSION_REPORTER
#define HK_VISUALIZE_VERSION_REPORTER

class hkStreamWriter;
class hkStreamReader;
class hkDisplaySerializeIStream;

// The first version with support for sending compiler strings as part of the version message.
#define HK_VERSION_SUPPORTS_COMPILER_STRING 7001
// The first version where the server expects to receive a display handler version message after connection.
#define HK_VERSION_SUPPORTS_DISPLAY_HANDLER_SETUP_COMMAND 10100
// The first version with support for caching geometries at the client-side.
#define HK_VERSION_SUPPORTS_CLIENTSIDE_GEOMETRY_CACHING 10100

/// This class manages version information between the server and client.
/// Currently the protocol version is the same as the build number and
/// for a client to work with a server they must have the same version
/// number. Please always use the visual debugger client that shipped
/// with the packages you received.
class hkVersionReporter 
{
	public:
			/// Sends a data 'chunk' which contains the version information
			/// of the Havok libs that the senders process/executable is
			/// linked with.
		static hkResult HK_CALL sendVersionInformation( hkStreamWriter* connection );

			/// Read a 'chunk' containing the version info of the other end of the
			/// connection.
		static hkResult HK_CALL receiveVersionInformation( hkStreamReader* connection, int& protocolVersion, int& protocolMinimumCompatible, hkStringPtr& platformString, hkStringPtr& compilerString );
		static hkResult HK_CALL receiveVersionInformation( hkDisplaySerializeIStream& chunkStream, int& protocolVersion, int& protocolMinimumCompatible, hkStringPtr& platformString, hkStringPtr& compilerString );

	public:

		static int m_protocolVersion;
		static int m_protocolMinimumCompatible;
};

#endif // HK_VISUALIZE_VERSION_REPORTER

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
