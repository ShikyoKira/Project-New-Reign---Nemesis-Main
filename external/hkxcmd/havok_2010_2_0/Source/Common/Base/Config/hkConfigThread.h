/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
// Build configuration settings
// These are compile time settings for the libraries.
// Changing these values with compiled libs will not work,

// If this setting is changed you must rebuild all Havok libs

// EQSELECT: HK_CONFIG_THREAD, Choose to enable or disable simd instructions:
// This determines whether or not we use simd instructions.


#ifndef HK_BASE_CONFIGTHREAD_H
#define HK_BASE_CONFIGTHREAD_H

#define HK_CONFIG_SINGLE_THREADED 1
#define HK_CONFIG_MULTI_THREADED  2

#if !defined(HKBASE_HKBASETYPES_H)
#	error hkbase/hkBase.h needs to be included before this file
#endif

#ifndef HK_CONFIG_THREAD

	// ia32
#		define HK_CONFIG_THREAD HK_CONFIG_MULTI_THREADED

	// AMD x64

#else // ifndef HK_CONFIG_THREAD
#	if (HK_CONFIG_THREAD != HK_CONFIG_MULTI_THREADED) && (HK_CONFIG_THREAD != HK_CONFIG_SINGLE_THREADED)
#		error invalid config thread option
#	endif
#endif // ifndef HK_CONFIG_THREAD

#define HK_MAX_NUM_THREADS 12	// latest I9 has 12 hyperthreads

#endif // HK_BASE_CONFIGTHREAD_H

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
