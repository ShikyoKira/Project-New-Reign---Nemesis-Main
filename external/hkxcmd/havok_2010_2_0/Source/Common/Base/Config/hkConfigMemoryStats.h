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

// If this setting is changed you must rebuild Havok libs

// EQSELECT: HK_CONFIG_MEMORY_STATS, Choose to enable or disable memory statistics;
// This determines whether or not we create static arrays of statistics information

#ifndef HK_BASE_CONFIGMEMORYSTATS_H
#define HK_BASE_CONFIGMEMORYSTATS_H

#define HK_CONFIG_MEMORY_STATS_ENABLED 0	// Enabled
#define HK_CONFIG_MEMORY_STATS_DISABLED 1	// Disabled
#define HK_CONFIG_MEMORY_STATS_DEFAULT 2	// Disabled in release

// VALUE:
#define HK_CONFIG_MEMORY_STATS HK_CONFIG_MEMORY_STATS_DEFAULT

// NOTE: stats are turned on and off based on debug (the above code is ignored)
// for better control, remove the code below (basetypes.h must be included before
// this file)

#if HK_CONFIG_MEMORY_STATS == HK_CONFIG_MEMORY_STATS_DEFAULT

#	if  defined(HK_DEBUG)
#	undef HK_CONFIG_MEMORY_STATS
#	define HK_CONFIG_MEMORY_STATS HK_CONFIG_MEMORY_STATS_ENABLED

#	else
#	undef HK_CONFIG_MEMORY_STATS
#	define HK_CONFIG_MEMORY_STATS HK_CONFIG_MEMORY_STATS_DISABLED

#	endif

#endif

#endif // HK_BASE_CONFIGMEMORYSTATS_H

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
