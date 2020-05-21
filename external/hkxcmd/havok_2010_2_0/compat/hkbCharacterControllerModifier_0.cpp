// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacterControllerModifier_0.h"

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


static const hkInternalClassEnumItem InitialVelocityCoordinatesEnumItems[] =
{
   {0, "INITIAL_VELOCITY_IN_WORLD_COORDINATES"},
   {1, "INITIAL_VELOCITY_IN_MODEL_COORDINATES"},
};

static const hkInternalClassEnumItem MotionModeEnumItems[] =
{
   {0, "MOTION_MODE_FOLLOW_ANIMATION"},
   {1, "MOTION_MODE_DYNAMIC"},
};

static const hkInternalClassEnum hkbCharacterControllerModifierClass_Enums[] = {
   {"InitialVelocityCoordinates", InitialVelocityCoordinatesEnumItems, _countof(InitialVelocityCoordinatesEnumItems), HK_NULL, 0 },
   {"MotionMode", MotionModeEnumItems, _countof(MotionModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* InitialVelocityCoordinatesEnum = reinterpret_cast<const hkClassEnum*>(&hkbCharacterControllerModifierClass_Enums[0]);
const hkClassEnum* MotionModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbCharacterControllerModifierClass_Enums[1]);

static const hkInternalClassMember hkbCharacterControllerModifierClass_Members[] =
{
   { "controlData",&hkbCharacterControllerControlDataClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifier,m_controlData) /*48*/,HK_NULL},
   { "initialVelocity",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifier,m_initialVelocity) /*80*/,HK_NULL},
   { "initialVelocityCoordinates",HK_NULL,InitialVelocityCoordinatesEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifier,m_initialVelocityCoordinates) /*96*/,HK_NULL},
   { "motionMode",HK_NULL,MotionModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifier,m_motionMode) /*97*/,HK_NULL},
   { "forceDownwardMomentum",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifier,m_forceDownwardMomentum) /*98*/,HK_NULL},
   { "applyGravity",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifier,m_applyGravity) /*99*/,HK_NULL},
   { "setInitialVelocity",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifier,m_setInitialVelocity) /*100*/,HK_NULL},
   { "isTouchingGround",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifier,m_isTouchingGround) /*101*/,HK_NULL},
   { "gravity",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacterControllerModifier,m_gravity) /*112*/,HK_NULL},
   { "timestep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacterControllerModifier,m_timestep) /*128*/,HK_NULL},
   { "isInitialVelocityAdded",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacterControllerModifier,m_isInitialVelocityAdded) /*132*/,HK_NULL},
};

// Signature:  f675d6fb
extern const hkClass hkbModifierClass;
extern const hkClass hkbCharacterControllerModifierClass;
const hkClass hkbCharacterControllerModifierClass(
    "hkbCharacterControllerModifier",
    &hkbModifierClass, // parent
    sizeof(hkbCharacterControllerModifier),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbCharacterControllerModifierClass_Enums), HK_COUNT_OF(hkbCharacterControllerModifierClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbCharacterControllerModifierClass_Members), HK_COUNT_OF(hkbCharacterControllerModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacterControllerModifier, hkbCharacterControllerModifier);

