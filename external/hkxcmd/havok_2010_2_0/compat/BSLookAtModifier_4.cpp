// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSLookAtModifier_4.h"

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


static const hkInternalClassMember BSLookAtModifierClass_Members[] =
{
   { "lookAtTarget",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_lookAtTarget) /*44*/,HK_NULL},
   { "bones",&BSLookAtModifierBoneDataClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_bones) /*48*/,HK_NULL},
   { "eyeBones",&BSLookAtModifierBoneDataClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_eyeBones) /*60*/,HK_NULL},
   { "limitAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_limitAngleDegrees) /*72*/,HK_NULL},
   { "limitAngleThresholdDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_limitAngleThresholdDegrees) /*76*/,HK_NULL},
   { "continueLookOutsideOfLimit",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_continueLookOutsideOfLimit) /*80*/,HK_NULL},
   { "onGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_onGain) /*84*/,HK_NULL},
   { "offGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_offGain) /*88*/,HK_NULL},
   { "useBoneGains",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_useBoneGains) /*92*/,HK_NULL},
   { "targetLocation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_targetLocation) /*96*/,HK_NULL},
   { "targetOutsideLimits",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_targetOutsideLimits) /*112*/,HK_NULL},
   { "targetOutOfLimitEvent",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_targetOutOfLimitEvent) /*116*/,HK_NULL},
   { "lookAtCamera",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_lookAtCamera) /*124*/,HK_NULL},
   { "lookAtCameraX",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_lookAtCameraX) /*128*/,HK_NULL},
   { "lookAtCameraY",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_lookAtCameraY) /*132*/,HK_NULL},
   { "lookAtCameraZ",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifier,m_lookAtCameraZ) /*136*/,HK_NULL},
   { "timeStep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSLookAtModifier,m_timeStep) /*140*/,HK_NULL},
   { "ballBonesValid",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSLookAtModifier,m_ballBonesValid) /*144*/,HK_NULL},
   { "pSkeletonMemory",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSLookAtModifier,m_pSkeletonMemory) /*148*/,HK_NULL},
};

// Signature:  d756fc25
extern const hkClass hkbModifierClass;
extern const hkClass BSLookAtModifierClass;
const hkClass BSLookAtModifierClass(
    "BSLookAtModifier",
    &hkbModifierClass, // parent
    sizeof(BSLookAtModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSLookAtModifierClass_Members), HK_COUNT_OF(BSLookAtModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    4 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSLookAtModifier, BSLookAtModifier);

