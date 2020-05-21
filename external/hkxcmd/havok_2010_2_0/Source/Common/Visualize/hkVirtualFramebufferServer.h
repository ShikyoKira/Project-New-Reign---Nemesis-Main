/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE__VIRTUAL_FRAMEBUFFER_SERVER
#define HK_VISUALIZE__VIRTUAL_FRAMEBUFFER_SERVER

static const hkUint32 HK_VIRTUAL_FRAMEBUFFER_SERVER_PORT = 26001;
static const hkUint32 HK_VIRTUAL_FRAMEBUFFER_CURRENT_PROTOCOL_VER = 1;

#include <Common/Visualize/hkVirtualFramebufferProtocol.h>

//
// Screen 
//

class hkVirtualFramebufferRelativeRect
{

public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkVirtualFramebufferRelativeRect );

	hkVirtualFramebufferRelativeRect()
	{
		setToAll();
	}

	void setToAll()
	{
		m_startX = 0.f;
		m_endX = -1.f;
		m_startY = 0.f;
		m_endY = -1.f;
	}

	void normalize()
	{
		m_startX = hkMath::clamp<float>(m_startX,0,1);
		m_startY = hkMath::clamp<float>(m_startY,0,1);
	
		if (m_endX < 0)
			m_endX = -1;
		else
			m_endX = hkMath::clamp<float>(m_endX,0,1);
		
		if (m_endY < 0)
			m_endY = -1;
		else
			m_endY = hkMath::clamp<float>(m_endY,0,1);
	}

	hkVirtualFramebufferRelativeRect& operator+=(const hkVirtualFramebufferRelativeRect& o)
	{
		if (o.m_startX < m_startX) m_startX = o.m_startX;
		if (o.m_startY < m_startY) m_startY = o.m_startY;
		if ((o.m_endX > m_endX) || (o.m_endX < 0)) m_endX = o.m_endX;
		if ((o.m_endY > m_endY) || (o.m_endY < 0)) m_endY = o.m_endY;
		return *this;
	}

	bool operator==(const hkVirtualFramebufferRelativeRect& o) const
	{
		return  (o.m_startX == m_startX) ||
				(o.m_startY == m_startY) ||
				(o.m_endX == m_endX) ||
				(o.m_endY == m_endY);
	}

	// These are((o.m_height > m_height) in the range 0..1 where 0 == start
	// Y is from lower row, so invert if you are using top row as first etc
	float m_startX;
	float m_startY;
	float m_endX;// -1 == all
 	float m_endY;// -1 == all
};

class hkVirtualFramebuffer
{
public:
	
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkVirtualFramebuffer );

	enum DataType
	{
		DATA_RGB = 0, // (fastest mode, as transport form)
		DATA_BGR = 1
	};

	enum RowOrder
	{
		DATA_TOP_LEFT = 0, // First row is top
		DATA_LOWER_LEFT = 1 // first row is bottom (fastest mode, as transport form)
	};

	DataType m_format;
	RowOrder m_rowOrder;

	// The size of the complete buffer, which this only may be a subsection of
	hkUint32 m_fullBufferWidthInPixels;  
	hkUint32 m_fullBufferHeightInPixels;

	// the subsection the data points too
	const hkUint8* m_data;
	hkUint32 m_dataSizeInBytes; // should match the dimensions below
	hkUint32 m_startX; // start offset in the full buffer
	hkUint32 m_startY; // In m_rowOrder space
	hkInt32 m_endX;   //-1 to end
	hkInt32 m_endY;   //-1 to end. In m_rowOrder space
		
	hkUint32 m_pixelStrideInBytes; // fastest mode is when this is 3
	hkUint32 m_rowPitchInBytes; // for this sub rect

	
};

//
// Gamepad
//

class hkVirtualGamepad
{
public:
	
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkVirtualGamepad );

	enum Button
	{
		PAD_BUTTON_0		= (1),
		PAD_BUTTON_1		= (1<<1),
		PAD_BUTTON_2		= (1<<2),
		PAD_BUTTON_3		= (1<<3),
		PAD_DPAD_UP			= (1<<4),
		PAD_DPAD_DOWN		= (1<<5),
		PAD_DPAD_LEFT		= (1<<6),
		PAD_DPAD_RIGHT		= (1<<7),
		PAD_SELECT			= (1<<8),	
		PAD_START			= (1<<9),
		PAD_BUTTON_L1		= (1<<10),
		PAD_BUTTON_R1		= (1<<11),
		PAD_BUTTON_L2		= (1<<12),
		PAD_BUTTON_R2		= (1<<13),
		PAD_BUTTON_LSTICK   = (1<<14),
		PAD_BUTTON_RSTICK   = (1<<15)
	};

	struct Stick
	{
		float x;
		float y;
	};

	struct Trigger
	{
		float z;
	};

	hkEnum<Button,hkUint32> m_buttons;	
	Stick   m_sticks[2];
	Trigger m_triggers[2];
	hkUint32 m_gamePadNum; //0..4 etc
};

class hkVirtualGamepadHandler
{
public:
	//+hk.MemoryTracker(ignore=True)
	
	virtual ~hkVirtualGamepadHandler() { } 
	virtual void onVirtualGamepadUpdate( const hkVirtualGamepad& g ) = 0;
};

//
// Keyboard
//

class hkVirtualKeyEvent
{
public:
	
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkVirtualKeyEvent );

	enum VKEY
	{
		VKEY_BACK           = 0x08,
		VKEY_TAB            = 0x09,
		VKEY_RETURN         = 0x0D,
		VKEY_SHIFT          = 0x10,
		VKEY_CONTROL        = 0x11,
		VKEY_MENU           = 0x12,
		VKEY_CAPITAL        = 0x14,
		VKEY_ESCAPE         = 0x1B,
		VKEY_SPACE          = 0x20,
		VKEY_END            = 0x23,
		VKEY_HOME           = 0x24,
		VKEY_LEFT           = 0x25,
		VKEY_UP             = 0x26,
		VKEY_RIGHT          = 0x27,
		VKEY_DOWN           = 0x28,
		VKEY_INSERT         = 0x2D,
		VKEY_DELETE         = 0x2E,
		//VKEY_0 thru VKEY_9 are the same as ASCII '0' thru '9' (= 0x30 - = 0x39) 
		//VKEY_A thru VKEY_Z are the same as ASCII 'A' thru 'Z' (= 0x41 - = 0x5A) 
		VKEY_LWIN           = 0x5B,
		VKEY_RWIN           = 0x5C,
		VKEY_APPS           = 0x5D,
		VKEY_NUMPAD0        = 0x60,
		VKEY_NUMPAD1        = 0x61,
		VKEY_NUMPAD2        = 0x62,
		VKEY_NUMPAD3        = 0x63,
		VKEY_NUMPAD4        = 0x64,
		VKEY_NUMPAD5        = 0x65,
		VKEY_NUMPAD6        = 0x66,
		VKEY_NUMPAD7        = 0x67,
		VKEY_NUMPAD8        = 0x68,
		VKEY_NUMPAD9        = 0x69,
		VKEY_MULTIPLY       = 0x6A,
		VKEY_ADD            = 0x6B,
		VKEY_SEPARATOR      = 0x6C,
		VKEY_SUBTRACT       = 0x6D,
		VKEY_DECIMAL        = 0x6E,
		VKEY_DIVIDE         = 0x6F,
		VKEY_F1             = 0x70,
		VKEY_F2             = 0x71,
		VKEY_F3             = 0x72,
		VKEY_F4             = 0x73,
		VKEY_F5             = 0x74,
		VKEY_F6             = 0x75,
		VKEY_F7             = 0x76,
		VKEY_F8             = 0x77,
		VKEY_F9             = 0x78,
		VKEY_F10            = 0x79,
		VKEY_F11            = 0x7A,
		VKEY_F12            = 0x7B,
		VKEY_F13            = 0x7C,
		VKEY_F14            = 0x7D,
		VKEY_F15            = 0x7E,
		VKEY_F16            = 0x7F,
		VKEY_F17            = 0x80,
		VKEY_F18            = 0x81,
		VKEY_F19            = 0x82,
		VKEY_F20            = 0x83,
		VKEY_F21            = 0x84,
		VKEY_F22            = 0x85,
		VKEY_F23            = 0x86,
		VKEY_F24            = 0x87,
		VKEY_NUMLOCK        = 0x90,
		VKEY_SCROLL         = 0x91,
		VKEY_LSHIFT         = 0xA0,
		VKEY_RSHIFT         = 0xA1,
		VKEY_LCONTROL       = 0xA2,
		VKEY_RCONTROL       = 0xA3,
		VKEY_LMENU          = 0xA4,
		VKEY_RMENU          = 0xA5,
		VKEY_OEM_1_SEMICOLON	 = 0xBA, // on US
		VKEY_OEM_PLUS			 = 0xBB, // 2000/XP 
		VKEY_OEM_COMMA			 = 0xBC, // 2000/XP 
		VKEY_OEM_MINUS			 = 0xBD, // 2000/XP 
		VKEY_OEM_PERIOD			 = 0xBE, // 2000/XP 
		VKEY_OEM_2_FORWARDSLASH  = 0xBF, //on US
		VKEY_OEM_3_TILDA         = 0xC0, //on US 
		VKEY_OEM_4_OPENBRACKET   = 0xDB, //on US 
		VKEY_OEM_5_BACKSLASH     = 0xDC, //on US 
		VKEY_OEM_6_CLOSE_BRACKET = 0xDD //on US 
	};

	hkUint8 m_key;
	hkBool m_state;
};

class hkVirtualKeyEventHandler
{
	public:
		//+hk.MemoryTracker(ignore=True)
	
		virtual ~hkVirtualKeyEventHandler() { } 
		virtual void onVirtualKeyEventUpdate( const hkVirtualKeyEvent& m ) = 0;
};

//
// Mouse
//

class hkVirtualMouse
{
public:
	
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkVirtualMouse );

	enum Button
	{
		MOUSE_RIGHT_BUTTON  = 1,
		MOUSE_MIDDLE_BUTTON = 2,
		MOUSE_LEFT_BUTTON   = 4
	};

	hkEnum<Button,hkUint32> m_buttons;
	float m_screenX; // 0..1
	float m_screenY; // 0..1
	float m_wheelDelta;

};

class hkVirtualMouseHandler
{
	public:
		//+hk.MemoryTracker(ignore=True)
	
		virtual ~hkVirtualMouseHandler() { } 
		virtual void onVirtualMouseUpdate( const hkVirtualMouse& m ) = 0;
};


//
// The main server, very simple
//
struct hkVirtualFramebufferServerClient
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkVirtualFramebufferServerClient );

	class hkSocket* m_socket;
	class hkDisplaySerializeOStream* m_outStream; 
	class hkDisplaySerializeIStream* m_inStream;

	struct RuntimeOptions
	{
		RuntimeOptions() : m_scale(2), m_maxSendFormat(hkVirtualFramebufferProtocol::PIXEL_RGB565), m_allowDiffSend(true) 
		{ 
			m_areaOfInterest.setToAll();
		}

		bool operator==(const RuntimeOptions& other) const 
		{ 
			return (m_scale == other.m_scale) &&
					(m_areaOfInterest == other.m_areaOfInterest) &&
					(m_maxSendFormat == other.m_maxSendFormat);
		}

		hkUint32 m_scale; // 1 == 1:1, 2 = half scale, 4 == quarter scale 
		hkVirtualFramebufferRelativeRect m_areaOfInterest; // from x, y   to x,y, (-1 in to == all), in 0..1 scale
		hkVirtualFramebufferProtocol::FramebufferFormat m_maxSendFormat; // so you can reduce color depth etc in favor of speed
		bool m_allowDiffSend; // as diff can take up local mem, on test consoles etc there may be none left
		bool m_allowRleSend;  // as rle can take up local mem, on test consoles etc there may be none left
	};
	
	RuntimeOptions m_options;
	hkArray< hkUint8 > m_framebufferDiffStore;
	RuntimeOptions m_lastDiffSendOptions; // if the same then the diff will be ok
};

class hkVirtualFramebufferServer : public hkReferencedObject
{
public:
	
	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES );

	hkVirtualFramebufferServer();
	virtual ~hkVirtualFramebufferServer();

	void serve( int listenPort = HK_VIRTUAL_FRAMEBUFFER_SERVER_PORT );

	int getNumConnectedClients() const; // don't bother providing a framebuffer if none connected yet
	bool getFramebufferRectOfInterest( hkVirtualFramebufferRelativeRect& rect );

	void sendFrameBuffer( const hkVirtualFramebuffer* buffer );
	void step();

	void registerGamepadCallback( hkVirtualGamepadHandler* h );
	void unregisterGamepadCallback( hkVirtualGamepadHandler* h );

	void registerKeyboardCallback( hkVirtualKeyEventHandler* h );
	void unregisterKeyboardCallback( hkVirtualKeyEventHandler* h );

	void registerMouseCallback( hkVirtualMouseHandler* h );
	void unregisterMouseCallback( hkVirtualMouseHandler* h );


protected:

	void deleteClient(int i);
	void recvInput( hkVirtualFramebufferServerClient& c );
	
	void sendFramebufferToClient( hkVirtualFramebufferServerClient& c, const hkVirtualFramebuffer* buffer );

	hkArray< hkVirtualGamepadHandler* >		m_gamepadHandlers;
	hkArray< hkVirtualKeyEventHandler* >	m_keyboardHandlers;
	hkArray< hkVirtualMouseHandler* >		m_mouseHandlers;
	
	hkArray< hkVirtualFramebufferServerClient* > m_clients; 
	hkSocket* m_server;

	
};

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
