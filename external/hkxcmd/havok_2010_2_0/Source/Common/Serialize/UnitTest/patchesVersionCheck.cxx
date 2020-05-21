// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

{
	const char* strings[] = {
#define	HK_PATCH_BEGIN(CLASSNAME, OLDVER, NEWCLASSNAME, NEWVER) CLASSNAME, NEWCLASSNAME, 
#		define		HK_PATCH_GROUP(CNAME, VERSION) 
#		define		HK_PATCH_CAST(CNAME) 
#		define		HK_PATCH_DEPENDS(CNAME, VERSION) 
#		define		HK_PATCH_PARENT_SET(OLDNAME, NEWNAME) 
#		define		HK_PATCH_ENUM_ADDED(NAME) 
#		define		HK_PATCH_ENUM_REMOVED(NAME) 
#		define		HK_PATCH_ENUM_RENAMED(OLD, NEW) 
#		define		HK_PATCH_ENUM_VALUE_ADDED(NAME, VNAME, VAL) 
#		define		HK_PATCH_ENUM_VALUE_REMOVED(NAME, VNAME, VAL) 
#		define		HK_PATCH_ENUM_VALUE_CHANGED(NAME, VNAME, OLD, NEW) 

#		define		HK_PATCH_MEMBER_ADDED(NAME, TYPE, TYPE_NAME, TUPLES) 
#		define		HK_PATCH_MEMBER_ADDED_INT(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_ADDED_REAL(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_ADDED_BOOL(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_ADDED_BYTE(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_ADDED_VEC_4(NAME, vx, vy, vz, vw) 
#		define		HK_PATCH_MEMBER_ADDED_VEC_4_IDENTITY(NAME) 
#		define		HK_PATCH_MEMBER_ADDED_VEC_12(NAME, ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw) 
#		define		HK_PATCH_MEMBER_ADDED_VEC_12_IDENTITY(NAME) 
#		define		HK_PATCH_MEMBER_ADDED_VEC_16(NAME, tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw) 
#		define		HK_PATCH_MEMBER_ADDED_VEC_16_IDENTITY(NAME) 
#		define		HK_PATCH_MEMBER_ADDED_CSTRING(NAME, VALUE)
#		define		HK_PATCH_MEMBER_ADDED_POINTER(NAME, SUBTYPE, VALUE)

#		define		HK_PATCH_MEMBER_DEFAULT_SET_INT(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_DEFAULT_SET_REAL(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_DEFAULT_SET_BOOL(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_DEFAULT_SET_BYTE(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_4(NAME, vx, vy, vz, vw) 
#		define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_12(NAME, tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw) 
#		define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_16(NAME, ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw) 
#		define		HK_PATCH_MEMBER_DEFAULT_SET_CSTRING(NAME, VALUE) 
#		define		HK_PATCH_MEMBER_DEFAULT_SET_POINTER(NAME, VALUE) 

#		define		HK_PATCH_MEMBER_RENAMED(OLDNAME,NEWNAME) 
#		define		HK_PATCH_MEMBER_DEFAULT(NAME,OLD,NEW) 
#		define		HK_PATCH_FUNCTION(NAME) 
#		define		HK_PATCH_MEMBER_REMOVED(NAME, TYPE, TYPE_NAME, TUPLES) 
#		define	HK_PATCH_END() 
#include HK_PATCHES_FILE
#		undef		HK_PATCH_BEGIN
#		undef		HK_PATCH_GROUP
#		undef		HK_PATCH_CAST
#		undef		HK_PATCH_DEPENDS
#		undef		HK_PATCH_PARENT_SET
#		undef		HK_PATCH_ENUM_ADDED
#		undef		HK_PATCH_ENUM_REMOVED
#		undef		HK_PATCH_ENUM_RENAMED
#		undef		HK_PATCH_ENUM_VALUE_ADDED
#		undef		HK_PATCH_ENUM_VALUE_REMOVED
#		undef		HK_PATCH_ENUM_VALUE_CHANGED

#		undef		HK_PATCH_MEMBER_ADDED
#		undef		HK_PATCH_MEMBER_ADDED_INT
#		undef		HK_PATCH_MEMBER_ADDED_REAL
#		undef		HK_PATCH_MEMBER_ADDED_BOOL
#		undef		HK_PATCH_MEMBER_ADDED_BYTE
#		undef		HK_PATCH_MEMBER_ADDED_VEC_4
#		undef		HK_PATCH_MEMBER_ADDED_VEC_4_IDENTITY
#		undef		HK_PATCH_MEMBER_ADDED_VEC_12
#		undef		HK_PATCH_MEMBER_ADDED_VEC_12_IDENTITY
#		undef		HK_PATCH_MEMBER_ADDED_VEC_16
#		undef		HK_PATCH_MEMBER_ADDED_VEC_16_IDENTITY
#		undef		HK_PATCH_MEMBER_ADDED_CSTRING
#		undef		HK_PATCH_MEMBER_ADDED_POINTER
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_INT
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_REAL
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_BOOL
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_BYTE
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_4
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_12
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_16
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_CSTRING
#		undef		HK_PATCH_MEMBER_DEFAULT_SET_POINTER

#		undef		HK_PATCH_MEMBER_RENAMED
#		undef		HK_PATCH_MEMBER_DEFAULT
#		undef		HK_PATCH_FUNCTION
#		undef		HK_PATCH_MEMBER_REMOVED
#		undef		HK_PATCH_END

};

	// This exception is for historical reasons ONLY
	// DO NOT ADD TO THIS LIST - move the patch or rename your class.
	const char* exceptions[] = { "hkWorldMemoryAvailableWatchDog" };

	int stringCount = (int) HK_COUNT_OF(strings);
	int prefixCount = (int) HK_COUNT_OF(prefixes);
	int exceptionCount = (int) HK_COUNT_OF(exceptions);
	for(int i=0; i<stringCount; i++)
	{
		if (!strings[i])
			continue;

		hkBool prefixOK = hasPrefixes( prefixes, prefixCount, strings[i] );

		if(!prefixOK) // check if we're supposed to skip it
		{
			for (int j=0; j<exceptionCount; j++)
			{
				if (hkString::strCmp( strings[i], exceptions[j]) == 0)
				{
					prefixOK = true;
				}
			}
		}

		HK_TEST2( prefixOK, "Patch for class " << strings[i] << " is in wrong patches file.");
	}
}

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
