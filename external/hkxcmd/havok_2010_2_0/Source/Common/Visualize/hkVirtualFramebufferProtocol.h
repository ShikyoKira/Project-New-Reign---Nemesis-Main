/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE__VIRTUAL_FRAMEBUFFER_CLIENT_PROTOCOL_H
#define HK_VISUALIZE__VIRTUAL_FRAMEBUFFER_CLIENT_PROTOCOL_H

namespace hkVirtualFramebufferProtocol
{
	enum Commands
	{
		// Server -> Clients
		SEND_FRAMEBUFFER = 1, // followed by sub command

		// Client -> Sever
		SEND_GAMEPAD = 2, // whole gamepad
		SEND_KEY	 = 3, // key up / down event
		SEND_MOUSE   = 4, // whole mouse

		// Change options / settings
		SEND_OPTIONS = 5  // hkVirtualFramebufferServer::RuntimeOptions
	};

	enum FramebufferCommands
	{
		// Framebuffer can be sent in a few eays
		// 1: The whole section, with a rect (can be whole screen)
		FRAMEBUFFER_RECT_RAW = 1, 	

		// 2: A Diff, with a rect for the diff, so can be a sub section of buffer
		FRAMEBUFFER_RECT_DIFF = 2, 		

		// 3: A raw rect, but using simple run length encoding 
		FRAMEBUFFER_RECT_RLE = 3 		
	};

	enum FramebufferFormat
	{
		PIXEL_RGB888 = 1, // 24Bits
		PIXEL_RGB565 = 2, // 16Bits
		PIXEL_PALLETE8 = 3 // 8 bit indexed
	};
}

#endif

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
