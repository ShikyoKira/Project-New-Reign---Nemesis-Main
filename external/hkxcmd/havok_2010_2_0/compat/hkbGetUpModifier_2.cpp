// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbGetUpModifier_2.h"

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


static const hkInternalClassMember hkbGetUpModifierClass_Members[] =
{
   { "groundNormal",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetUpModifier,m_groundNormal) /*48*/,HK_NULL},
   { "duration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetUpModifier,m_duration) /*64*/,HK_NULL},
   { "alignWithGroundDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetUpModifier,m_alignWithGroundDuration) /*68*/,HK_NULL},
   { "rootBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetUpModifier,m_rootBoneIndex) /*72*/,HK_NULL},
   { "otherBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetUpModifier,m_otherBoneIndex) /*74*/,HK_NULL},
   { "anotherBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetUpModifier,m_anotherBoneIndex) /*76*/,HK_NULL},
   { "timeSinceBegin",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGetUpModifier,m_timeSinceBegin) /*80*/,HK_NULL},
   { "timeStep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGetUpModifier,m_timeStep) /*84*/,HK_NULL},
   { "initNextModify",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGetUpModifier,m_initNextModify) /*88*/,HK_NULL},
};

// Signature:  61cb7ac0
extern const hkClass hkbModifierClass;
extern const hkClass hkbGetUpModifierClass;
const hkClass hkbGetUpModifierClass(
    "hkbGetUpModifier",
    &hkbModifierClass, // parent
    sizeof(hkbGetUpModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbGetUpModifierClass_Members), HK_COUNT_OF(hkbGetUpModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbGetUpModifier, hkbGetUpModifier);

