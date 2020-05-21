// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PATCHES_FILE
#	error You must define HK_PATCHES_FILE before including this file
#endif





#include <Common/Base/Config/hkConfigBranches.h>
{
#define HK_CLASS_ADDED -1
#define HK_CLASS_REMOVED -1

//
// Defaults
//
#define HK_PATCH_BEGIN(CLASSNAME, OLDVER, NEWNAME, NEWVER)
#define		HK_PATCH_GROUP(CNAME, VERSION)
#define		HK_PATCH_CAST(CNAME)
#define		HK_PATCH_DEPENDS(CNAME, VERSION)
#define		HK_PATCH_PARENT_SET(OLDNAME, NEWNAME)

#define		HK_PATCH_MEMBER_ADDED(NAME, TYPE, TYPE_NAME, TUPLES)
#define		HK_PATCH_MEMBER_ADDED_INT(NAME, VALUE) static const hkInt64 HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = hkInt64(VALUE);
#define		HK_PATCH_MEMBER_ADDED_REAL(NAME, VALUE) static const hkReal HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = hkReal(VALUE);
#define		HK_PATCH_MEMBER_ADDED_BOOL(NAME, VALUE) static const hkInt64 HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = hkInt64(VALUE);
#define		HK_PATCH_MEMBER_ADDED_BYTE(NAME, VALUE) static const hkInt64 HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = hkInt64(VALUE);
#define		HK_PATCH_MEMBER_ADDED_VEC_4(NAME, vx, vy, vz, vw) static const hkReal HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)[] = {vx, vy, vz, vw};
#define		HK_PATCH_MEMBER_ADDED_VEC_4_IDENTITY(NAME)
#define		HK_PATCH_MEMBER_ADDED_VEC_12(NAME, tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw) static const hkReal HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)[] = {tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw};
#define		HK_PATCH_MEMBER_ADDED_VEC_12_IDENTITY(NAME)
#define		HK_PATCH_MEMBER_ADDED_VEC_16(NAME, ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw) static const hkReal HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)[] = {ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw};
#define		HK_PATCH_MEMBER_ADDED_VEC_16_IDENTITY(NAME)
#define		HK_PATCH_MEMBER_ADDED_CSTRING(NAME, VALUE) static const char* HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = VALUE;
#define		HK_PATCH_MEMBER_ADDED_POINTER(NAME, SUBTYPE, VALUE) static const void* HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = VALUE;

#define		HK_PATCH_MEMBER_DEFAULT_SET_INT(NAME, VALUE) static const hkInt64 HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = hkInt64(VALUE);
#define		HK_PATCH_MEMBER_DEFAULT_SET_REAL(NAME, VALUE) static const hkReal HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = hkReal(VALUE);
#define		HK_PATCH_MEMBER_DEFAULT_SET_BOOL(NAME, VALUE) static const hkInt64 HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = hkInt64(VALUE);
#define		HK_PATCH_MEMBER_DEFAULT_SET_BYTE(NAME, VALUE) static const hkInt64 HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = hkInt64(VALUE);
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_4(NAME, vx, vy, vz, vw) static const hkReal HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)[] = {vx, vy, vz, vw};
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_12(NAME, tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw) static const hkReal HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)[] = {tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw};
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_16(NAME, ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw) static const hkReal HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)[] = {ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw};
#define		HK_PATCH_MEMBER_DEFAULT_SET_CSTRING(NAME, VALUE) static const char* HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = VALUE;
#define		HK_PATCH_MEMBER_DEFAULT_SET_POINTER(NAME, VALUE) static const void* HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__) = VALUE;

#define		HK_PATCH_MEMBER_RENAMED(OLDNAME,NEWNAME)
#define		HK_PATCH_FUNCTION(NAME)
#define		HK_PATCH_MEMBER_REMOVED(NAME, TYPE, TYPE_NAME, TUPLES)

#define HK_PATCH_END()
#		include HK_PATCHES_FILE
#undef	HK_PATCH_BEGIN
#undef		HK_PATCH_GROUP
#undef		HK_PATCH_CAST
#undef		HK_PATCH_DEPENDS
#undef		HK_PATCH_PARENT_SET

#undef		HK_PATCH_MEMBER_ADDED
#undef		HK_PATCH_MEMBER_ADDED_INT
#undef		HK_PATCH_MEMBER_ADDED_REAL
#undef		HK_PATCH_MEMBER_ADDED_BOOL
#undef		HK_PATCH_MEMBER_ADDED_BYTE
#undef		HK_PATCH_MEMBER_ADDED_VEC_4
#undef		HK_PATCH_MEMBER_ADDED_VEC_4_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_VEC_12
#undef		HK_PATCH_MEMBER_ADDED_VEC_12_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_VEC_16
#undef		HK_PATCH_MEMBER_ADDED_VEC_16_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_CSTRING
#undef		HK_PATCH_MEMBER_ADDED_POINTER

#undef		HK_PATCH_MEMBER_DEFAULT_SET_INT
#undef		HK_PATCH_MEMBER_DEFAULT_SET_REAL
#undef		HK_PATCH_MEMBER_DEFAULT_SET_BOOL
#undef		HK_PATCH_MEMBER_DEFAULT_SET_BYTE
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_4
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_12
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_16
#undef		HK_PATCH_MEMBER_DEFAULT_SET_CSTRING
#undef		HK_PATCH_MEMBER_DEFAULT_SET_POINTER

#undef		HK_PATCH_MEMBER_RENAMED
#undef		HK_PATCH_FUNCTION
#undef		HK_PATCH_MEMBER_REMOVED
#undef	HK_PATCH_END
//
// Patches
//

#define HK_PATCH_BEGIN(CLASSNAME, OLDVER, NEWNAME, NEWVER)
#define		HK_PATCH_GROUP(CNAME, VERSION) static const hkVersionPatchManager::GroupPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {CNAME, VERSION};
#define		HK_PATCH_CAST(CNAME) static const hkVersionPatchManager::CastPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {CNAME};
#define		HK_PATCH_DEPENDS(CNAME, VERSION) static const hkVersionPatchManager::DependsPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {CNAME, VERSION};
#define		HK_PATCH_PARENT_SET(OLDNAME, NEWNAME) static const hkVersionPatchManager::SetParentPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {OLDNAME, NEWNAME};

#define		HK_PATCH_MEMBER_ADDED(NAME, TYPE, TYPE_NAME, TUPLES) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE,TYPE_NAME,TUPLES, HK_NULL};
#define		HK_PATCH_MEMBER_ADDED_INT(NAME, VALUE) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_INT,HK_NULL,0,&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_ADDED_REAL(NAME, VALUE) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_REAL,HK_NULL,0,&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_ADDED_BOOL(NAME, VALUE) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_BYTE,HK_NULL,0,&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_ADDED_BYTE(NAME, VALUE) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_BYTE,HK_NULL,0,&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_ADDED_VEC_4(NAME, vx, vy, vz, vw) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_VEC_4,HK_NULL,0,reinterpret_cast<const hkVector4*>(&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__))};
#define		HK_PATCH_MEMBER_ADDED_VEC_4_IDENTITY(NAME) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_VEC_4,HK_NULL,0,reinterpret_cast<const hkVector4*>(&hkQuaternion::getIdentity())};
#define		HK_PATCH_MEMBER_ADDED_VEC_12(NAME, tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_VEC_12,HK_NULL,0,reinterpret_cast<const hkQsTransform*>(&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__))};
#define		HK_PATCH_MEMBER_ADDED_VEC_12_IDENTITY(NAME) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_VEC_12,HK_NULL,0,reinterpret_cast<const hkQsTransform*>(&hkQsTransform::getIdentity())};
#define		HK_PATCH_MEMBER_ADDED_VEC_16(NAME, ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_VEC_16,HK_NULL,0,reinterpret_cast<const hkTransform*>(&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__))};
#define		HK_PATCH_MEMBER_ADDED_VEC_16_IDENTITY(NAME) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_VEC_16,HK_NULL,0,reinterpret_cast<const hkTransform*>(&hkTransform::getIdentity())};
#define		HK_PATCH_MEMBER_ADDED_CSTRING(NAME, VALUE) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_CSTRING,HK_NULL,0,&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_ADDED_POINTER(NAME, SUBTYPE, VALUE) static const hkVersionPatchManager::MemberAddedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE_OBJECT,SUBTYPE,0,&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};

#define		HK_PATCH_MEMBER_DEFAULT_SET_INT(NAME, VALUE) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME, &HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_DEFAULT_SET_REAL(NAME, VALUE) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME, &HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_DEFAULT_SET_BOOL(NAME, VALUE) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME, &HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_DEFAULT_SET_BYTE(NAME, VALUE) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME, &HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_4(NAME, vx, vy, vz, vw) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME, &HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_12(NAME, tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME, &HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_16(NAME, ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME, &HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_DEFAULT_SET_CSTRING(NAME, VALUE) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};
#define		HK_PATCH_MEMBER_DEFAULT_SET_POINTER(NAME, VALUE) static const hkVersionPatchManager::DefaultChangedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,&HK_PREPROCESSOR_JOIN_TOKEN(d,__LINE__)};

#define		HK_PATCH_MEMBER_RENAMED(OLDNAME,NEWNAME) static const hkVersionPatchManager::MemberRenamedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {OLDNAME,NEWNAME};
#define		HK_PATCH_FUNCTION(NAME) static const hkVersionPatchManager::FunctionPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = { #NAME, NAME };
#define		HK_PATCH_MEMBER_REMOVED(NAME, TYPE, TYPE_NAME, TUPLES) static const hkVersionPatchManager::MemberRemovedPatch HK_PREPROCESSOR_JOIN_TOKEN(p,__LINE__) = {NAME,hkDataObject::TYPE,TYPE_NAME,TUPLES};
#define HK_PATCH_END()
#		include HK_PATCHES_FILE
#undef	HK_PATCH_BEGIN
#undef		HK_PATCH_GROUP
#undef		HK_PATCH_CAST
#undef		HK_PATCH_DEPENDS
#undef		HK_PATCH_PARENT_SET

#undef		HK_PATCH_MEMBER_ADDED
#undef		HK_PATCH_MEMBER_ADDED_INT
#undef		HK_PATCH_MEMBER_ADDED_REAL
#undef		HK_PATCH_MEMBER_ADDED_BOOL
#undef		HK_PATCH_MEMBER_ADDED_BYTE
#undef		HK_PATCH_MEMBER_ADDED_VEC_4
#undef		HK_PATCH_MEMBER_ADDED_VEC_4_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_VEC_12
#undef		HK_PATCH_MEMBER_ADDED_VEC_12_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_VEC_16
#undef		HK_PATCH_MEMBER_ADDED_VEC_16_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_CSTRING
#undef		HK_PATCH_MEMBER_ADDED_POINTER

#undef		HK_PATCH_MEMBER_DEFAULT_SET_INT
#undef		HK_PATCH_MEMBER_DEFAULT_SET_REAL
#undef		HK_PATCH_MEMBER_DEFAULT_SET_BOOL
#undef		HK_PATCH_MEMBER_DEFAULT_SET_BYTE
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_4
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_16
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_12
#undef		HK_PATCH_MEMBER_DEFAULT_SET_CSTRING
#undef		HK_PATCH_MEMBER_DEFAULT_SET_POINTER

#undef		HK_PATCH_MEMBER_RENAMED
#undef		HK_PATCH_FUNCTION
#undef		HK_PATCH_MEMBER_REMOVED
#undef	HK_PATCH_END

//
// PatchInfos (wrapping several logical patches)
//

#define	HK_PATCH_BEGIN(CLASSNAME, OLDVER, NEWCLASSNAME, NEWVER) { const char* classname = CLASSNAME; const char* newclassname = NEWCLASSNAME; const int oldver = OLDVER; const int newver = NEWVER; static const hkVersionPatchManager::PatchInfo::Component patch[] = {
#define		HK_PATCH_GROUP(CNAME, VERSION) {hkVersionPatchManager::PATCH_GROUP, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_CAST(CNAME) {hkVersionPatchManager::PATCH_CAST, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_DEPENDS(CNAME, VERSION) {hkVersionPatchManager::PATCH_DEPENDS, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_PARENT_SET(OLDNAME, NEWNAME) {hkVersionPatchManager::PATCH_PARENT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},

#define		HK_PATCH_MEMBER_ADDED(NAME, TYPE, TYPE_NAME, TUPLES) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_INT(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_REAL(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_BOOL(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_BYTE(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_VEC_4(NAME, vx, vy, vz, vw) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_VEC_4_IDENTITY(NAME) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_VEC_12(NAME, tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_VEC_12_IDENTITY(NAME) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_VEC_16(NAME, ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_VEC_16_IDENTITY(NAME) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_CSTRING(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_ADDED_POINTER(NAME, SUBTYPE, VALUE) {hkVersionPatchManager::PATCH_MEMBER_ADDED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},

#define		HK_PATCH_MEMBER_DEFAULT_SET_INT(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_DEFAULT_SET_REAL(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_DEFAULT_SET_BOOL(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_DEFAULT_SET_BYTE(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_4(NAME, vx, vy, vz, vw) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_16(NAME, ax, ay, az, aw, bx, by, bz, bw, cx, cy, cz, cw, dx, dy, dz, dw) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_DEFAULT_SET_VEC_12(NAME, tx, ty, tz, tw, rix, riy, riz, rr, sx, sy, sz, sw) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_DEFAULT_SET_CSTRING(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_DEFAULT_SET_POINTER(NAME, VALUE) {hkVersionPatchManager::PATCH_MEMBER_DEFAULT_SET, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},

#define		HK_PATCH_MEMBER_RENAMED(OLDNAME,NEWNAME) {hkVersionPatchManager::PATCH_MEMBER_RENAMED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_FUNCTION(NAME) {hkVersionPatchManager::PATCH_FUNCTION, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define		HK_PATCH_MEMBER_REMOVED(NAME, TYPE, TYPE_NAME, TUPLES) {hkVersionPatchManager::PATCH_MEMBER_REMOVED, HK_PREPROCESSOR_JOIN_TOKEN(&p,__LINE__)},
#define	HK_PATCH_END() {hkVersionPatchManager::PATCH_INVALID, HK_NULL} }; static const hkVersionPatchManager::PatchInfo pinfo = {classname, newclassname, oldver, newver, patch, HK_COUNT_OF(patch) - 1}; man.addPatch(&pinfo); }
#		include HK_PATCHES_FILE
#undef	HK_PATCH_BEGIN
#undef		HK_PATCH_GROUP
#undef		HK_PATCH_CAST
#undef		HK_PATCH_DEPENDS
#undef		HK_PATCH_PARENT_SET

#undef		HK_PATCH_MEMBER_ADDED
#undef		HK_PATCH_MEMBER_ADDED_INT
#undef		HK_PATCH_MEMBER_ADDED_REAL
#undef		HK_PATCH_MEMBER_ADDED_BOOL
#undef		HK_PATCH_MEMBER_ADDED_BYTE
#undef		HK_PATCH_MEMBER_ADDED_VEC_4
#undef		HK_PATCH_MEMBER_ADDED_VEC_4_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_VEC_12
#undef		HK_PATCH_MEMBER_ADDED_VEC_12_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_VEC_16
#undef		HK_PATCH_MEMBER_ADDED_VEC_16_IDENTITY
#undef		HK_PATCH_MEMBER_ADDED_CSTRING
#undef		HK_PATCH_MEMBER_ADDED_POINTER

#undef		HK_PATCH_MEMBER_DEFAULT_SET_INT
#undef		HK_PATCH_MEMBER_DEFAULT_SET_REAL
#undef		HK_PATCH_MEMBER_DEFAULT_SET_BOOL
#undef		HK_PATCH_MEMBER_DEFAULT_SET_BYTE
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_4
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_16
#undef		HK_PATCH_MEMBER_DEFAULT_SET_VEC_12
#undef		HK_PATCH_MEMBER_DEFAULT_SET_CSTRING
#undef		HK_PATCH_MEMBER_DEFAULT_SET_POINTER

#undef		HK_PATCH_MEMBER_RENAMED
#undef		HK_PATCH_FUNCTION
#undef		HK_PATCH_MEMBER_REMOVED
#undef	HK_PATCH_END

#undef HK_CLASS_ADDED
#undef HK_CLASS_REMOVED
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
