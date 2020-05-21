// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbFootIkDriverInfo_0.h"

#include <Common/Serialize/hkSerialize.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Common/Serialize/Version/hkVersionPatchManager.h>
#include <Common/Serialize/Data/Dict/hkDataObjectDict.h>
#include <Common/Serialize/Data/Native/hkDataObjectNative.h>
#include <Common/Serialize/Data/Util/hkDataObjectUtil.h>
#include <Common/Base/Reflection/Registry/hkDynamicClassNameRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Serialize/Util/hkSerializationCheckingUtils.h>
#include <Common/Serialize/Util/hkVersionCheckingUtils.h>


static const hkInternalClassMember hkbFootIkDriverInfoClass_Members[] =
{
   { "legs",&hkbFootIkDriverInfoLegClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_legs) /*8*/,HK_NULL},
   { "raycastDistanceUp",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_raycastDistanceUp) /*20*/,HK_NULL},
   { "raycastDistanceDown",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_raycastDistanceDown) /*24*/,HK_NULL},
   { "originalGroundHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_originalGroundHeightMS) /*28*/,HK_NULL},
   { "verticalOffset",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_verticalOffset) /*32*/,HK_NULL},
   { "collisionFilterInfo",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_collisionFilterInfo) /*36*/,HK_NULL},
   { "forwardAlignFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_forwardAlignFraction) /*40*/,HK_NULL},
   { "sidewaysAlignFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_sidewaysAlignFraction) /*44*/,HK_NULL},
   { "sidewaysSampleWidth",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_sidewaysSampleWidth) /*48*/,HK_NULL},
   { "lockFeetWhenPlanted",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_lockFeetWhenPlanted) /*52*/,HK_NULL},
   { "useCharacterUpVector",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_useCharacterUpVector) /*53*/,HK_NULL},
   { "isQuadrupedNarrow",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfo,m_isQuadrupedNarrow) /*54*/,HK_NULL},
};

// Signature:  c6a09dbf
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbFootIkDriverInfoClass;
const hkClass hkbFootIkDriverInfoClass(
    "hkbFootIkDriverInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbFootIkDriverInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbFootIkDriverInfoClass_Members), HK_COUNT_OF(hkbFootIkDriverInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbFootIkDriverInfo, hkbFootIkDriverInfo);

