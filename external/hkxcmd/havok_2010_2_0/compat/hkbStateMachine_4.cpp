// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbStateMachine_4.h"

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


static const hkInternalClassEnumItem StartStateModeEnumItems[] =
{
   {0, "START_STATE_MODE_DEFAULT"},
   {1, "START_STATE_MODE_SYNC"},
   {2, "START_STATE_MODE_RANDOM"},
   {3, "START_STATE_MODE_CHOOSER"},
};

static const hkInternalClassEnumItem StateMachineSelfTransitionModeEnumItems[] =
{
   {0, "SELF_TRANSITION_MODE_NO_TRANSITION"},
   {1, "SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE"},
   {2, "SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE"},
};

static const hkInternalClassEnum hkbStateMachineClass_Enums[] = {
   {"StartStateMode", StartStateModeEnumItems, _countof(StartStateModeEnumItems), HK_NULL, 0 },
   {"StateMachineSelfTransitionMode", StateMachineSelfTransitionModeEnumItems, _countof(StateMachineSelfTransitionModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* StartStateModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbStateMachineClass_Enums[0]);
const hkClassEnum* StateMachineSelfTransitionModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbStateMachineClass_Enums[1]);

static const hkInternalClassMember hkbStateMachineClass_Members[] =
{
   { "eventToSendWhenStateOrTransitionChanges",&hkbEventClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_eventToSendWhenStateOrTransitionChanges) /*40*/,HK_NULL},
   { "startStateChooser",&hkbStateChooserClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_startStateChooser) /*52*/,HK_NULL},
   { "startStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_startStateId) /*56*/,HK_NULL},
   { "returnToPreviousStateEventId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_returnToPreviousStateEventId) /*60*/,HK_NULL},
   { "randomTransitionEventId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_randomTransitionEventId) /*64*/,HK_NULL},
   { "transitionToNextHigherStateEventId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_transitionToNextHigherStateEventId) /*68*/,HK_NULL},
   { "transitionToNextLowerStateEventId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_transitionToNextLowerStateEventId) /*72*/,HK_NULL},
   { "syncVariableIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_syncVariableIndex) /*76*/,HK_NULL},
   { "currentStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_currentStateId) /*80*/,HK_NULL},
   { "wrapAroundStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_wrapAroundStateId) /*84*/,HK_NULL},
   { "maxSimultaneousTransitions",HK_NULL,HK_NULL,hkClassMember::TYPE_INT8,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_maxSimultaneousTransitions) /*85*/,HK_NULL},
   { "startStateMode",HK_NULL,StartStateModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_startStateMode) /*86*/,HK_NULL},
   { "selfTransitionMode",HK_NULL,StateMachineSelfTransitionModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_selfTransitionMode) /*87*/,HK_NULL},
   { "isActive",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_isActive) /*88*/,HK_NULL},
   { "states",&hkbStateMachineStateInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_states) /*92*/,HK_NULL},
   { "wildcardTransitions",&hkbStateMachineTransitionInfoArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachine,m_wildcardTransitions) /*104*/,HK_NULL},
   { "stateIdToIndexMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_stateIdToIndexMap) /*108*/,HK_NULL},
   { "activeTransitions",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_activeTransitions) /*112*/,HK_NULL},
   { "transitionFlags",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_transitionFlags) /*124*/,HK_NULL},
   { "wildcardTransitionFlags",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_wildcardTransitionFlags) /*136*/,HK_NULL},
   { "delayedTransitions",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_delayedTransitions) /*148*/,HK_NULL},
   { "timeInState",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_timeInState) /*160*/,HK_NULL},
   { "lastLocalTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_lastLocalTime) /*164*/,HK_NULL},
   { "previousStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_previousStateId) /*168*/,HK_NULL},
   { "nextStartStateIndexOverride",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_nextStartStateIndexOverride) /*172*/,HK_NULL},
   { "stateOrTransitionChanged",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_stateOrTransitionChanged) /*176*/,HK_NULL},
   { "echoNextUpdate",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_echoNextUpdate) /*177*/,HK_NULL},
   { "sCurrentStateIndexAndEntered",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT16,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachine,m_sCurrentStateIndexAndEntered) /*178*/,HK_NULL},
};

// Signature:  816c1dcb
extern const hkClass hkbGeneratorClass;
extern const hkClass hkbStateMachineClass;
const hkClass hkbStateMachineClass(
    "hkbStateMachine",
    &hkbGeneratorClass, // parent
    sizeof(hkbStateMachine),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbStateMachineClass_Enums), HK_COUNT_OF(hkbStateMachineClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbStateMachineClass_Members), HK_COUNT_OF(hkbStateMachineClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    4 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbStateMachine, hkbStateMachine);

