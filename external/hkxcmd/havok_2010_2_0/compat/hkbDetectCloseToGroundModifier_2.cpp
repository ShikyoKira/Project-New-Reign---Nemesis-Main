// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbDetectCloseToGroundModifier_2.h"

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


static const hkInternalClassMember hkbDetectCloseToGroundModifierClass_Members[] =
{
   { "closeToGroundEvent",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDetectCloseToGroundModifier,m_closeToGroundEvent) /*44*/,HK_NULL},
   { "closeToGroundHeight",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDetectCloseToGroundModifier,m_closeToGroundHeight) /*52*/,HK_NULL},
   { "raycastDistanceDown",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDetectCloseToGroundModifier,m_raycastDistanceDown) /*56*/,HK_NULL},
   { "collisionFilterInfo",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDetectCloseToGroundModifier,m_collisionFilterInfo) /*60*/,HK_NULL},
   { "boneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDetectCloseToGroundModifier,m_boneIndex) /*64*/,HK_NULL},
   { "animBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDetectCloseToGroundModifier,m_animBoneIndex) /*66*/,HK_NULL},
   { "isCloseToGround",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbDetectCloseToGroundModifier,m_isCloseToGround) /*68*/,HK_NULL},
};

// Signature:  981687b2
extern const hkClass hkbModifierClass;
extern const hkClass hkbDetectCloseToGroundModifierClass;
const hkClass hkbDetectCloseToGroundModifierClass(
    "hkbDetectCloseToGroundModifier",
    &hkbModifierClass, // parent
    sizeof(hkbDetectCloseToGroundModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbDetectCloseToGroundModifierClass_Members), HK_COUNT_OF(hkbDetectCloseToGroundModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbDetectCloseToGroundModifier, hkbDetectCloseToGroundModifier);

