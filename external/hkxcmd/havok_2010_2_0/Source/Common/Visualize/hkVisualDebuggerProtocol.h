/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUAL_DEBUGGER_PROTOCOL_H
#define HK_VISUAL_DEBUGGER_PROTOCOL_H

/// This namespace collects the command bytes which are used to distinguish messages between the
/// client and server.
namespace hkVisualDebuggerProtocol
{
		/// Commands sent from the server to the client.
		/// These must be distinct from the BidirectionalCommands.
	enum ServerToClientCommands
	{
		//
		// Debug display handler
		//

		HK_STEP = 0x00,
		HK_ADD_GEOMETRY,
		HK_UPDATE_GEOMETRY,
		HK_SET_COLOR_GEOMETRY,
		HK_REMOVE_GEOMETRY,
		HK_DISPLAY_GEOMETRY_WITH_TRANSFORM,
		HK_DISPLAY_GEOMETRY,
		HK_DISPLAY_POINT,
		HK_DISPLAY_LINE,
		HK_DISPLAY_TEXT,	// These may move into a separate interface later

		HK_SEND_STATISTICS_DUMP,

		HK_HOLD_IMMEDIATE,

		HK_ADD_CAMERA,
		HK_UPDATE_CAMERA,
		HK_REMOVE_CAMERA,

		HK_SEND_MEMSTATS_DUMP,  // Added for 3.0.0
		HK_ADD_GEOMETRY_INSTANCE, // Added for 4.5.0
		HK_DISPLAY_TEXT_3D,	// Added for 4.6.0

		HK_UNUSED_0,
		HK_UNUSED_1,

		HK_DISPLAY_ANNOTATION, // Added in 7.0

		HK_ADD_MESH_OBSOLETE,			// Added in 10.1, removed in 10.2
		HK_REMOVE_MESH_OBSOLETE,		// Added in 10.1, removed in 10.2
		HK_UPDATE_GEOMETRY_WITH_SCALE,  // Added in 10.1
		HK_SKIN_GEOMETRY,				// Added in 10.1

		// Support for vdb geometry caching. Added in 10.1
		HK_ADD_GEOMETRY_PART, // When a geometry is sent directly (not via an add-hash & do-request-geometry exchange)
		HK_ADD_GEOMETRY_FINAL, // When a geometry is sent directly (not via an add-hash & do-request-geometry exchange)
		HK_ADD_GEOMETRY_HASH,
		HK_GEOMETRY_FOR_HASH_PART, // Any part except the last.
		HK_GEOMETRY_FOR_HASH_FINAL,	// The last part.		

		HK_DISPLAY_BONE, // Added in 10.2
		HK_SET_TRANSPARENCY_GEOMETRY, // Added in 10.2

		//
		// Object inspection
		//

		HK_SETUP = 0x20,

		//
		// Version information exchange.
		//

		HK_VERSION_INFORMATION = 0x90,
		HK_SERVER_LAYOUT,

		//
		// Process registration.
		//

		HK_REGISTER_PROCESS = 0xC0, 
		HK_SELECT_PROCESS = 0xC1,
		

		HK_PAUSE_WORLD_STEP = 0xC4,   // these are here to avoid dependency of VDB frontend on Destruction
		HK_UNPAUSE_WORLD_STEP = 0xC5,

		//
		// World snapshot.
		//

		HK_SNAPSHOT = 0xD0,

		MEMORY_SNAPSHOT = 0xD1,

	};

		/// Commands sent in both directions.
		/// These must be distinct from both the ServerToClientCommands and the ClientToServerCommands.
	enum BidirectionalCommands
	{
		//
		// Object inspection
		//
		HK_ADD_TOPLEVEL = 0x21, // Add a top level object (two ids, one for object and one for the class) (server->client)
		HK_REMOVE_TOPLEVEL, // Remove a top level object (one id, just the object) (server->client)
		HK_REQUEST_OBJECT, // Request the data for a given object id (client->server)
		HK_ADD_OBJECT, // New object (can be a class) data or just refreshed. (bidirectional potentially)
		HK_UPDATE_MEMBER, // Update the data for a given member. Sent over is the object id, along with enoygh nformation to odentify the exact member item (down to a data item)		

		HK_LIVE_OBJECT = 0x40,
	};

		/// Commands sent from the client to the server.
		/// These must be distinct from the BidirectionalCommands.
	enum ClientToServerCommands
	{
		//
		// Command router.
		//
		COMMAND_STEP = 0x00, // sent from Server to client on step

		//
		// Client-side caching
		//
		COMMAND_REQUEST_GEOMETRY_WITH_HASH = 0x30,
		COMMAND_DO_NOT_REQUEST_GEOMETRY_WITH_HASH = 0x31,
		COMMAND_CLIENT_DISPLAY_HANDLER_SETTINGS = 0x32,		

		//
		// Mouse picking
		//
		HK_PICK_OBJECT = 0xB0,
		HK_DRAG_OBJECT = 0xB1,
		HK_RELEASE_OBJECT = 0xB2,

		//
		// Process registration
		//
		HK_CREATE_PROCESS = 0xC2,
		HK_DELETE_PROCESS = 0xC3,

		//
		// Command router.
		//
		COMMAND_ACK = 0xF0, // ack back to say full step recvd
	};
}

#endif // HK_VISUAL_DEBUGGER_PROTOCOL_H

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
