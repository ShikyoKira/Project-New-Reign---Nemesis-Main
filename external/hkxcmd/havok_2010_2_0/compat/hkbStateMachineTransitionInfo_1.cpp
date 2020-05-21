// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbStateMachineTransitionInfo_1.h"

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


static const hkInternalClassEnumItem TransitionFlagsEnumItems[] =
{
   {1, "FLAG_USE_TRIGGER_INTERVAL"},
   {2, "FLAG_USE_INITIATE_INTERVAL"},
   {4, "FLAG_UNINTERRUPTIBLE_WHILE_PLAYING"},
   {8, "FLAG_UNINTERRUPTIBLE_WHILE_DELAYED"},
   {16, "FLAG_DELAY_STATE_CHANGE"},
   {32, "FLAG_DISABLED"},
   {64, "FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE"},
   {128, "FLAG_DISALLOW_RANDOM_TRANSITION"},
   {256, "FLAG_DISABLE_CONDITION"},
   {512, "FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE"},
   {1024, "FLAG_IS_GLOBAL_WILDCARD"},
   {2048, "FLAG_IS_LOCAL_WILDCARD"},
   {4096, "FLAG_FROM_NESTED_STATE_ID_IS_VALID"},
   {8192, "FLAG_TO_NESTED_STATE_ID_IS_VALID"},
   {16384, "FLAG_ABUT_AT_END_OF_FROM_GENERATOR"},
};

static const hkInternalClassEnumItem InternalFlagBitsEnumItems[] =
{
   {1, "FLAG_INTERNAL_IN_TRIGGER_INTERVAL"},
   {2, "FLAG_INTERNAL_IN_INITIATE_INTERVAL"},
};

static const hkInternalClassEnum hkbStateMachineTransitionInfoClass_Enums[] = {
   {"TransitionFlags", TransitionFlagsEnumItems, _countof(TransitionFlagsEnumItems), HK_NULL, 0 },
   {"InternalFlagBits", InternalFlagBitsEnumItems, _countof(InternalFlagBitsEnumItems), HK_NULL, 0 },
};
const hkClassEnum* TransitionFlagsEnum = reinterpret_cast<const hkClassEnum*>(&hkbStateMachineTransitionInfoClass_Enums[0]);
const hkClassEnum* InternalFlagBitsEnum = reinterpret_cast<const hkClassEnum*>(&hkbStateMachineTransitionInfoClass_Enums[1]);

static const hkInternalClassMember hkbStateMachineTransitionInfoClass_Members[] =
{
   { "triggerInterval",&hkbStateMachineTimeIntervalClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_triggerInterval) /*0*/,HK_NULL},
   { "initiateInterval",&hkbStateMachineTimeIntervalClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_initiateInterval) /*16*/,HK_NULL},
   { "transition",&hkbTransitionEffectClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_transition) /*32*/,HK_NULL},
   { "condition",&hkbConditionClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_condition) /*36*/,HK_NULL},
   { "eventId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_eventId) /*40*/,HK_NULL},
   { "toStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_toStateId) /*44*/,HK_NULL},
   { "fromNestedStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_fromNestedStateId) /*48*/,HK_NULL},
   { "toNestedStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_toNestedStateId) /*52*/,HK_NULL},
   { "priority",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_priority) /*56*/,HK_NULL},
   { "flags",HK_NULL,TransitionFlagsEnum,hkClassMember::TYPE_FLAGS,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineTransitionInfo,m_flags) /*58*/,HK_NULL},
};

// Signature:  cdec8025
extern const hkClass hkbStateMachineTransitionInfoClass;
const hkClass hkbStateMachineTransitionInfoClass(
    "hkbStateMachineTransitionInfo",
    HK_NULL, // parent
    sizeof(hkbStateMachineTransitionInfo),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbStateMachineTransitionInfoClass_Enums), HK_COUNT_OF(hkbStateMachineTransitionInfoClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbStateMachineTransitionInfoClass_Members), HK_COUNT_OF(hkbStateMachineTransitionInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbStateMachineTransitionInfo, hkbStateMachineTransitionInfo);

