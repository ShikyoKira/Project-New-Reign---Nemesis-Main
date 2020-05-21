// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbStateMachineActiveTransitionInfo_1.h"

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


static const hkInternalClassMember hkbStateMachineActiveTransitionInfoClass_Members[] =
{
   { "transitionEffect",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachineActiveTransitionInfo,m_transitionEffect) /*0*/,HK_NULL},
   { "transitionEffectInternalStateInfo",&hkbNodeInternalStateInfoClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineActiveTransitionInfo,m_transitionEffectInternalStateInfo) /*4*/,HK_NULL},
   { "transitionInfoReference",&hkbStateMachineTransitionInfoReferenceClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineActiveTransitionInfo,m_transitionInfoReference) /*8*/,HK_NULL},
   { "transitionInfoReferenceForTE",&hkbStateMachineTransitionInfoReferenceClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineActiveTransitionInfo,m_transitionInfoReferenceForTE) /*14*/,HK_NULL},
   { "fromStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineActiveTransitionInfo,m_fromStateId) /*20*/,HK_NULL},
   { "toStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineActiveTransitionInfo,m_toStateId) /*24*/,HK_NULL},
   { "isReturnToPreviousState",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineActiveTransitionInfo,m_isReturnToPreviousState) /*28*/,HK_NULL},
};

// Signature:  bb90d54f
extern const hkClass hkbStateMachineActiveTransitionInfoClass;
const hkClass hkbStateMachineActiveTransitionInfoClass(
    "hkbStateMachineActiveTransitionInfo",
    HK_NULL, // parent
    sizeof(hkbStateMachineActiveTransitionInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbStateMachineActiveTransitionInfoClass_Members), HK_COUNT_OF(hkbStateMachineActiveTransitionInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbStateMachineActiveTransitionInfo, hkbStateMachineActiveTransitionInfo);

