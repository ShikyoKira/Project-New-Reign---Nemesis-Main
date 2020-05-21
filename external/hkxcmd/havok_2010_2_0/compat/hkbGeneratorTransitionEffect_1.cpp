// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbGeneratorTransitionEffect_1.h"

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


static const hkInternalClassEnumItem ToGeneratorStateEnumItems[] =
{
   {0, "STATE_INACTIVE"},
   {1, "STATE_READY_FOR_SET_LOCAL_TIME"},
   {2, "STATE_READY_FOR_APPLY_SELF_TRANSITION_MODE"},
   {3, "STATE_ACTIVE"},
};

static const hkInternalClassEnumItem StageEnumItems[] =
{
   {0, "STAGE_BLENDING_IN"},
   {1, "STAGE_PLAYING_TRANSITION_GENERATOR"},
   {2, "STAGE_BLENDING_OUT"},
};

static const hkInternalClassEnum hkbGeneratorTransitionEffectClass_Enums[] = {
   {"ToGeneratorState", ToGeneratorStateEnumItems, _countof(ToGeneratorStateEnumItems), HK_NULL, 0 },
   {"Stage", StageEnumItems, _countof(StageEnumItems), HK_NULL, 0 },
};
extern const hkClassEnum* ToGeneratorStateEnum;
extern const hkClassEnum* StageEnum;

const hkClassEnum* ToGeneratorStateEnum = reinterpret_cast<const hkClassEnum*>(&hkbGeneratorTransitionEffectClass_Enums[0]);
const hkClassEnum* StageEnum = reinterpret_cast<const hkClassEnum*>(&hkbGeneratorTransitionEffectClass_Enums[1]);

static const hkInternalClassMember hkbGeneratorTransitionEffectClass_Members[] =
{
   { "transitionGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_transitionGenerator) /*44*/,HK_NULL},
   { "blendInDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_blendInDuration) /*48*/,HK_NULL},
   { "blendOutDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_blendOutDuration) /*52*/,HK_NULL},
   { "syncToGeneratorStartTime",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_syncToGeneratorStartTime) /*56*/,HK_NULL},
   { "fromGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_fromGenerator) /*60*/,HK_NULL},
   { "toGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_toGenerator) /*64*/,HK_NULL},
   { "timeInTransition",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_timeInTransition) /*68*/,HK_NULL},
   { "duration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_duration) /*72*/,HK_NULL},
   { "effectiveBlendInDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_effectiveBlendInDuration) /*76*/,HK_NULL},
   { "effectiveBlendOutDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_effectiveBlendOutDuration) /*80*/,HK_NULL},
   { "toGeneratorState",HK_NULL,HK_NULL,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_toGeneratorState) /*84*/,HK_NULL},
   { "echoTransitionGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_echoTransitionGenerator) /*85*/,HK_NULL},
   { "echoToGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_echoToGenerator) /*86*/,HK_NULL},
   { "justActivated",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_justActivated) /*87*/,HK_NULL},
   { "updateActiveNodes",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_updateActiveNodes) /*88*/,HK_NULL},
   { "stage",HK_NULL,HK_NULL,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbGeneratorTransitionEffect,m_stage) /*89*/,HK_NULL},
};

// Signature:  5f771b12
extern const hkClass hkbTransitionEffectClass;
extern const hkClass hkbGeneratorTransitionEffectClass;
const hkClass hkbGeneratorTransitionEffectClass(
    "hkbGeneratorTransitionEffect",
    &hkbTransitionEffectClass, // parent
    sizeof(hkbGeneratorTransitionEffect),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbGeneratorTransitionEffectClass_Enums), HK_COUNT_OF(hkbGeneratorTransitionEffectClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbGeneratorTransitionEffectClass_Members), HK_COUNT_OF(hkbGeneratorTransitionEffectClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbGeneratorTransitionEffect, hkbGeneratorTransitionEffect);

