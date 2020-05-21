/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALZE_VERSION_CHECKING_UTILS_H
#define HKSERIALZE_VERSION_CHECKING_UTILS_H

class hkOstream;
class hkVersionPatchManager;
class hkDataWorld;
class hkClassNameRegistry;

/// Utility routine to help maintain version compatibility
/// We use these routines to help ensure we don't break backward compatibility
namespace hkVersionCheckingUtils
{
		/// Write a human readable description of the difference between these 2 versions
		/// For this to be useful the classes should be different versions of the same hkClass
		/// If detailed is true then this reports member offset and class size changes too.
	void HK_CALL summarizeChanges(hkOstream& output, const hkClass& oldVersion, const hkClass& newVersion, bool detailed = false);

	enum Flags
	{
		NONE			= 0,
		VERBOSE			= 1,
		IGNORE_REMOVED	= 2
	};

	hkResult HK_CALL verifyClassPatches(hkOstream& report, const hkDataWorld& world, const hkClassNameRegistry& classReg, const hkVersionPatchManager& patchManager, Flags flags = NONE);
}

#endif // HKSERIALZE_VERSION_CHECKING_UTILS_H

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
