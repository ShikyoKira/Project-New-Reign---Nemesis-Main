// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbStateMachineProspectiveTransitionInfo_2.h"

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


static const hkInternalClassMember hkbStateMachineProspectiveTransitionInfoClass_Members[] =
{
   { "transitionInfoReference",&hkbStateMachineTransitionInfoReferenceClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineProspectiveTransitionInfo,m_transitionInfoReference) /*0*/,HK_NULL},
   { "transitionInfoReferenceForTE",&hkbStateMachineTransitionInfoReferenceClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineProspectiveTransitionInfo,m_transitionInfoReferenceForTE) /*6*/,HK_NULL},
   { "toStateId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbStateMachineProspectiveTransitionInfo,m_toStateId) /*12*/,HK_NULL},
};

// Signature:  3ab09a2e
extern const hkClass hkbStateMachineProspectiveTransitionInfoClass;
const hkClass hkbStateMachineProspectiveTransitionInfoClass(
    "hkbStateMachineProspectiveTransitionInfo",
    HK_NULL, // parent
    sizeof(hkbStateMachineProspectiveTransitionInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbStateMachineProspectiveTransitionInfoClass_Members), HK_COUNT_OF(hkbStateMachineProspectiveTransitionInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbStateMachineProspectiveTransitionInfo, hkbStateMachineProspectiveTransitionInfo);

