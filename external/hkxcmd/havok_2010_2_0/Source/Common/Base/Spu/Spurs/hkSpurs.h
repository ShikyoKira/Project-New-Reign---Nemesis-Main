/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BASE_SPU_SPURS_H
#define HK_BASE_SPU_SPURS_H

	typedef hkUint32 sys_event_queue_t;
	#define HK_CELL_SPURS void

	/// Call this utility function to inform Havok about your SPURS instance
	/// This must be called before a hkSemaphores are created of Tasksets are initialized.
void HK_CALL hkSetSpursInstance(HK_CELL_SPURS* spurs);

	/// Access the globally set SPURS instance
HK_CELL_SPURS* HK_CALL hkGetSpursInstance();


#endif // HK_BASE_SPU_SPURS_H

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
