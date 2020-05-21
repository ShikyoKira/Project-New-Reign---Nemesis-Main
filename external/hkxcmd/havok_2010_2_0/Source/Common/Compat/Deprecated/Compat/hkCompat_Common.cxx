// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COMPAT_VERSION_FROM
#error You must define HK_COMPAT_VERSION_FROM before including hkCompat_Common.cxx
#endif

#ifndef HK_COMPAT_VERSION_TO
#error You must define HK_COMPAT_VERSION_TO before including hkCompat_Common.cxx
#endif

#ifndef HK_COMPAT_OPTIONAL_UPDATE_FUNC
#define HK_SET_DEFAULT_UPDATE_FUNC
#define HK_COMPAT_OPTIONAL_UPDATE_FUNC HK_NULL
#endif

extern hkVersionRegistry::UpdateDescription hkVersionUpdateDescription;
hkVersionRegistry::UpdateDescription hkVersionUpdateDescription(s_renames, s_updateActions, & HK_COMPAT_VERSION_TO::hkHavokDefaultClassRegistry);
extern hkVersionRegistry::Updater hkVersionUpdater;
hkVersionRegistry::Updater hkVersionUpdater =
{
	HK_COMPAT_VERSION_FROM::VersionString,
	HK_COMPAT_VERSION_TO::VersionString,
	&hkVersionUpdateDescription,
	HK_COMPAT_OPTIONAL_UPDATE_FUNC
};
#ifdef HK_SET_DEFAULT_UPDATE_FUNC
#undef HK_COMPAT_OPTIONAL_UPDATE_FUNC
#undef HK_SET_DEFAULT_UPDATE_FUNC
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
