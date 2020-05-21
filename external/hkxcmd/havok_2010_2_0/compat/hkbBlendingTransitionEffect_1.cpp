// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBlendingTransitionEffect_1.h"

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


static const hkInternalClassEnumItem FlagBitsEnumItems[] =
{
   {0, "FLAG_NONE"},
   {1, "FLAG_IGNORE_FROM_WORLD_FROM_MODEL"},
   {2, "FLAG_SYNC"},
   {4, "FLAG_IGNORE_TO_WORLD_FROM_MODEL"},
};

static const hkInternalClassEnumItem EndModeEnumItems[] =
{
   {0, "END_MODE_NONE"},
   {1, "END_MODE_TRANSITION_UNTIL_END_OF_FROM_GENERATOR"},
   {2, "END_MODE_CAP_DURATION_AT_END_OF_FROM_GENERATOR"},
};

static const hkInternalClassEnum hkbBlendingTransitionEffectClass_Enums[] = {
   {"FlagBits", FlagBitsEnumItems, _countof(FlagBitsEnumItems), HK_NULL, 0 },
   {"EndMode", EndModeEnumItems, _countof(EndModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* FlagBitsEnum = reinterpret_cast<const hkClassEnum*>(&hkbBlendingTransitionEffectClass_Enums[0]);
const hkClassEnum* EndModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbBlendingTransitionEffectClass_Enums[1]);
extern const hkClassEnum* BlendCurveEnum;

static const hkInternalClassMember hkbBlendingTransitionEffectClass_Members[] =
{
   { "duration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_duration) /*44*/,HK_NULL},
   { "toGeneratorStartTimeFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_toGeneratorStartTimeFraction) /*48*/,HK_NULL},
   { "flags",HK_NULL,FlagBitsEnum,hkClassMember::TYPE_FLAGS,hkClassMember::TYPE_UINT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_flags) /*52*/,HK_NULL},
   { "endMode",HK_NULL,EndModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_endMode) /*54*/,HK_NULL},
   { "blendCurve",HK_NULL,BlendCurveEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_blendCurve) /*55*/,HK_NULL},
   { "fromGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_fromGenerator) /*56*/,HK_NULL},
   { "toGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_toGenerator) /*60*/,HK_NULL},
   { "characterPoseAtBeginningOfTransition",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_characterPoseAtBeginningOfTransition) /*64*/,HK_NULL},
   { "timeRemaining",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_timeRemaining) /*76*/,HK_NULL},
   { "timeInTransition",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_timeInTransition) /*80*/,HK_NULL},
   { "applySelfTransition",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_applySelfTransition) /*84*/,HK_NULL},
   { "initializeCharacterPose",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlendingTransitionEffect,m_initializeCharacterPose) /*85*/,HK_NULL},
};

// Signature:  fd8584fe
extern const hkClass hkbTransitionEffectClass;
extern const hkClass hkbBlendingTransitionEffectClass;
const hkClass hkbBlendingTransitionEffectClass(
    "hkbBlendingTransitionEffect",
    &hkbTransitionEffectClass, // parent
    sizeof(hkbBlendingTransitionEffect),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbBlendingTransitionEffectClass_Enums), HK_COUNT_OF(hkbBlendingTransitionEffectClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbBlendingTransitionEffectClass_Members), HK_COUNT_OF(hkbBlendingTransitionEffectClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBlendingTransitionEffect, hkbBlendingTransitionEffect);

