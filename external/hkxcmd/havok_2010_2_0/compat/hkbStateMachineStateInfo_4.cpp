// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbStateMachineStateInfo_4.h"

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


static const hkInternalClassMember hkbStateMachineStateInfoClass_Members[] =
{
   { "listeners",&hkbStateListenerClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_listeners) /*28*/,HK_NULL},
   { "enterNotifyEvents",&hkbStateMachineEventPropertyArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_enterNotifyEvents) /*40*/,HK_NULL},
   { "exitNotifyEvents",&hkbStateMachineEventPropertyArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_exitNotifyEvents) /*44*/,HK_NULL},
   { "transitions",&hkbStateMachineTransitionInfoArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_transitions) /*48*/,HK_NULL},
   { "generator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_generator) /*52*/,HK_NULL},
   { "name",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_name) /*56*/,HK_NULL},
   { "stateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_stateId) /*60*/,HK_NULL},
   { "probability",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_probability) /*64*/,HK_NULL},
   { "enable",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineStateInfo,m_enable) /*68*/,HK_NULL},
};

// Signature:  0ed7f9d0
extern const hkClass hkbBindableClass;
extern const hkClass hkbStateMachineStateInfoClass;
const hkClass hkbStateMachineStateInfoClass(
    "hkbStateMachineStateInfo",
    &hkbBindableClass, // parent
    sizeof(hkbStateMachineStateInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbStateMachineStateInfoClass_Members), HK_COUNT_OF(hkbStateMachineStateInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    4 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbStateMachineStateInfo, hkbStateMachineStateInfo);

