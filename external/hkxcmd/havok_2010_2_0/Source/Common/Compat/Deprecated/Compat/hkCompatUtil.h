/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_COMPAT_COMPATUTIL_H
#define HK_COMPAT_COMPATUTIL_H

#include <Common/Compat/hkCompat.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkClassMemberAccessor.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Compat/Deprecated/Version/hkObjectUpdateTracker.h>
#include <Common/Serialize/Version/hkVersionUtil.h>
#include <Common/Compat/Deprecated/Version/hkVersionRegistry.h>

/// Versioning utility functions and structures.
namespace hkCompatUtil
{
	/// Update the named variant class name value.
	void HK_CALL updateNamedVariantClassName( hkArray<hkVariant>& objectsInOut, const hkVersionRegistry::ClassRename* renames, hkObjectUpdateTracker& tracker );

	/// Version an array of structs by calling a versioning function on each member.
	void HK_CALL versionArrayOfStructs(	hkClassMemberAccessor& oldArrayMember, 
										hkClassMemberAccessor& newArrayMember, 
										hkVersionRegistry::VersionFunc structVersionFunc, 
										hkObjectUpdateTracker& tracker );
}

#endif // HK_COMPAT_COMPATUTIL_H

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
