/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_VIRTUAL_KEYBOARD
#define HK_VISUALIZE_VIRTUAL_KEYBOARD

enum hkKeyboardCommand // for serialization
{
	HK_VIRTUAL_KEYBOARD_DUMP = 0xA0
};


typedef unsigned char HK_KEYBOARD_VKEY;

#define HK_VKEY_BACK           0x08
#define HK_VKEY_TAB            0x09

#define HK_VKEY_RETURN         0x0D
#define HK_VKEY_SHIFT          0x10
#define HK_VKEY_CONTROL        0x11

#define HK_VKEY_CAPITAL        0x14

#define HK_VKEY_ESCAPE         0x1B
#define HK_VKEY_SPACE          0x20
#define HK_VKEY_END            0x23
#define HK_VKEY_HOME           0x24

#define HK_VKEY_LEFT           0x25
#define HK_VKEY_UP             0x26
#define HK_VKEY_RIGHT          0x27
#define HK_VKEY_DOWN           0x28

#define HK_VKEY_INSERT         0x2D
#define HK_VKEY_DELETE         0x2E

/* HK_VKEY_0 thru HK_VKEY_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* HK_VKEY_A thru HK_VKEY_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

#define HK_VKEY_LWIN           0x5B
#define HK_VKEY_RWIN           0x5C
#define HK_VKEY_APPS           0x5D
#define HK_VKEY_NUMPAD0        0x60
#define HK_VKEY_NUMPAD1        0x61
#define HK_VKEY_NUMPAD2        0x62
#define HK_VKEY_NUMPAD3        0x63
#define HK_VKEY_NUMPAD4        0x64
#define HK_VKEY_NUMPAD5        0x65
#define HK_VKEY_NUMPAD6        0x66
#define HK_VKEY_NUMPAD7        0x67
#define HK_VKEY_NUMPAD8        0x68
#define HK_VKEY_NUMPAD9        0x69

#define HK_VKEY_MULTIPLY       0x6A
#define HK_VKEY_ADD            0x6B
#define HK_VKEY_SEPARATOR      0x6C
#define HK_VKEY_SUBTRACT       0x6D
#define HK_VKEY_DECIMAL        0x6E
#define HK_VKEY_DIVIDE         0x6F

#define HK_VKEY_F1             0x70
#define HK_VKEY_F2             0x71
#define HK_VKEY_F3             0x72
#define HK_VKEY_F4             0x73
#define HK_VKEY_F5             0x74
#define HK_VKEY_F6             0x75
#define HK_VKEY_F7             0x76
#define HK_VKEY_F8             0x77
#define HK_VKEY_F9             0x78
#define HK_VKEY_F10            0x79
#define HK_VKEY_F11            0x7A
#define HK_VKEY_F12            0x7B
#define HK_VKEY_F13            0x7C
#define HK_VKEY_F14            0x7D
#define HK_VKEY_F15            0x7E
#define HK_VKEY_F16            0x7F
#define HK_VKEY_F17            0x80
#define HK_VKEY_F18            0x81
#define HK_VKEY_F19            0x82
#define HK_VKEY_F20            0x83
#define HK_VKEY_F21            0x84
#define HK_VKEY_F22            0x85
#define HK_VKEY_F23            0x86
#define HK_VKEY_F24            0x87

#define HK_VKEY_NUMLOCK        0x90
#define HK_VKEY_SCROLL         0x91
#define HK_VKEY_LSHIFT         0xA0
#define HK_VKEY_RSHIFT         0xA1
#define HK_VKEY_LCONTROL       0xA2
#define HK_VKEY_RCONTROL       0xA3
#define HK_VKEY_LMENU          0xA4
#define HK_VKEY_RMENU          0xA5


#define HK_KEYBOARD_NUM_VKEYS  0xFF // 255 possible virtual key values
#define HK_KEYBOARD_NUM_CHARS  0xFF // 255 possible char values 

/// A simple keyboard state wrapper for the visual debugger
struct hkKeyboard
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_TOOLS, hkKeyboard );

	hkBool m_curVKeyState[HK_KEYBOARD_NUM_VKEYS];
	hkBool m_curCKeyState[HK_KEYBOARD_NUM_CHARS];

	hkBool m_prevVKeyState[HK_KEYBOARD_NUM_VKEYS];
	hkBool m_prevCKeyState[HK_KEYBOARD_NUM_CHARS];

	hkBool m_valid;
};

#endif // HK_VISUALIZE_VIRTUAL_KEYBOARD

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
