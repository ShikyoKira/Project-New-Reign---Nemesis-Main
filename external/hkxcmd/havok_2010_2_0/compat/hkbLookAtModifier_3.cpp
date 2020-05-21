// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbLookAtModifier_3.h"

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


static const hkInternalClassMember hkbLookAtModifierClass_Members[] =
{
   { "targetWS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_targetWS) /*48*/,HK_NULL},
   { "headForwardLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_headForwardLS) /*64*/,HK_NULL},
   { "neckForwardLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_neckForwardLS) /*80*/,HK_NULL},
   { "neckRightLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_neckRightLS) /*96*/,HK_NULL},
   { "eyePositionHS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_eyePositionHS) /*112*/,HK_NULL},
   { "newTargetGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_newTargetGain) /*128*/,HK_NULL},
   { "onGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_onGain) /*132*/,HK_NULL},
   { "offGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_offGain) /*136*/,HK_NULL},
   { "limitAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_limitAngleDegrees) /*140*/,HK_NULL},
   { "limitAngleLeft",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_limitAngleLeft) /*144*/,HK_NULL},
   { "limitAngleRight",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_limitAngleRight) /*148*/,HK_NULL},
   { "limitAngleUp",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_limitAngleUp) /*152*/,HK_NULL},
   { "limitAngleDown",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_limitAngleDown) /*156*/,HK_NULL},
   { "headIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_headIndex) /*160*/,HK_NULL},
   { "neckIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_neckIndex) /*162*/,HK_NULL},
   { "isOn",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_isOn) /*164*/,HK_NULL},
   { "individualLimitsOn",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_individualLimitsOn) /*165*/,HK_NULL},
   { "isTargetInsideLimitCone",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbLookAtModifier,m_isTargetInsideLimitCone) /*166*/,HK_NULL},
   { "lookAtLastTargetWS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbLookAtModifier,m_lookAtLastTargetWS) /*176*/,HK_NULL},
   { "lookAtWeight",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbLookAtModifier,m_lookAtWeight) /*192*/,HK_NULL},
};

// Signature:  3d28e066
extern const hkClass hkbModifierClass;
extern const hkClass hkbLookAtModifierClass;
const hkClass hkbLookAtModifierClass(
    "hkbLookAtModifier",
    &hkbModifierClass, // parent
    sizeof(hkbLookAtModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbLookAtModifierClass_Members), HK_COUNT_OF(hkbLookAtModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    3 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbLookAtModifier, hkbLookAtModifier);

