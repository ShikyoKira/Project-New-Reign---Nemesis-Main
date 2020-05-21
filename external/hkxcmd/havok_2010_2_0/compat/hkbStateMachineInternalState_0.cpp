// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbStateMachineInternalState_0.h"

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


static const hkInternalClassMember hkbStateMachineInternalStateClass_Members[] =
{
   { "activeTransitions",&hkbStateMachineActiveTransitionInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_activeTransitions) /*8*/,HK_NULL},
   { "transitionFlags",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_transitionFlags) /*20*/,HK_NULL},
   { "wildcardTransitionFlags",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_wildcardTransitionFlags) /*32*/,HK_NULL},
   { "delayedTransitions",&hkbStateMachineDelayedTransitionInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_delayedTransitions) /*44*/,HK_NULL},
   { "timeInState",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_timeInState) /*56*/,HK_NULL},
   { "lastLocalTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_lastLocalTime) /*60*/,HK_NULL},
   { "currentStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_currentStateId) /*64*/,HK_NULL},
   { "previousStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_previousStateId) /*68*/,HK_NULL},
   { "nextStartStateIndexOverride",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_nextStartStateIndexOverride) /*72*/,HK_NULL},
   { "stateOrTransitionChanged",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_stateOrTransitionChanged) /*76*/,HK_NULL},
   { "echoNextUpdate",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineInternalState,m_echoNextUpdate) /*77*/,HK_NULL},
};

// Signature:  bd1a7502
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbStateMachineInternalStateClass;
const hkClass hkbStateMachineInternalStateClass(
    "hkbStateMachineInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbStateMachineInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbStateMachineInternalStateClass_Members), HK_COUNT_OF(hkbStateMachineInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbStateMachineInternalState, hkbStateMachineInternalState);

