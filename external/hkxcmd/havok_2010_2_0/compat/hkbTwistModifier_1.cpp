// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbTwistModifier_1.h"

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


static const hkInternalClassEnumItem SetAngleMethodEnumItems[] =
{
   {0, "LINEAR"},
   {1, "RAMPED"},
};

static const hkInternalClassEnumItem RotationAxisCoordinatesEnumItems[] =
{
   {0, "ROTATION_AXIS_IN_MODEL_COORDINATES"},
   {1, "ROTATION_AXIS_IN_LOCAL_COORDINATES"},
};

static const hkInternalClassEnum hkbTwistModifierClass_Enums[] = {
   {"SetAngleMethod", SetAngleMethodEnumItems, _countof(SetAngleMethodEnumItems), HK_NULL, 0 },
   {"RotationAxisCoordinates", RotationAxisCoordinatesEnumItems, _countof(RotationAxisCoordinatesEnumItems), HK_NULL, 0 },
};
const hkClassEnum* SetAngleMethodEnum = reinterpret_cast<const hkClassEnum*>(&hkbTwistModifierClass_Enums[0]);
const hkClassEnum* RotationAxisCoordinatesEnum = reinterpret_cast<const hkClassEnum*>(&hkbTwistModifierClass_Enums[1]);

static const hkInternalClassMember hkbTwistModifierClass_Members[] =
{
   { "axisOfRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTwistModifier,m_axisOfRotation) /*48*/,HK_NULL},
   { "twistAngle",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTwistModifier,m_twistAngle) /*64*/,HK_NULL},
   { "startBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTwistModifier,m_startBoneIndex) /*68*/,HK_NULL},
   { "endBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTwistModifier,m_endBoneIndex) /*70*/,HK_NULL},
   { "setAngleMethod",HK_NULL,SetAngleMethodEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTwistModifier,m_setAngleMethod) /*72*/,HK_NULL},
   { "rotationAxisCoordinates",HK_NULL,RotationAxisCoordinatesEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTwistModifier,m_rotationAxisCoordinates) /*73*/,HK_NULL},
   { "isAdditive",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTwistModifier,m_isAdditive) /*74*/,HK_NULL},
   { "boneChainIndices",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbTwistModifier,m_boneChainIndices) /*76*/,HK_NULL},
   { "parentBoneIndices",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbTwistModifier,m_parentBoneIndices) /*88*/,HK_NULL},
};

// Signature:  b6b76b32
extern const hkClass hkbModifierClass;
extern const hkClass hkbTwistModifierClass;
const hkClass hkbTwistModifierClass(
    "hkbTwistModifier",
    &hkbModifierClass, // parent
    sizeof(hkbTwistModifier),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbTwistModifierClass_Enums), HK_COUNT_OF(hkbTwistModifierClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbTwistModifierClass_Members), HK_COUNT_OF(hkbTwistModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbTwistModifier, hkbTwistModifier);

