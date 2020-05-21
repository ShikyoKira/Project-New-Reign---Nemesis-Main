// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbTransitionEffect_0.h"

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


static const hkInternalClassEnumItem SelfTransitionModeEnumItems[] =
{
   {0, "SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC"},
   {1, "SELF_TRANSITION_MODE_CONTINUE"},
   {2, "SELF_TRANSITION_MODE_RESET"},
   {3, "SELF_TRANSITION_MODE_BLEND"},
};

static const hkInternalClassEnumItem EventModeEnumItems[] =
{
   {0, "EVENT_MODE_DEFAULT"},
   {1, "EVENT_MODE_PROCESS_ALL"},
   {2, "EVENT_MODE_IGNORE_FROM_GENERATOR"},
   {3, "EVENT_MODE_IGNORE_TO_GENERATOR"},
};

static const hkInternalClassEnum hkbTransitionEffectClass_Enums[] = {
   {"SelfTransitionMode", SelfTransitionModeEnumItems, _countof(SelfTransitionModeEnumItems), HK_NULL, 0 },
   {"EventMode", EventModeEnumItems, _countof(EventModeEnumItems), HK_NULL, 0 },
};
extern const hkClassEnum* SelfTransitionModeEnum;
extern const hkClassEnum* EventModeEnum;

const hkClassEnum* SelfTransitionModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbTransitionEffectClass_Enums[0]);
const hkClassEnum* EventModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbTransitionEffectClass_Enums[1]);

static const hkInternalClassMember hkbTransitionEffectClass_Members[] =
{
   { "selfTransitionMode",HK_NULL,SelfTransitionModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransitionEffect,m_selfTransitionMode) /*40*/,HK_NULL},
   { "eventMode",HK_NULL,EventModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransitionEffect,m_eventMode) /*41*/,HK_NULL},
   { "defaultEventMode",HK_NULL,HK_NULL,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbTransitionEffect,m_defaultEventMode) /*42*/,HK_NULL},
};

// Signature:  945da157
extern const hkClass hkbGeneratorClass;
extern const hkClass hkbTransitionEffectClass;
const hkClass hkbTransitionEffectClass(
    "hkbTransitionEffect",
    &hkbGeneratorClass, // parent
    sizeof(hkbTransitionEffect),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbTransitionEffectClass_Enums), HK_COUNT_OF(hkbTransitionEffectClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbTransitionEffectClass_Members), HK_COUNT_OF(hkbTransitionEffectClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbTransitionEffect, hkbTransitionEffect);

