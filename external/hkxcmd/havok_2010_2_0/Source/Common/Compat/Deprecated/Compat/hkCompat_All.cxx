// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COMPAT_FILE
#	error Please define HK_COMPAT_FILE before including this file
#endif

// The default is to list all classes and updaters

#ifndef HK_LIST_UPDATERS
#	define HK_LIST_UPDATERS 1
#endif

#ifndef HK_LIST_CLASS_VERSIONS
#	define HK_LIST_CLASS_VERSIONS 1
#endif

#include <Common/Base/Config/hkConfigVersion.h>
#include <Common/Compat/Deprecated/Version/hkVersionRegistry.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>

#if HK_LIST_UPDATERS
	// Forward declarations of all used updaters
#	define HK_CLASS_UPDATE_INFO(FROM,TO) \
		namespace hkCompat_hk##FROM##_hk##TO \
		{ \
			extern hkVersionRegistry::Updater hkVersionUpdater; \
		}
#		include HK_COMPAT_FILE
#	undef HK_CLASS_UPDATE_INFO

	// List of all updaters

#	define HK_CLASS_UPDATE_INFO(FROM,TO) &hkCompat_hk##FROM##_hk##TO::hkVersionUpdater,

const hkVersionRegistry::Updater* hkVersionRegistry::StaticLinkedUpdaters[] =
{
#		include HK_COMPAT_FILE
	HK_NULL
};
#	undef HK_CLASS_UPDATE_INFO

#else

// empty updaters list
const hkVersionRegistry::Updater* hkVersionRegistry::StaticLinkedUpdaters[] =
{
	HK_NULL
};

#endif

#if HK_LIST_CLASS_VERSIONS

	// Forward declarations of all used classes
#	define HK_HAVOK_CLASSES_VERSION(HKVER) \
	namespace hkHavok##HKVER##Classes \
	{ \
		extern const hkStaticClassNameRegistry hkHavokDefaultClassRegistry; \
	}
#	include <Common/Compat/Deprecated/Compat/hkHavokVersionClasses.h>
#	undef HK_HAVOK_CLASSES_VERSION

	// List of all versions and corresponding classes
#	define HK_HAVOK_CLASSES_VERSION(HKVER) &hkHavok##HKVER##Classes::hkHavokDefaultClassRegistry,

	const hkStaticClassNameRegistry* hkVersionRegistry::StaticLinkedClassRegistries[] =
	{
#		include <Common/Compat/Deprecated/Compat/hkHavokVersionClasses.h>
		HK_NULL
	};
#undef HK_HAVOK_CLASSES_VERSION

#else

	// only current class registry
	namespace hkHavokCurrentClasses
	{
		extern const hkStaticClassNameRegistry hkHavokDefaultClassRegistry;
	}
	const hkStaticClassNameRegistry* hkVersionRegistry::StaticLinkedClassRegistries[] =
	{
		&hkHavokCurrentClasses::hkHavokDefaultClassRegistry,
		HK_NULL
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
